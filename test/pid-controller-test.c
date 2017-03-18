/**
 * Some very basic unit tests.
 */
// force assertions
#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include "pid-controller.h"

#define VERIFY_ITERATIONS 100

static const double MIN = 100.0;
static const double MAX = 200.0;
static const double INITIAL = 200.0;
static const double REFERENCE = 100.0;
static const double K_I = 0.0;

static const double EPSILON = 0.01;

static double abs_dbl(double val) {
  return val >= 0.0 ? val : -val;
}

int main(void) {
  pid_controller state;
  pid_controller_log_buffer lb;
  double u;
  // init
  int ret = pid_controller_init(&state, REFERENCE, MIN, MAX, INITIAL, K_I);
  assert(ret == 0);
  ret = pid_controller_set_logging(&state, &lb, 1, stdout);
  assert(ret == 0);
  // some middle config (so that "K_I" is computed usefully for later)
  u = pid_controller_adapt(&state, 0, REFERENCE * 1.5);
  assert(abs_dbl(u - 400.0/3.0) < EPSILON);
  // max config
  u = pid_controller_adapt(&state, 0, REFERENCE / 2);
  assert(abs_dbl(u - 200.0) < EPSILON);
  // min config
  u = pid_controller_adapt(&state, 0, REFERENCE * 2.0);
  assert(abs_dbl(u - 100.0) < EPSILON);
  // back to some middle config
  u = pid_controller_adapt(&state, 0, REFERENCE / 1.5);
  assert(abs_dbl(u - 1300.0/9.0) < EPSILON); // 144.444444
  // destroy
  pid_controller_destroy(&state);
  return 0;
}
