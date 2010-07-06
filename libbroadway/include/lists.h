/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __LISTS_H__
#define __LISTS_H__

/* All your linked lists _must_ look like this at the top. */
typedef struct linked_node {
	struct linked_node*	next;
	struct linked_node*	prev;
} linked_node;

#define linked_walk(list, condition) do { \
	if((list) == NULL) { walk = NULL; break; } \
	for(walk = (list); (walk->next != NULL) && (condition); walk = walk->next); \
	if(!(condition)) walk = NULL; /* Our condition never happened, so we have no result */ \
} while(0)

/* l is the list to be added to (a void pointer to clear up GCC warnings) [actually a linked_node**]
 * n is the object to add on (a void pointer to clear up GCC warnings) [actually a linked_node*]
 * node is the object to reference to in the addition (a void pointer to clear up GCC warnings)
 */
linked_node* linked_make(size_t size);
void* linked_add_first( void* l, void *n);
void* linked_add_last(  void* l, void *n);
void* linked_add_before(void* l, void *n, void* node);
void* linked_add_after( void* l, void *n, void* node);
void  linked_del(       void* l,          void* node);

#endif
