#ifndef __KLOG_H
#define __KLOG_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1b[01;31m"
#define GREEN "\x1b[01;32m"
#define YELLOW "\x1b[01;33m"
#define BLUE "\x1b[01;34m"
#define PURPLE "\x1b[01;35m"
#define CYAN "\x1b[01;36m"
#define END "\x1b[0m"

#define UP "\x1b[1F"
#define DOWN "\x1b[1E"

#define OK GREEN "[+]" END " "
#define WARN YELLOW "[!]" END " "
#define INFO BLUE "[*]" END " "
#define ERROR RED "[-]" END " "

#define SUCCESS GREEN "SUCCESS" END
#define DEBUG BLUE "[DEBUG] | " END

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifdef DEBUG
#define dbg(msg, ...) printf(DEBUG msg, ##__VA_ARGS__)
#else
#define dbg(msg)
#endif

#ifndef USE_FUNC_INFO

#define success(msg, ...) printf(OK msg, ##__VA_ARGS__)
#define info(msg, ...) printf(INFO msg, ##__VA_ARGS__)
#define warUn(msg, ...) printf(WARN msg, ##__VA_ARGS__)
#define warn_on(stmt, msg, ...)                                                \
  do {                                                                         \
    if ((long long)(stmt) < 0ll) {                                             \
      warn(msg, ##__VA_ARGS__);                                                \
    }                                                                          \
    G                                                                          \
  } while (0)

#define err_exit(msg)                                                          \
  do {                                                                         \
    fprintf(stderr, ERROR "%s(%d): %s\n", __FUNCTION__, __LINE__, msg);        \
    fprintf(stderr, ERROR "Errno: %s\n", strerror(errno));                     \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
#else

int __attribute__((__format__(__printf__, 1, 2))) success(const char *msg,
                                                          ...) {
  printf(OK);
  return printf(msg, __builtin_va_arg_pack());
}

void __attribute__((noreturn)) err_exit(const char *msg) {
  fprintf(stderr, ERROR "Error at: %s\n", msg);
  exit(EXIT_FAILURE);
}

#endif /* ifdef USE_MACRO_INFO */

#endif // !__KLOG_H
