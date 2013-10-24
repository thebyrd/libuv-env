#include <stdio.h>
#include <stdlib.h>
#include <uv.h>



void
after_write () {

}


uv_buf_t
on_alloc (uv_handle_t* handle, size_t suggested_size) {
  uv_buf_t buf;

  buf.base = malloc(suggested_size);
  buf.len = suggested_size;

  return buf;
}

void
on_close (uv_handle_t *handle) {
  free(handle);
}

void
on_read (uv_stream_t* tcp_handle, ssize_t nread, uv_buf_t buf) {
  if (nread < 0) {
    if (uv_last_error(uv_default_loop()).code == UV_EOF) { // lolz. EOF is an error.
      uv_close((uv_handle_t*) tcp_handle, on_close);
    } else {
      abort();
    }
  }

  if (nread > 0) {
    fwrite(buf.base, 1, nread, stdout);
  }

  free(buf.base);
}


void
after_connect (uv_connect_t * connect_req, int status) {
  uv_write_t* write_req;
  uv_buf_t buf;

  if (status < 0) abort();

  write_req = malloc(sizeof(uv_write_t));

  buf.base = "GET / HTTP/1.0\r\n"
             "HOST: bowery.io\r\n"
             "\r\n";

  buf.len = strlen(buf.base);

  uv_write(write_req, connect_req->handle, &buf, 1, after_write);
  uv_read_start(connect_req->handle, on_alloc, on_read);

  free(connect_req);
}


void
after_getaddrinfo (uv_getaddrinfo_t* gai_req, int status, struct addrinfo* ai) {
  uv_tcp_t* tcp_handle;
  uv_connect_t* connect_req;

  if (status < 0) abort();

  tcp_handle = malloc(sizeof(uv_tcp_t));
  uv_tcp_init(uv_default_loop(), tcp_handle);

  connect_req = malloc(sizeof(uv_connect_t));
  uv_tcp_connect(connect_req, tcp_handle, *(struct sockaddr_in*) ai->ai_addr, after_connect);

  free(gai_req);
  uv_freeaddrinfo(ai);
}



int
main (int argc, char* argv[]) {
  // uv_loop_t *loop = uv_loop_new();

  // printf("Now quititng.\n");
  // uv_run(loop, UV_RUN_DEFAULT);
  uv_getaddrinfo_t* gai_req = malloc(sizeof(uv_getaddrinfo_t));

  uv_getaddrinfo(uv_default_loop(),
                gai_req,
                after_getaddrinfo,
                "bowery.io",
                "80",
                NULL);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  return 0;
}