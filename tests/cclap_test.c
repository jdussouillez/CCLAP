#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cclap/errors.h"
#include "cclap/option.h"

#include "CUnit/Basic.h"

#define DEBUG !0

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
  // optionval, only sortname -> true
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

