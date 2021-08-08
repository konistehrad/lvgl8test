#ifndef DRIVER_H
#define DRIVER_H

#include "WString.h"

#ifdef __cplusplus
extern "C" {
#endif


void hal_setup(void);
void hal_loop(void (*fun_ptr)(uint32_t));


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
