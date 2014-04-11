#ifndef OPTION_H
#define OPTION_H

#include "cclap/core.h"

typedef enum {
  SOPTION = 0, // Simple option (soption_t)
  OPTIONVAL // Option with value (optionval_t)
} optiontype_t;

/*
 * Simple option
 * Ex: "-v" and/or "--verbose"
 */
typedef struct {
  /*
   * Option's short name. For instance : 'v', detected in arguments as "-v".
   * Can be '\0' (= (char)0, it means this option has no short name), a letter
   * or a digit.
   */
  char sname;
  /*
   * Option's long name. For instance : "version", detected in arguments as
   * "--version". Can be empty, or a length > 1 character.
   */
  char lname[CCLAP_BUFSIZE];
  /*
   * A sentence to explain the option. Used to generate the help
   * message. (for example the option "--help"). Can be empty.
   */
  char helpmsg[CCLAP_BUFSIZE];
  /*
   * Indicates if the option was set or not. The default value is 0, then
   * the parser set it to 1 if the option is detected in the arguments.
   */
  int isset;
} soption_t;

/*
 * Option with value
 * Ex: "-s 10" and/or "--size=10"
 */
typedef struct {
  /*
   * Simple option
   */
  soption_t opt;
  /*
   * Option's value name. Is used when the help message is generated. For
   * example : if the valuename attribute is set to "SIZE", the help message
   * will generate this kind of msg : -o SIZE, --optionName=SIZE
   */
  char valuename[CCLAP_BUFSIZE];
  /*
   * Option's value. For exemple : if the argument is "--size=10", the value
   * of the "size" option is 10
   */
  char value[CCLAP_BUFSIZE];
} optionval_t;


/*
 * Generic option
 */
typedef struct {
  optiontype_t type;
  union {
    soption_t opt;
    optionval_t optv;
  };
} option_t;


/*
 * Compares 2 options (short and long name only).
 * Returns a non-zero integer if the options are equals, 0 otherwise.
 */
int cclap_option_equals(option_t* opt1, option_t* opt2);

/*
 * Returns the help msg.
 */
char* cclap_option_helpmsg(option_t* option);

/*
 * Returns 1 if the option is set, 0 otherwise.
 * This value is changed by the parser.
 */
int cclap_option_isset(option_t* option);

/*
 * Returns the long name.
 */
char* cclap_option_lname(option_t* option);

/*
 * Returns the short name.
 */
char cclap_option_sname(option_t* option);

/*
 * If the option is an option with value, returns the value (can be empty).
 * Otherwise, returns NULL.
 */
char* cclap_option_value(option_t* option);

/*
 * If the option is an option with value, returns the value name (can be empty).
 * Otherwise, returns NULL.
 */
char* cclap_option_valuename(option_t* option);

/*
 * Creates an new option with value.
 * On error, returns NULL and "cclap_errno" is set appropriately.
 */
option_t* cclap_optionval_new(char sname, char* lname, char* helpmsg, char* valuename);

/*
 * Creates an new simple option.
 * On error, returns NULL and "cclap_errno" is set appropriately.
 */
option_t* cclap_soption_new(char sname, char* lname, char* helpmsg);

#endif // OPTION_H
