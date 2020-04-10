#ifndef _I8254_H_
#define _I8254_H_

#include <stdint.h>

/* Initialize i8254 PIT. Timer interrupt will fire at frequency 'freq'. */
void i8254_init(uint32_t freq);

/*
 * Sleep for 'time_s' seconds. This API will be removed once the thread module
 * is written.
 */
void i8254_sleep(uint32_t time_s);

#endif /* _I8254_H_ */
