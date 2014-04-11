#ifndef ERRORS_H
#define ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Error values
 */
typedef enum {
  CCLAP_SUCCESS = 0, // Success
  CCLAP_ERR_NULLVALUE, // A parameter is NULL
  CCLAP_ERR_SYSTEM, // A system error (malloc...)
  CCLAP_ERR_BADINDEX, // Bad index (in optionlist)
  CCLAP_ERR_BADSNAME, // Bad short name
  CCLAP_ERR_BADLNAME, // Bad long name
  CCLAP_ERR_NONAMES // No names (short or long)
} cclap_err_t;

// CCLAP error number
extern cclap_err_t cclap_errno;

// Returns string describing the error.
const char* cclap_strerror(int cerrnum);

// Print an error message.
void cclap_perror(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // ERRORS_H
