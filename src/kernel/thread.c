#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "assert.h"
#include "cpu.h"
#include "heap.h"
#include "list.h"
#include "thread.h"

#define THREAD_DEFAULT_STACK_SIZE 4096

struct thread_run_queue {
    struct list thread_list;
    size_t no_threads;
};

struct thread {
    uint32_t id;
    const char *name;
    uint32_t stack_ptr;
    size_t stack_size;
    bool ready;
    struct list node;
};

/*
 * Assembly interface to load first thread.
 *
 * Stack pointer is set to 'stack_ptr' and then the thread is 'restored' as if
 * it was a thread that had been preempted.
 */
void thread_load_first_asm(uint32_t stack_ptr);

/*
 * Assembly interface to context switch.
 *
 * The 'current_stack_ptr' argument is updated to hold the stack pointer of the
 * currently running thread, before the stack pointer is set to 'next_stack_ptr'
 * and the next thread to run is restored.
 */
void thread_context_switch_asm(uint32_t *current_stack_ptr,
                               uint32_t next_stack_ptr);

static struct thread_run_queue thread_run_queue;

static struct thread *idle_thread;
static struct thread *current_thread;

static uint32_t global_id_counter = 0;

static void
thread_run_queue_init(struct thread_run_queue *run_queue)
{
    list_init(&run_queue->thread_list);
    run_queue->no_threads = 0;
}

static bool
thread_run_queue_empty(const struct thread_run_queue *run_queue)
{
    return (run_queue->no_threads == 0);
}

static void
thread_run_queue_add(struct thread_run_queue *run_queue, struct thread *thread)
{
    list_add_tail(&thread->node, &run_queue->thread_list);
    run_queue->no_threads++;
}

static void
thread_runqueue_remove(struct thread_run_queue *run_queue, struct thread *thread)
{
    list_del(&thread->node);
    run_queue->no_threads--;
}

static struct thread *
thread_run_queue_pop(struct thread_run_queue *run_queue)
{
    struct thread *thread;
    
    assert(!thread_run_queue_empty(run_queue));

    thread = list_first_entry(&run_queue->thread_list, struct thread, node);

    thread_runqueue_remove(run_queue, thread);

    return thread;
}

static void
thread_load_first(struct thread *thread)
{
    thread_load_first_asm(thread->stack_ptr);
}

static void
thread_context_switch(struct thread *current, struct thread *next)
{
    thread_context_switch_asm(&current->stack_ptr, next->stack_ptr);
}

static struct thread *
thread_get_current(void)
{
    return current_thread;
}

static void
thread_set_current(struct thread *thread)
{
    current_thread = thread;
}

static bool
thread_is_idle(const struct thread *thread)
{
    return (thread == idle_thread);
}

static struct thread *
thread_get_next(void)
{
    struct thread *next;

    if (thread_run_queue_empty(&thread_run_queue)) {
        next = idle_thread;
    } else {
        next = thread_run_queue_pop(&thread_run_queue);
    }

    return next;
}

static void
thread_reschedule(void)
{
    struct thread *current, *next;

    current = thread_get_current();

    next = thread_get_next();

    thread_set_current(next);

    if ((!thread_is_idle(current)) && (current->ready)) {
        thread_run_queue_add(&thread_run_queue, current);
    }

    thread_context_switch(current, next);
}

static void
thread_main(thread_fn_t fn, void *arg)
{
    struct thread *current;

    current = thread_get_current();

    fn(arg);

    current->ready = false;

    thread_reschedule();
}

static void
thread_setup_stack(void *stack_base_addr,
                   size_t stack_size,
                   thread_fn_t fn,
                   void *arg,
                   uint32_t *stack_ptrp)
{
    uint32_t *stack_ptr;

    /* Stack grows downwards, sp points to the highest address. */
    stack_ptr = (uint32_t *)((uintptr_t)(stack_base_addr) + stack_size);

    /*
     * Setup stack frame for an implicit call to thread_main at the end of
     * thread_restore, for threads that are running for the first time.
     *
     * This allows us to treat new threads in the same way as threads that have
     * been preempted.
     */

    /* thread_main args, pushed right to left as per the calling convention. */
    stack_ptr[-1] = (uint32_t)arg;
    stack_ptr[-2] = (uint32_t)fn;

    /*
     * Fake return address. Would have been pushed if we used the 'call'
     * instruction to call thread_main.
     */
    stack_ptr[-3] = 0;

    /*
     * Return address at the point that we execute 'ret' at the end of
     * thread_restore, which means we jump to thread_main.
     */
    stack_ptr[-4] = (uint32_t)thread_main;

    /* Saved registers - initialize to 0. */
    stack_ptr[-5] = 0;
    stack_ptr[-6] = 0;
    stack_ptr[-7] = 0;
    stack_ptr[-8] = 0;

    /* Set sp to point to the top of the stack. */
    stack_ptr -= 8;

    *stack_ptrp = (uint32_t)stack_ptr;
}

/*
 * TODO: Naming issues, thread_init() taken by module initialization function.
 * Reconsider naming scheme.
 */
static void
thread_struct_init(struct thread *thread,
                   const char *name,
                   void *stack_base_addr,
                   size_t stack_size,
                   thread_fn_t fn,
                   void *arg)
{
    thread->id = global_id_counter++;
    thread->name = name;
    thread->ready = true;
    
    list_node_init(&thread->node);

    thread->stack_size = stack_size;

    thread_setup_stack(stack_base_addr,
                       stack_size,
                       fn,
                       arg,
                       &thread->stack_ptr);
}

static void
idle_thread_fn(void *arg)
{
    (void)arg;

    for (;;) {
        cpu_wait_for_interrupt();
    }
}

static int
idle_thread_create(struct thread **threadp)
{
    struct thread *thread;
    void *stack;
    size_t stack_size;

    thread = NULL;
    stack = NULL;

    thread = heap_malloc(sizeof(*thread));
    if (!thread) {
        goto out;
    }

    stack_size = THREAD_DEFAULT_STACK_SIZE;

    stack = heap_malloc(stack_size);
    if (!stack) {
        goto out;
    }

    thread_struct_init(thread,
                       "Idle thread",
                       stack,
                       stack_size,
                       idle_thread_fn,
                       NULL);

    *threadp = thread;

    return 0;

out:
    heap_free(thread);
    heap_free(stack);
    return 1; /* TODO: Define kernel-wide error codes. */
}

int
thread_init(void)
{
    int error;

    error = idle_thread_create(&idle_thread);
    if (error) {
        return error;
    }

    thread_run_queue_init(&thread_run_queue);

    return 0;
}

int
thread_create(struct thread **threadp,
              const char *name,
              thread_fn_t fn,
              void *arg)
{
    struct thread *thread;
    void *stack;
    size_t stack_size;

    thread = NULL;
    stack = NULL;

    thread = heap_malloc(sizeof(*thread));
    if (!thread) {
        goto out;
    }

    stack_size = THREAD_DEFAULT_STACK_SIZE;

    stack = heap_malloc(stack_size);
    if (!stack) {
        goto out;
    }

    thread_struct_init(thread, name, stack, stack_size, fn, arg);

    thread_run_queue_add(&thread_run_queue, thread);

    *threadp = thread;

    return 0;

out:
    heap_free(thread);
    heap_free(stack);
    return 1; /* TODO: Define kernel-wide error codes. */
}

void
thread_start_scheduler(void)
{
    struct thread *first;

    first = thread_get_next();

    thread_set_current(first);

    thread_load_first(first);

    /* Never reached. */
}
