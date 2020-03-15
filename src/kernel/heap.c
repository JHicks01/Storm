/*

 * Copyright © 2017 Embedded Artistry LLC.
 *
 * MIT License
 *
 * Copyright (c) 2017 Embedded Artistry

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * -----------------------------------------------------------------------------
 *
 * Some minor changes have been made to satisfy naming preferences.
 *
 * #pragma marks also removed.
 */

#include <heap.h>
#include <list.h>
#include <stdint.h>

/**
 * Simple macro for making sure memory addresses are aligned
 * to the nearest power of two
 */
#ifndef align_up
#define align_up(num, align) (((num) + ((align)-1)) & ~((align)-1))
#endif

/*
 * This is the container for our free-list.
 * Node the usage of the linked list here: the library uses offsetof
 * and container_of to manage the list and get back to the parent struct.
 */
typedef struct
{
	ll_t node;
	size_t size;
	char* block;
} alloc_node_t;


/**
 * We vend a memory address to the user.  This lets us translate back and forth
 * between the vended pointer and the container we use for managing the data.
 */
#define ALLOC_HEADER_SZ offsetof(alloc_node_t, block)

// We are enforcing a minimum allocation size of 32B.
#define MIN_ALLOC_SZ ALLOC_HEADER_SZ + 32

#define HEAP_SIZE (4 * 1024 * 1024)

static void heap_defrag_free_list(void);
static void heap_add_block(void *addr, size_t size);

static uint8_t heap[HEAP_SIZE];

// This macro simply declares and initializes our linked list
static LIST_INIT(free_list);

/**
 * When we free, we can take our node and check to see if any memory blocks
 * can be combined into larger blocks.  This will help us fight against
 * memory fragmentation in a simple way.
 */
static void heap_defrag_free_list(void)
{
	alloc_node_t* b;
	alloc_node_t* lb = NULL;
	alloc_node_t* t;

	list_for_each_entry_safe(b, t, &free_list, node)
	{
		if(lb)
		{
			if((((uintptr_t)&lb->block) + lb->size) == (uintptr_t)b)
			{
				lb->size += sizeof(*b) + b->size;
				list_del(&b->node);
				continue;
			}
		}
		lb = b;
	}
}

void heap_init(void)
{
    heap_add_block(heap, HEAP_SIZE);
}

void* heap_malloc(size_t size)
{
	void* ptr = NULL;
	alloc_node_t* blk = NULL;

	if(size > 0)
	{
		// Align the pointer
		size = align_up(size, sizeof(void*));

		// try to find a big enough block to alloc
		list_for_each_entry(blk, &free_list, node)
		{
			if(blk->size >= size)
			{
				ptr = &blk->block;
				break;
			}
		}

		// we found something
		if(ptr)
		{
			// Can we split the block?
			if((blk->size - size) >= MIN_ALLOC_SZ)
			{
				alloc_node_t* new_blk;
				new_blk = (alloc_node_t*)((uintptr_t)(&blk->block) + size);
				new_blk->size = blk->size - size - ALLOC_HEADER_SZ;
				blk->size = size;
				list_insert(&new_blk->node, &blk->node, blk->node.next);
			}

			list_del(&blk->node);
		}

	} // else NULL

	return ptr;
}

void heap_free(void* ptr)
{
	alloc_node_t* free_blk;
	alloc_node_t* blk;

	// Don't free a NULL pointer..
	if(ptr)
	{
		// we take the pointer and use container_of to get the corresponding alloc block
		blk = container_of(ptr, alloc_node_t, block);

		// Let's put it back in the proper spot
		list_for_each_entry(free_blk, &free_list, node)
		{
			if(free_blk > blk)
			{
				list_insert(&blk->node, free_blk->node.prev, &free_blk->node);
				goto blockadded;
			}
		}
		list_add_tail(&blk->node, &free_list);

	blockadded:
		// Let's see if we can combine any memory
		heap_defrag_free_list();
	}
}

static void heap_add_block(void* addr, size_t size)
{
	alloc_node_t* blk;

	// let's align the start address of our block to the next pointer aligned number
	blk = (void*)align_up((uintptr_t)addr, sizeof(void*));

	// calculate actual size - remove our alignment and our header space from the availability
	blk->size = (uintptr_t)addr + size - (uintptr_t)blk - ALLOC_HEADER_SZ;

	// and now our giant block of memory is added to the list!
	list_add(&blk->node, &free_list);
}