/**
 * Utility functions to wrap pid-controller.h
 *
 * @author Connor Imes
 * @date 2017-03-18
 */

#ifndef _PID_CONTROLLER_UTIL_H_
#define _PID_CONTROLLER_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "pid-controller.h"

/**
 * Create a controller.
 */
pid_controller* pid_controller_alloc_init(double reference, double min, double max, double initial, double k_i,
                                          uint32_t lb_length, const char* log_filename);

/**
 * Cleanup.
 */
int pid_controller_destroy_free(pid_controller* state);

#ifdef __cplusplus
}
#endif

#endif
