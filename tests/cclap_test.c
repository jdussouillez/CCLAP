#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cclap/errors.h"
#include "cclap/option.h"
#include "cclap/optionlist.h"
#include "cclap/parser.h"

#include "CUnit/Basic.h"

#define DEBUG 0

#define FREE(p) do { free(p); p = NULL; } while (0)

/*
 * CUnit Test Suite
 */
int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

static optionlist_t* _create(void) {
  optionlist_t* optionlist;
  option_t* option;
  optionlist = cclap_optionlist_new();

  // Simple options
  option = cclap_soption_new('v', "verbose", "expain what is being done");
  cclap_optionlist_add(optionlist, option);
  FREE(option);

  option = cclap_soption_new(0, "version", "output version and exit");
  cclap_optionlist_add(optionlist, option);
  FREE(option);

  option = cclap_soption_new('n', NULL, "numer all output lines");
  cclap_optionlist_add(optionlist, option);
  FREE(option);

  // Options with value
  option = cclap_optionval_new('c', "color", "colorize the output", "COLOR_NAME");
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  return optionlist;
}

static void _reset(optionlist_t* optionlist) {
  CCLAP_OPTIONLIST_FOREACH(optionlist, {
      if (option->type == SOPTION) {
	option->opt.isset = 0;
      }
      else {
	option->optv.opt.isset = 0;
	memset(option->optv.value, 0, sizeof(option->optv.value));
      }
    });
}

/*
 ***************************************************
 ******************* Option suite ******************
 ***************************************************
 */
/*
 ********************************
 * Test-to-pass
 ********************************
 */
void option_equals_TTP() {
  option_t *opt1, *opt2;
  // Different types -> false
  opt1 = cclap_soption_new('s', NULL, NULL);
  opt2 = cclap_optionval_new('s', NULL, NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, only shortname -> false
  opt1 = cclap_soption_new('s', NULL, NULL);
  opt2 = cclap_soption_new('d', NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, only sortname -> true
  opt1 = cclap_soption_new('s', NULL, NULL);
  opt2 = cclap_soption_new('s', NULL, NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, only longname -> false
  opt1 = cclap_soption_new(0, "verbose", NULL);
  opt2 = cclap_soption_new(0, "ABCDEF", NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, only longname -> true
  opt1 = cclap_soption_new(0, "verbose", NULL);
  opt2 = cclap_soption_new(0, "verbose", NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, both short and long name -> false
  opt1 = cclap_soption_new('v', "verbose", NULL);
  opt2 = cclap_soption_new('v', "version", NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  opt1 = cclap_soption_new('v', "verbose", NULL);
  opt2 = cclap_soption_new('i', "verbose", NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // soption, both short and long name -> true
  opt1 = cclap_soption_new('v', "verbose", NULL);
  opt2 = cclap_soption_new('v', "verbose", NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, only shortname -> false
  opt1 = cclap_optionval_new('f', NULL, NULL, NULL);
  opt2 = cclap_optionval_new('s', NULL, NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, only shortname -> true
  opt1 = cclap_optionval_new('f', NULL, NULL, NULL);
  opt2 = cclap_optionval_new('f', NULL, NULL, NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, only longname -> false
  opt1 = cclap_optionval_new(0, "file", NULL, NULL);
  opt2 = cclap_optionval_new(0, "size", NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, only longname -> true
  opt1 = cclap_optionval_new(0, "file", NULL, NULL);
  opt2 = cclap_optionval_new(0, "file", NULL, NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, both short and long name -> false
  opt1 = cclap_optionval_new('f', "file", NULL, NULL);
  opt2 = cclap_optionval_new('f', "folder", NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  opt1 = cclap_optionval_new('f', "folder", NULL, NULL);
  opt2 = cclap_optionval_new('F', "folder", NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
  // optionval, both short and long name -> true
  opt1 = cclap_optionval_new('f', "file", NULL, NULL);
  opt2 = cclap_optionval_new('f', "file", NULL, NULL);
  CU_ASSERT_TRUE(cclap_option_equals(opt1, opt2));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(opt1);
  FREE(opt2);
}

void option_helpmsg_TTP() {
  option_t* option;
  char* helpmsg;
  // optionval, no help msg
  option = cclap_optionval_new('f', NULL, NULL, NULL);
  helpmsg = cclap_option_helpmsg(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(helpmsg);
  CU_ASSERT_EQUAL(strcmp(helpmsg, ""), 0);
  FREE(option);
  // optionval, with help msg
  option = cclap_optionval_new('f', NULL, "Input file", NULL);
  helpmsg = cclap_option_helpmsg(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(helpmsg);
  CU_ASSERT_EQUAL(strcmp(helpmsg, "Input file"), 0);
  FREE(option);
  // soption, no help msg
  option = cclap_soption_new('v', NULL, NULL);
  helpmsg = cclap_option_helpmsg(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(helpmsg);
  CU_ASSERT_EQUAL(strcmp(helpmsg, ""), 0);
  FREE(option);
  // soption, with help msg
  option = cclap_soption_new('v', NULL, "Explain what is being done");
  helpmsg = cclap_option_helpmsg(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(helpmsg);
  CU_ASSERT_EQUAL(strcmp(helpmsg, "Explain what is being done"), 0);
  FREE(option);
}

void option_isset_TTP() {
  option_t* option;
  // optionval, default value (0)
  option = cclap_optionval_new('f', NULL, NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_isset(option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  // optionval, modified value (1)
  option->optv.opt.isset = 1;
  CU_ASSERT_TRUE(cclap_option_isset(option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(option);
  // soption, default value (0)
  option = cclap_soption_new('v', NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_isset(option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  // soption, modified value (1)
  option->opt.isset = 1;
  CU_ASSERT_TRUE(cclap_option_isset(option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  FREE(option);
}

void option_lname_TTP() {
  option_t* option;
  char* lname;
  // optionval, no long name
  option = cclap_optionval_new('s', NULL, NULL, NULL);
  lname = cclap_option_lname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(lname);
  CU_ASSERT_EQUAL(strcmp(lname, ""), 0);
  FREE(option);
  // optionval, with long name
  option = cclap_optionval_new(0, "size", NULL, NULL);
  lname = cclap_option_lname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(lname);
  CU_ASSERT_EQUAL(strcmp(lname, "size"), 0);
  FREE(option);
  // soption, no long name
  option = cclap_soption_new('v', NULL, NULL);
  lname = cclap_option_lname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(lname);
  CU_ASSERT_EQUAL(strcmp(lname, ""), 0);
  FREE(option);
  // soption, with long name
  option = cclap_soption_new(0, "verbose", NULL);
  lname = cclap_option_lname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(lname);
  CU_ASSERT_EQUAL(strcmp(lname, "verbose"), 0);
  FREE(option);
}

void option_sname_TTP() {
  option_t* option;
  char sname;
  // optionval, no short name
  option = cclap_optionval_new(0, "size", NULL, NULL);
  sname = cclap_option_sname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(sname, 0);
  FREE(option);
  // optionval, with short name
  option = cclap_optionval_new('s', NULL, NULL, NULL);
  sname = cclap_option_sname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(sname, 's');
  FREE(option);
  // soption, no short name
  option = cclap_soption_new(0, "verbose", NULL);
  sname = cclap_option_sname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(sname, 0);
  FREE(option);
  // soption, with short name
  option = cclap_soption_new('v', NULL, NULL);
  sname = cclap_option_sname(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(sname, 'v');
  FREE(option);
}

void option_value_TTP() {
  option_t* option;
  char* value;
  // optionval
  option = cclap_optionval_new('s', NULL, NULL, NULL);
  value = cclap_option_value(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(value);
  CU_ASSERT_EQUAL(strcmp(value, ""), 0);
  FREE(option);
  // optionval (value modified)
  option = cclap_optionval_new('s', NULL, NULL, NULL);
  strncpy(option->optv.value, "VALUE", CCLAP_BUFSIZE);
  value = cclap_option_value(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(value);
  CU_ASSERT_EQUAL(strcmp(value, "VALUE"), 0);
  FREE(option);
  // soption
  option = cclap_soption_new('s', NULL, NULL);
  value = cclap_option_value(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NULL(value);
  FREE(option);
}

void option_valuename_TTP() {
  option_t* option;
  char* valuename;
  // optionval: NULL valuename -> NULL
  option = cclap_optionval_new('s', NULL, NULL, NULL);
  valuename = cclap_option_valuename(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(valuename);
  CU_ASSERT_EQUAL(strcmp(valuename, ""), 0);
  FREE(option);
  // optionval: non-NULL valuename -> valuename
  option = cclap_optionval_new('f', NULL, NULL, "Input file");
  valuename = cclap_option_valuename(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NOT_NULL(valuename);
  CU_ASSERT_EQUAL(strcmp(valuename, "Input file"), 0);
  FREE(option);
  // Invalid option -> NULL
  option = cclap_soption_new('v', NULL, NULL);
  valuename = cclap_option_valuename(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_PTR_NULL(valuename);
  FREE(option);
}

void optionval_new_TTP() {
  option_t* option;
  // Only shortname
  option = cclap_optionval_new('f', NULL, NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, OPTIONVAL);
  CU_ASSERT_EQUAL(option->optv.opt.sname, 'f');
  CU_ASSERT_EQUAL(strlen(option->optv.opt.lname), 0);
  CU_ASSERT_EQUAL(strlen(option->optv.opt.helpmsg), 0);
  CU_ASSERT_EQUAL(option->optv.opt.isset, 0);
  CU_ASSERT_EQUAL(strlen(option->optv.valuename), 0);
  CU_ASSERT_EQUAL(strlen(option->optv.value), 0);
  FREE(option);
  // Longname
  option = cclap_optionval_new(0, "file", NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, OPTIONVAL);
  CU_ASSERT_EQUAL(option->optv.opt.sname, 0);
  CU_ASSERT_EQUAL(strcmp(option->optv.opt.lname, "file"), 0);
  CU_ASSERT_EQUAL(strlen(option->optv.opt.helpmsg), 0);
  CU_ASSERT_EQUAL(option->optv.opt.isset, 0);
  CU_ASSERT_EQUAL(strlen(option->optv.valuename), 0);
  CU_ASSERT_EQUAL(strlen(option->optv.value), 0);
  FREE(option);
  // Shortname, longname, helpmsg and valuename
  option = cclap_optionval_new('f', "file", "Input filename", "FILENAME");
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, OPTIONVAL);
  CU_ASSERT_EQUAL(option->optv.opt.sname, 'f');
  CU_ASSERT_EQUAL(strcmp(option->optv.opt.lname, "file"), 0);
  CU_ASSERT_EQUAL(strcmp(option->optv.opt.helpmsg, "Input filename"), 0);
  CU_ASSERT_EQUAL(option->optv.opt.isset, 0);
  CU_ASSERT_EQUAL(strcmp(option->optv.valuename, "FILENAME"), 0);
  CU_ASSERT_EQUAL(strlen(option->optv.value), 0);
  FREE(option);
}

void soption_new_TTP() {
  option_t* option;
  // Only shortname
  option = cclap_soption_new('v', NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, SOPTION);
  CU_ASSERT_EQUAL(option->opt.sname, 'v');
  CU_ASSERT_EQUAL(strlen(option->opt.lname), 0);
  CU_ASSERT_EQUAL(strlen(option->opt.helpmsg), 0);
  CU_ASSERT_EQUAL(option->opt.isset, 0);
  FREE(option);
  // Only longname
  option = cclap_soption_new(0, "verbose", NULL);
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, SOPTION);
  CU_ASSERT_EQUAL(option->opt.sname, 0);
  CU_ASSERT_EQUAL(strcmp(option->opt.lname, "verbose"), 0);
  CU_ASSERT_EQUAL(strlen(option->opt.helpmsg), 0);
  CU_ASSERT_EQUAL(option->opt.isset, 0);
  FREE(option);
  // Shortname, longname and helpmsg
  option = cclap_soption_new('v', "verbose", "output version information and exit");
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(option->type, SOPTION);
  CU_ASSERT_EQUAL(option->opt.sname, 'v');
  CU_ASSERT_EQUAL(strcmp(option->opt.lname, "verbose"), 0);
  CU_ASSERT_EQUAL(strcmp(option->opt.helpmsg, "output version information and exit"), 0);
  CU_ASSERT_EQUAL(option->opt.isset, 0);
  FREE(option);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void option_equals_TTF() {
  option_t* option = cclap_soption_new('v', NULL, NULL);
  CU_ASSERT_FALSE(cclap_option_equals(option, NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  CU_ASSERT_FALSE(cclap_option_equals(NULL, option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  FREE(option);
}

void option_helpmsg_TTF() {
  CU_ASSERT_PTR_NULL(cclap_option_helpmsg(NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void option_isset_TTF() {
  CU_ASSERT_FALSE(cclap_option_isset(NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void option_lname_TTF() {
  CU_ASSERT_PTR_NULL(cclap_option_lname(NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void option_sname_TTF() {
  CU_ASSERT_EQUAL(cclap_option_sname(NULL), 0);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void option_value_TTF() {
  CU_ASSERT_PTR_NULL(cclap_option_value(NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void option_valuename_TTF() {
  CU_ASSERT_PTR_NULL(cclap_option_valuename(NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void optionval_new_TTF() {
  option_t* option;
  // Bad shortname
  option = cclap_optionval_new('_', NULL, NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADSNAME);
  // Bad longname
  option = cclap_optionval_new('\0', "A", NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADLNAME);
  option = cclap_optionval_new('\0', "long-name", NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADLNAME);
  // No names
  option = cclap_optionval_new('\0', NULL, NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NONAMES);
}

void soption_new_TTF() {
  option_t* option;
  // Bad shortname
  option = cclap_soption_new('_', NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADSNAME);
  // Bad longname
  option = cclap_soption_new('\0', "A", NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADLNAME);
  option = cclap_soption_new('\0', "long-name", NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADLNAME);
  // No names
  option = cclap_soption_new('\0', NULL, NULL);
  CU_ASSERT_PTR_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NONAMES);
}

/*
 ***************************************************
 ***************** OptionList suite ****************
 ***************************************************
 */
/*
 ********************************
 * Test-to-pass
 ********************************
 */
void optionlist_add_TTP() {
  option_t* option;
  optionlist_t* optionlist;
  optionlist = cclap_optionlist_new();
  // Valid soption, on an empty list
  option = cclap_soption_new('v', NULL, NULL);
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  CU_ASSERT_EQUAL(optionlist->size, 1);
  CU_ASSERT_EQUAL(optionlist->head->option->opt.sname, 'v');
  CU_ASSERT_EQUAL(strlen(optionlist->head->option->opt.lname), 0);
  CU_ASSERT_EQUAL(strlen(optionlist->head->option->opt.helpmsg), 0);
  CU_ASSERT_FALSE(optionlist->head->option->opt.isset);
  // Valid optionval, on a non-empty list
  option = cclap_optionval_new('s', "size", NULL, NULL);
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  CU_ASSERT_EQUAL(optionlist->size, 2);
  CU_ASSERT_EQUAL(optionlist->head->next->option->optv.opt.sname, 's');
  CU_ASSERT_EQUAL(strcmp(optionlist->head->next->option->optv.opt.lname, "size"), 0);
  CU_ASSERT_EQUAL(strlen(optionlist->head->next->option->optv.opt.helpmsg), 0);
  CU_ASSERT_FALSE(optionlist->head->next->option->optv.opt.isset);
  CU_ASSERT_EQUAL(strlen(optionlist->head->next->option->optv.valuename), 0);
  CU_ASSERT_EQUAL(strlen(optionlist->head->next->option->optv.value), 0);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_destroy_TTP() {
  optionlist_t* optionlist;
  optionlist = cclap_optionlist_new();
  CU_ASSERT_PTR_NOT_NULL(optionlist);
  cclap_optionlist_destroy(&optionlist);
  CU_ASSERT_PTR_NULL(optionlist);
}

void optionlist_empty_TTP() {
  option_t* option;
  optionlist_t* optionlist;
  optionlist = cclap_optionlist_new();
  // On an empty list
  CU_ASSERT_TRUE(cclap_optionlist_empty(optionlist));
  // On a list of 2 options
  option = cclap_soption_new('v', NULL, NULL);
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  CU_ASSERT_FALSE(cclap_optionlist_empty(optionlist));
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_getbylname_TTP() {
  option_t* option;
  optionlist_t* optionlist;
  option = cclap_soption_new(0, "verbose", NULL);
  optionlist = cclap_optionlist_new();
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  option = cclap_optionlist_getbylname(optionlist, "verbose");
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(strcmp(cclap_option_lname(option), "verbose"), 0);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_getbysname_TTP() {
  option_t* option;
  optionlist_t* optionlist;
  option = cclap_soption_new('v', NULL, NULL);
  optionlist = cclap_optionlist_new();
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  option = cclap_optionlist_getbysname(optionlist, 'v');
  CU_ASSERT_PTR_NOT_NULL(option);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  CU_ASSERT_EQUAL(cclap_option_sname(option), 'v');
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_new_TTP() {
  optionlist_t* optionlist;
  optionlist = cclap_optionlist_new();
  CU_ASSERT_PTR_NOT_NULL(optionlist);
  CU_ASSERT_EQUAL(optionlist->size, 0);
  CU_ASSERT_PTR_NULL(optionlist->head);
  CU_ASSERT_PTR_NULL(optionlist->tail);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_size_TTP() {
  option_t* option;
  optionlist_t* optionlist;
  optionlist = cclap_optionlist_new();
  // On an empty list
  CU_ASSERT_EQUAL(cclap_optionlist_size(optionlist), 0);
  // On a list of 2 options
  option = cclap_soption_new('v', NULL, NULL);
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  option = cclap_soption_new('s', NULL, NULL);
  cclap_optionlist_add(optionlist, option);
  FREE(option);
  CU_ASSERT_EQUAL(cclap_optionlist_size(optionlist), 2);
  cclap_optionlist_destroy(&optionlist);
}

/*
 ********************************
 * Test-to-fail
 ********************************
 */
void optionlist_add_TTF() {
  optionlist_t* optionlist;
  option_t* option;
  // NULL optionlist
  option = cclap_soption_new('v', NULL, NULL);
  CU_ASSERT_FALSE(cclap_optionlist_add(NULL, option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  FREE(option);
  // NULL option
  optionlist = cclap_optionlist_new();
  CU_ASSERT_FALSE(cclap_optionlist_add(optionlist, NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  // Try to add a duplicate option
  option = cclap_soption_new('v', NULL, NULL);
  CU_ASSERT_TRUE(cclap_optionlist_add(optionlist, option));
  CU_ASSERT_EQUAL(optionlist->size, 1);
  CU_ASSERT_FALSE(cclap_optionlist_add(optionlist, option));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_DUPOPT);
  CU_ASSERT_EQUAL(optionlist->size, 1);
  FREE(option);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_destroy_TTF() {
  optionlist_t* optionlist = NULL;
  // NULL optionlist
  cclap_optionlist_destroy(&optionlist);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  cclap_optionlist_destroy(NULL);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void optionlist_empty_TTF() {
  CU_ASSERT_EQUAL(cclap_optionlist_empty(NULL), -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}

void optionlist_getbylname_TTF() {
  optionlist_t* optionlist;
  CU_ASSERT_PTR_NULL(cclap_optionlist_getbylname(NULL, "verbose"));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  optionlist = cclap_optionlist_new();
  CU_ASSERT_PTR_NULL(cclap_optionlist_getbylname(optionlist, NULL));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADLNAME);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_getbysname_TTF() {
  optionlist_t* optionlist;
  CU_ASSERT_PTR_NULL(cclap_optionlist_getbysname(NULL, 'v'));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
  optionlist = cclap_optionlist_new();
  CU_ASSERT_PTR_NULL(cclap_optionlist_getbysname(optionlist, 0));
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_BADSNAME);
  cclap_optionlist_destroy(&optionlist);
}

void optionlist_size_TTF() {
  CU_ASSERT_EQUAL(cclap_optionlist_size(NULL), -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);
}


/*
***********************************************
 ***************** Parser suite ****************
 ***********************************************
 */
/*
 ********************************
 * Test-to-pass
 ********************************
 */
void parser_TTP() {
  char* argv1[] = {"./myprog", "--verbose", "-nc", "RED", "file1", "file2"};
  int size = 256, firstnonoptionarg;
  char logstr[size];
  option_t* option;
  optionlist_t* optionlist = _create();

  firstnonoptionarg = cclap_parse(optionlist, 5, argv1, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, 4);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_SUCCESS);
  option = cclap_optionlist_getbylname(optionlist, "verbose");
  CU_ASSERT_TRUE(cclap_option_isset(option));
  option = cclap_optionlist_getbysname(optionlist, 'n');
  CU_ASSERT_TRUE(cclap_option_isset(option));
  option = cclap_optionlist_getbysname(optionlist, 'c');
  CU_ASSERT_TRUE(cclap_option_isset(option));
  CU_ASSERT_EQUAL(strcmp(cclap_option_value(option), "RED"), 0);

  cclap_optionlist_destroy(&optionlist);
}

/*
 ********************************
 * Test-to-pass
 ********************************
 */
void parser_TTF() {
  char* argv1[] = {"./myprog", "-", "file1", "file2"};
  char* argv2[] = {"./myprog", "--verbose", "--test"};
  char* argv3[] = {"./myprog", "--color"};
  char* argv4[] = {"./myprog", "--verbose=true"};
  int size = 256, firstnonoptionarg;
  char logstr[size];
  option_t* option;
  optionlist_t* optionlist = _create();

  // NULL optionlist
  firstnonoptionarg = cclap_parse(NULL, 0, argv1, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -2);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);

  // NULL parameters
  firstnonoptionarg = cclap_parse(optionlist, 0, NULL, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -2);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);

  // NULL log string
  firstnonoptionarg = cclap_parse(optionlist, 0, argv1, NULL, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -2);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_NULLVALUE);

  // Invalid token "-"
  firstnonoptionarg = cclap_parse(optionlist, 4, argv1, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_PARS);
  CU_ASSERT_EQUAL(strcmp(logstr, "invalid option -- '-'"), 0);

  // Invalid long option
  firstnonoptionarg = cclap_parse(optionlist, 3, argv2, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_PARS);
  option = cclap_optionlist_getbylname(optionlist, "verbose");
  CU_ASSERT_TRUE(cclap_option_isset(option));
  _reset(optionlist);
  CU_ASSERT_EQUAL(strcmp(logstr, "invalid option -- 'test'"), 0);

  // Long option, required argument not found
  firstnonoptionarg = cclap_parse(optionlist, 2, argv3, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_PARS);
  option = cclap_optionlist_getbylname(optionlist, "color");
  CU_ASSERT_EQUAL(strcmp(logstr, "option 'color' requires an argument"), 0);
  
  // Long option, unexpected argument
  firstnonoptionarg = cclap_parse(optionlist, 2, argv4, logstr, size);
  CU_ASSERT_EQUAL(firstnonoptionarg, -1);
  CU_ASSERT_EQUAL(cclap_errno, CCLAP_ERR_PARS);
  option = cclap_optionlist_getbylname(optionlist, "verbose");
  CU_ASSERT_EQUAL(strcmp(logstr, "option 'verbose' does not require an argument"), 0);

  cclap_optionlist_destroy(&optionlist);
}


/*
 ********************************************************
 ************************* Main *************************
 ********************************************************
 */
int main(void) {
  CU_pSuite pSuite = NULL;
  int num_failures;

  // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /*
   *********************************
   * Option suite
   *********************************
   */
  pSuite = CU_add_suite("Option", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "option_equals_TTP", option_equals_TTP) == NULL ||
      CU_add_test(pSuite, "option_equals_TTF", option_equals_TTF) == NULL ||
      CU_add_test(pSuite, "option_helpmsg_TTP", option_helpmsg_TTP) == NULL ||
      CU_add_test(pSuite, "option_helpmsg_TTF", option_helpmsg_TTF) == NULL ||
      CU_add_test(pSuite, "option_isset_TTP", option_isset_TTP) == NULL ||
      CU_add_test(pSuite, "option_isset_TTF", option_isset_TTF) == NULL ||
      CU_add_test(pSuite, "option_lname_TTP", option_lname_TTP) == NULL ||
      CU_add_test(pSuite, "option_lname_TTF", option_lname_TTF) == NULL ||
      CU_add_test(pSuite, "option_sname_TTP", option_sname_TTP) == NULL ||
      CU_add_test(pSuite, "option_sname_TTF", option_sname_TTF) == NULL ||
      CU_add_test(pSuite, "option_value_TTP", option_value_TTP) == NULL ||
      CU_add_test(pSuite, "option_value_TTF", option_value_TTF) == NULL ||
      CU_add_test(pSuite, "option_valuename_TTP", option_valuename_TTP) == NULL ||
      CU_add_test(pSuite, "optionval_new_TTP", optionval_new_TTP) == NULL ||
      CU_add_test(pSuite, "soption_new_TTP", soption_new_TTP) == NULL ||
      CU_add_test(pSuite, "optionval_new_TTF", optionval_new_TTF) == NULL ||
      CU_add_test(pSuite, "soption_new_TTF", soption_new_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /*
   *********************************
   * OptionList suite
   *********************************
   */
  pSuite = CU_add_suite("OptionList", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "optionlist_add_TTP", optionlist_add_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_add_TTF", optionlist_add_TTF) == NULL ||
      CU_add_test(pSuite, "optionlist_destroy_TTP", optionlist_destroy_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_destroy_TTF", optionlist_destroy_TTF) == NULL ||
      CU_add_test(pSuite, "optionlist_empty_TTP", optionlist_empty_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_empty_TTF", optionlist_empty_TTF) == NULL ||
      CU_add_test(pSuite, "optionlist_getbylname_TTP", optionlist_getbylname_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_getbylname_TTF", optionlist_getbylname_TTF) == NULL ||
      CU_add_test(pSuite, "optionlist_getbysname_TTP", optionlist_getbysname_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_getbysname_TTF", optionlist_getbysname_TTF) == NULL ||
      CU_add_test(pSuite, "optionlist_size_TTP", optionlist_size_TTP) == NULL ||
      CU_add_test(pSuite, "optionlist_size_TTF", optionlist_size_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /*
   *********************************
   * Parser suite
   *********************************
   */
  pSuite = CU_add_suite("Parser", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (CU_add_test(pSuite, "parser_TTP", parser_TTP) == NULL ||
      CU_add_test(pSuite, "parser_TTF", parser_TTF) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the CUnit Basic interface
  CU_basic_set_mode((DEBUG ? CU_BRM_VERBOSE : CU_BRM_NORMAL));
  CU_basic_run_tests();
  printf("\n\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n");
  num_failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  return num_failures;
}

