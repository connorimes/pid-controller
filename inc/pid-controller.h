/**
 * A PID controller.
 *
 * @author Connor Imes
 * @date 2017-03-18
 */

#ifndef _PID_CONTROLLER_H_
#define _PID_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdio.h>

// Stores user-defined parameters
typedef struct pid_controller_context {
  double reference;
  double min;
  double max;
  double K_I;
} pid_controller_context;

// Log file fields
typedef struct pid_controller_log_buffer {
  uint64_t id;
  uint64_t uid;
  double measured;
  double e;
  double u;
} pid_controller_log_buffer;

// Maintains logging config and state
typedef struct pid_controller_log_state {
  uint64_t id;
  uint32_t lb_length;
  pid_controller_log_buffer* lb;
  FILE* lf;
} pid_controller_log_state;

// The top-level context/state struct
typedef struct pid_controller {
  pid_controller_context ctx;
  pid_controller_log_state ls;
  double u;
} pid_controller;

/**
 * Initialize the controller.
 *
 * @param state
 *  not NULL
 * @param reference
 *  reference > 0
 * @param min
 *  min > 0
 * @param max
 *  max >= min
 * @param initial
 *  min <= initial <= max
 * @param k_i
 *  ratio of control change s.t. k_i > 0.  If <= 0, it will be estimated in the first iteration
 * @return 0 on success, -1 otherwise (errno will be set)
 */
int pid_controller_init(pid_controller* state, double reference, double min, double max, double initial, double k_i);

/**
 * Destroy the controller.
 *
 * @param state
 */
void pid_controller_destroy(pid_controller* state);

/**
 * Get the new control signal.
 *
 * @param state
 *  not NULL
 * @param uid
 *  A user-specified identifier
 * @param measured
 *  The measured output
 * @return the new control signal
 */
double pid_controller_adapt(pid_controller* state, uint64_t uid, double measured);

/**
 * Enable/disable logging.
 *
 * @param state
 *  not NULL
 * @param lb
 *  the log buffer (NULL to disable)
 * @param lb_length
 *  the log buffer length (0 to disable)
 * @param lf
 *  the log file (NULL to disable)
 * @return 0 on success, -1 otherwise (errno will be set)
 */
int pid_controller_set_logging(pid_controller* state, pid_controller_log_buffer* lb, uint32_t lb_length, FILE* lf);

/**
 * Change the reference input.
 *
 * @param state
 *  not NULL
 * @param reference
 *  reference > 0
 * @return 0 on success, -1 otherwise (errno will be set)
 */
int pid_controller_set_reference(pid_controller* state, double reference);

#ifdef __cplusplus
}
#endif

#endif
