#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cclap/core.h"
#include "cclap/errors.h"
#include "cclap/option.h"
#include "cclap/optionlist.h"

static void _option_set(option_t* option, int set) {
  if (option->type == SOPTION)
    option->opt.isset = set;
  else
    option->optv.opt.isset = set;
}


static void _option_setvalue(option_t* option, char* value) {
  if (option->type == OPTIONVAL)
    strncpy(option->optv.value, value, CCLAP_BUFSIZE);
}


int cclap_parse(optionlist_t* options, int argc, char** argv, char* logstr, ssize_t logstrlen) {
  int i, j, last_opt_index = -1, invalid_arg_detected = 0, tmplen;
  option_t* opt;
  char shortname, *longname, *value, *tmp;
  if (options == NULL || argv == NULL || logstr == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return -2;
  }
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strlen(argv[i]) == 1) {
	snprintf(logstr, logstrlen, "invalid option -- '-'");
	cclap_errno = CCLAP_ERR_PARS;
	return -1;
      }
      if (invalid_arg_detected) {
	snprintf(logstr, logstrlen, "unexpected option '%s'", argv[i]);
	cclap_errno = CCLAP_ERR_PARS;
	return -1;
      }
      if (argv[i][1] != '-') {
	/*
	 * Short name options
	 */
	for (j = 1; j < (int)strlen(argv[i]) - 1; j++) {
	  // In this loop, the options are in the same "block"
	  // For instance : "-abc", here we analyse "a" and "b"
	  // These 2 options can't have an argument, only the last option can (next argument)
	  shortname = argv[i][j];
	  opt = cclap_optionlist_getbysname(options, shortname);
	  if (opt == NULL) {
	    snprintf(logstr, logstrlen, "invalid option -- '%c'", shortname);
	    cclap_errno = CCLAP_ERR_PARS;
	    return -1;
	  }
	  if (opt->type == OPTIONVAL) {
	    snprintf(logstr, logstrlen, "option '%c' requires an argument", shortname);
	    cclap_errno = CCLAP_ERR_PARS;
	    return -1;
	  }
	  _option_set(opt, 1);
	}
	// Last option (ex : 's' in "-s" or 'c' in "-abc")
	// This option can have an argument
	shortname = argv[i][strlen(argv[i]) - 1];
	opt = cclap_optionlist_getbysname(options, shortname);
	if (opt == NULL) {
	  snprintf(logstr, logstrlen, "invalid option -- '%c'", shortname);
	  cclap_errno = CCLAP_ERR_PARS;
	  return -1;
	}
	if (opt->type == OPTIONVAL) {
	  if (i >= argc) {
	    snprintf(logstr, logstrlen, "option '%c' requires an argument", shortname);
	    cclap_errno = CCLAP_ERR_PARS;
	    return -1;
	  }
	  _option_setvalue(opt, argv[++i]);
	}
	_option_set(opt, 1);
      }
      else {
	/*
	 * Long name option
	 */
	tmplen = strlen(argv[i]) + 1;
	if ((tmp = malloc(tmplen)) == NULL) {
	  cclap_errno = CCLAP_ERR_SYSTEM;
	  return -1;
	}
	memset(tmp, 0, tmplen);
	strncpy(tmp, argv[i], tmplen);
        longname = strtok(tmp, "=") + 2;
	value = strtok(NULL, "=");
        opt = cclap_optionlist_getbylname(options, longname);
	if (opt == NULL) {
	  snprintf(logstr, logstrlen, "invalid option -- '%s'", longname);
	  free(tmp);
	  cclap_errno = CCLAP_ERR_PARS;
	  return -1;
	}
	if (opt->type == OPTIONVAL) {
	  if (value == NULL) {
	    // The option expected an argument, but it was not specified
	    snprintf(logstr, logstrlen, "option '%s' requires an argument", longname);
	    free(tmp);
	    cclap_errno = CCLAP_ERR_PARS;
	    return -1;
	  }
	  _option_setvalue(opt, value);
	}
	else if (value != NULL) {
	  // The option does not expect an argument, bu there is one
	  snprintf(logstr, logstrlen, "option '%s' does not require an argument", longname);
	  free(tmp);
	  cclap_errno = CCLAP_ERR_PARS;
	  return -1;
	}
	_option_set(opt, 1);
	free(tmp);
      }
      last_opt_index = i;
    }
    else {
      // This arg is not an option
      invalid_arg_detected = 1;
    }
  }
  return last_opt_index + 1; // First non-option argument
}

