#ifndef PARSER_H
#define PARSER_H

#include "cclap/optionlist.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Parses the argument and modifies the options in the option list (isset and value if an argument is needed).
 * 
 * On success, returns the first non-option argument index.
 * On error, returns -1 and "cclap_errno" is set appropriately.
 * If cclap_errno is a parsing error (CCLAP_ERR_PARS), the logstr string
 * contains the appropriate error message (logstrlen char max).
 */
int cclap_parse(optionlist_t* options, int argc, char** argv, char* logstr, int logstrlen);

#ifdef __cplusplus
}
#endif

#endif // PARSER_H
