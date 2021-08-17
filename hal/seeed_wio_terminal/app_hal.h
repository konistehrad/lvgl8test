#ifndef DRIVER_H
#define DRIVER_H

#include <FreeRTOS.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void hal_setup(void);
void hal_loop(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRIVER_H*/
