#ifndef OPTIONLIST_H
#define OPTIONLIST_H

#include "cclap/core.h"
#include "cclap/option.h"

/*
 * Iterates over the option list "listp" and run the instructions "instrs".
 * The "option" variable contains the option of the current list node.
 */
#define CCLAP_OPTIONLIST_FOREACH(listp, instrs) do {	\
    option_t* option;					\
    optionlistnode_t* node = listp->head;		\
    while (node != NULL) {				\
      option = node->option;				\
      instrs;						\
      node = node->next;				\
    }							\
  } while (0)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_optionlistnode {
  option_t* option;
  struct s_optionlistnode* next;
} optionlistnode_t;

typedef struct {
  int size; // Size of the list
  optionlistnode_t* head; // Pointer to the head of the list
  optionlistnode_t* tail; // Pointer to the tail of the list
} optionlist_t;

/*
 * Adds the option to the list (memory copy).
 * On success, returns 1.
 * On error, 0 is returned and "cclap_errno" is set appropriately.
 */
int cclap_optionlist_add(optionlist_t* list, option_t* opt);

/*
 * Destroys the list.
 * Free all the elements and the list. The pointer "list" is set to NULL.
 * On error, "cclap_errno" is set appropriately.
 */
void cclap_optionlist_destroy(optionlist_t** list);

/*
 * Returns 1 if the list is empty, 0 otherwise.
 * Returns -1 if the list is NULL and "cclap_errno" is set appropriately.
 */
int cclap_optionlist_empty(optionlist_t* list);

/*
 * Returns a pointer to an option identified by its short and long name.
 * Returns NULL if the option is not found.
 * On error, returns NULL and "cclap_errno" is set appropriately.
 */
option_t* cclap_optionlist_get(optionlist_t* list, char sname, char* lname);

/*
 * Creates a new list.
 * On success, returns the list.
 * On error, NULL is returned and "cclap_errno" is set appropriately.
 * At the end of your program, you must call "cclap_optionlist_destroy"
 * to destroy the options and free the memory.
 */
optionlist_t* cclap_optionlist_new(void);

/*
 * Returns the size (number of elements) of the list.
 * Returns -1 if the list is NULL and "cclap_errno" is set appropriately.
 */
int cclap_optionlist_size(optionlist_t* list);

#ifdef __cplusplus
}
#endif

#endif // OPTIONLIST_H
