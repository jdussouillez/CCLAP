#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cclap/core.h"
#include "cclap/errors.h"
#include "cclap/option.h"

static int valid_lname(char* lname) {
  int i, len;
  if (lname == NULL)
    return 1;
  if ((len = strlen(lname)) < 2)
    return 0;
  for (i = 0; i < len; i++) {
    if (!isalnum(lname[i]))
      return 0;
  }
  return 1;
}


static int check_args(char sname, char* lname) {
  if (sname != '\0' && !isalnum(sname)) {
    cclap_errno = CCLAP_ERR_BADSNAME;
    return 0;
  }
  if (!valid_lname(lname)) {
    cclap_errno = CCLAP_ERR_BADLNAME;
    return 0;
  }
  if (sname == '\0' && lname == NULL) {
    cclap_errno = CCLAP_ERR_NONAMES;
    return 0;
  }
  return 1;
}


int cclap_option_equals(option_t* opt1, option_t* opt2) {
  if (opt1 == NULL || opt2 == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return 0;
  }
  cclap_errno = CCLAP_SUCCESS;
  if (opt1->type != opt2->type)
    return 0;
  if (cclap_option_sname(opt1) != cclap_option_sname(opt2))
    return 0;
  return strncmp(cclap_option_lname(opt1), cclap_option_lname(opt2), CCLAP_BUFSIZE) == 0;
}


char* cclap_option_helpmsg(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? option->opt.helpmsg : option->optv.opt.helpmsg;
}


int cclap_option_isset(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return 0;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? option->opt.isset : option->optv.opt.isset;
}


char* cclap_option_lname(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? option->opt.lname : option->optv.opt.lname;
}


char cclap_option_sname(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return 0;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? option->opt.sname : option->optv.opt.sname;
}


char* cclap_option_value(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? NULL : option->optv.value;
}


char* cclap_option_valuename(option_t* option) {
  if (option == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (option->type == SOPTION) ? NULL : option->optv.valuename;
}


option_t* cclap_optionval_new(char sname, char* lname, char* helpmsg, char* valuename) {
  option_t* option;
  if (!check_args(sname, lname))
    return NULL;
  if ((option = malloc(sizeof(option_t))) == NULL) {
    cclap_errno = CCLAP_ERR_SYSTEM;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  memset(option, 0, sizeof(option_t));
  option->type = OPTIONVAL;
  option->optv.opt.sname = sname;
  if (lname != NULL)
    strncpy(option->optv.opt.lname, lname, CCLAP_BUFSIZE);
  if (helpmsg != NULL)
    strncpy(option->optv.opt.helpmsg, helpmsg, CCLAP_BUFSIZE);
  if (valuename != NULL)
    strncpy(option->optv.valuename, valuename, CCLAP_BUFSIZE);
  option->optv.opt.isset = 0;
  return option;
}


option_t* cclap_soption_new(char sname, char* lname, char* helpmsg) {
  option_t* option;
  if (!check_args(sname, lname))
    return NULL;
  if ((option = malloc(sizeof(option_t))) == NULL) {
    cclap_errno = CCLAP_ERR_SYSTEM;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  memset(option, 0, sizeof(option_t));
  option->type = SOPTION;
  option->opt.sname = sname;
  if (lname != NULL)
    strncpy(option->opt.lname, lname, CCLAP_BUFSIZE);
  if (helpmsg != NULL)
    strncpy(option->opt.helpmsg, helpmsg, CCLAP_BUFSIZE);
  option->opt.isset = 0;
  return option;
}

