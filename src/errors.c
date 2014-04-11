#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "cclap/errors.h"

cclap_err_t cclap_errno = CCLAP_SUCCESS;

static const char* cclap_errmsg[] = {"Success",
				     "NULL parameter",
				     "System error", // Useless because we use errno to display the system error
				     "Bad index",
				     "Bad short name",
				     "Bad long name",
				     "No names"};

const char* cclap_strerror(int errnum) {
  if (errnum < 0 || errnum > 6)
    return NULL;
  return (errnum == CCLAP_ERR_SYSTEM) ? strerror(errno) : cclap_errmsg[errnum];
}

void cclap_perror(const char* msg) {
  printf("%s: %s\n", msg, cclap_strerror(cclap_errno));
}
