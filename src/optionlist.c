#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cclap/errors.h"
#include "cclap/option.h"
#include "cclap/optionlist.h"

int cclap_optionlist_add(optionlist_t* list, option_t* opt) {
  optionlistnode_t* node;
  if (list == NULL || opt == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return 0;
  }
  // Check for duplicates
  CCLAP_OPTIONLIST_FOREACH(list, {
      if (cclap_option_equals(option, opt)) {
	cclap_errno = CCLAP_ERR_DUPOPT;
	return 0;
      }
    });
  if ((node = malloc(sizeof(optionlistnode_t))) == NULL ||
      (node->option = malloc(sizeof(option_t))) == NULL) {
    cclap_errno = CCLAP_ERR_SYSTEM;
    return 0;
  }
  memcpy(node->option, opt, sizeof(option_t));
  node->next = NULL;
  if (list->head == NULL) {
    // First element
    list->head = node;
    list->tail = node;
  }
  else {
    list->tail->next = node;
    list->tail = node;
  }
  list->size++;
  cclap_errno = CCLAP_SUCCESS;
  return 1;
}


void cclap_optionlist_destroy(optionlist_t** list) {
  optionlistnode_t* tmp;
  int i;
  if (list == NULL || *list == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return;
  }
  // Free the options
  for (i = 0; i < (*list)->size - 1; i++) {
    free((*list)->head->option);
    tmp = (*list)->head;
    (*list)->head = (*list)->head->next;
    free(tmp);
  }
  if ((*list)->head != NULL)
    free((*list)->head->option);
  free((*list)->head);
  // Free the optionlist
  free(*list);
  *list = NULL;
  cclap_errno = CCLAP_SUCCESS;
}


int cclap_optionlist_empty(optionlist_t* list) {
  if (list == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return -1;
  }
  cclap_errno = CCLAP_SUCCESS;
  return (list->size == 0) ? 1 : 0;
}


option_t* cclap_optionlist_get(optionlist_t* list, char sname, char* lname) {
  char* lname_tmp;
  if (list == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return NULL;
  }
  cclap_errno = CCLAP_SUCCESS;
  CCLAP_OPTIONLIST_FOREACH(list, {
      if (cclap_option_sname(option) == sname) {
	lname_tmp = cclap_option_lname(option);
	if ((lname == NULL && strlen(lname_tmp) == 0) ||
	    (lname != NULL && strcmp(lname_tmp, lname) == 0))
	  return option;
      }
    });
  return NULL;
}


optionlist_t* cclap_optionlist_new(void) {
  optionlist_t* list = malloc(sizeof(optionlist_t));
  if (list == NULL) {
    cclap_errno = CCLAP_ERR_SYSTEM;
    return NULL;
  }
  memset(list, 0, sizeof(optionlist_t));
  cclap_errno = CCLAP_SUCCESS;
  return list;
}


int cclap_optionlist_size(optionlist_t* list) {
  if (list == NULL) {
    cclap_errno = CCLAP_ERR_NULLVALUE;
    return -1;
  }
  cclap_errno = CCLAP_SUCCESS;
  return list->size;
}
