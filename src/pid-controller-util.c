/**
 * Utility functions.
 *
 * @author Connor Imes
 * @date 2017-03-18
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "pid-controller.h"
#include "pid-controller-util.h"

pid_controller* pid_controller_alloc_init(double reference, double min, double max, double initial, double k_i,
                                          uint32_t lb_length, const char* log_filename) {
  pid_controller* state;
  pid_controller_log_buffer* lb = NULL;
  FILE* lf = NULL;
  int err_save;

  // create the pid_controller struct
  state = malloc(sizeof(pid_controller));
  if (state == NULL) {
    return NULL;
  }
  
  // allocate log buffer
  if (lb_length > 0) {
    lb = malloc(lb_length * sizeof(pid_controller_log_buffer));
    if (lb == NULL) {
      free(state);
      return NULL;
    }
    // Open log file
    if (log_filename != NULL) {
      lf = fopen(log_filename, "w");
      if (lf == NULL) {
        free(lb);
        free(state);
        return NULL;
      }
    }
  }

  if (pid_controller_init(state, reference, min, max, initial, k_i) || pid_controller_set_logging(state, lb, lb_length, lf)) {
    err_save = errno;
    if (lf != NULL) {
      fclose(lf);
    }
    free(lb);
    free(state);
    errno = err_save;
    return NULL;
  }

  return state;
}

int pid_controller_destroy_free(pid_controller* state) {
  int ret = 0;
  if (state != NULL) {
    pid_controller_destroy(state);
    if (state->ls.lf != NULL) {
      ret |= fclose(state->ls.lf);
    }
    free(state->ls.lb);
    free(state);
  }
  return ret;
}
