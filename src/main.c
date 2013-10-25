#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;


/**
 * Print out the status code and signal. Also close connection to handle.
 */
void
on_exit (uv_process_t *req, int status, int signal) {
  fprintf(stderr, "Process exited with status %d, signal %d\n", status, signal);
  uv_close((uv_handle_t*) req, NULL);
}

/**
 * Will run whatever command follows ./main as a child process.
 * examples:
 *   ./main mkdir test-dir
 *   ./main rm -rf test-dir
 *   ./main node app.js
 */
int
main (int argc, char* args[]) {
  loop = uv_default_loop();

  char* child_args[argc];
  child_args[argc - 1] = NULL; // must have NULL as last element

  int i = argc - 2; // want to skip ./main
  while (i >= 0) {
    child_args[i] = args[i + 1];
    i--;
  }

  options.exit_cb = on_exit;
  options.file = child_args[0];
  options.args = child_args;

  if (uv_spawn(loop, &child_req, options)) {
    fprintf(stderr, "%s\n", uv_strerror(uv_last_error(loop)));
    return 1;
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}