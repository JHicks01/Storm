#ifndef _ASSERT_H_
#define _ASSERT_H_

#include "panic.h"

#define assert(expression) \
    if (!(expression)) panic("Assertion failed: " #expression);

#endif /* _ASSERT_H_ */
