/*
 * A PID controller.
 *
 * @author Connor Imes
 * @date 2017-03-18
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "pid-controller.h"

/**
 * If log file and buffer exist, writes "count" entries to the log file from the start of the log buffer.
 */
static void flush_log_file(FILE* lf, const pid_controller_log_buffer* lb, uint32_t count) {
  uint32_t i;
  for (i = 0; lb != NULL && lf != NULL && i < count; i++) {
    fprintf(lf,
            "%-16"PRIu64" %-16"PRIu64" %-16f %-16f %-16f\n",
            lb[i].id, lb[i].uid, lb[i].measured, lb[i].e, lb[i].u);
  }
}

/**
 * If the circular log buffer exists, record an entry in it.
 * If the log file exists and the buffer becomes full, the buffer will flush its contents to the file.
 */
static void pid_controller_log(pid_controller* state, uint32_t uid, double measured, double e) {
  uint32_t i;
  pid_controller_log_state* ls = &state->ls;
  pid_controller_log_buffer* lb = ls->lb;

  if (ls->lb_length > 0 && lb != NULL) {
    i = ls->id % ls->lb_length;
    lb[i].id = ls->id;
    lb[i].uid = uid;
    lb[i].measured = measured;
    lb[i].e = e;
    lb[i].u = state->u;

    if (i == ls->lb_length - 1) {
      // flush buffer to log file (if used)
      flush_log_file(ls->lf, lb, ls->lb_length);
    }
    ls->id++;
  }
}

int pid_controller_init(pid_controller* state, double reference, double min, double max, double initial, double k_i) {
  if (state == NULL || reference <= 0 || min <= 0 || max < min || initial < min || initial > max) {
    errno = EINVAL;
    return -1;
  }

  // set the context
  state->ctx.reference = reference;
  state->ctx.min = min;
  state->ctx.max = max;
  state->ctx.K_I = k_i;

  // no logging by default
  state->ls.id = 0;
  state->ls.lb_length = 0;
  state->ls.lb = NULL;
  state->ls.lf = NULL;

  // initial signal value
  state->u = initial;

  return 0;
}

void pid_controller_destroy(pid_controller* state) {
  // flush log buffer (if used)
  if (state != NULL && state->ls.lb_length > 0) {
    flush_log_file(state->ls.lf, state->ls.lb, state->ls.id % state->ls.lb_length);
  }
}

static double clamp(double val, double min, double max) {
  return val < min ? min : (val > max ? max : val);
}

double pid_controller_adapt(pid_controller* state, uint64_t uid, double measured) {
  if (state == NULL || measured <= 0) {
    errno = EINVAL;
    return 0;
  }

  pid_controller_context* ctx = &state->ctx;

  if (ctx->K_I <= 0) {
    // initialize
    ctx->K_I = state->u / measured;
  }

  // compute error and control signal
  double e = ctx->reference - measured;
  double u = state->u + (ctx->K_I * e);
  state->u = clamp(u, ctx->min, ctx->max);

  // internal logging
  pid_controller_log(state, uid, measured, e);

  return state->u;
}

int pid_controller_set_logging(pid_controller* state, pid_controller_log_buffer* lb, uint32_t lb_length, FILE* lf) {
  if (state == NULL) {
    errno = EINVAL;
    return -1;
  }
  if (lf != NULL) {
    // write header to log file
    if (fprintf(lf,
                "%-16s %-16s %-16s %-16s %-16s\n",
                "ID", "USER_TAG", "MEASURED", "ERROR", "U") < 0) {
      return -1;
    }
  }
  // reset id (prevents writing garbage to files when logging is unset/set)
  state->ls.id = 0;
  state->ls.lb_length = lb_length;
  state->ls.lb = lb;
  state->ls.lf = lf;
  return 0;
}

int pid_controller_set_reference(pid_controller* state, double reference) {
  if (state == NULL || reference <= 0) {
    errno = EINVAL;
    return -1;
  }
  state->ctx.reference = reference;
  return 0;
}
