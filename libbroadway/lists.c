/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <types.h>
#include <malloc.h>
#include <lists.h>

linked_node* linked_make(size_t size)
{
	if(size < sizeof(linked_node))
		return NULL;
	return calloc(size, 1);
}

void* linked_add_last(void* l, void *n)
{
	linked_node** list = l;
	linked_node* node = n;
	linked_node* walk;
	if(node == NULL)
		return NULL;
	if(*list == NULL) {
		*list = node;
	}else{
		linked_walk(*list, 1);
		if(walk == NULL) {
			/* Uhm, there are no entries? */
			free(node);
			return NULL;
		}
		walk->next = node;
		node->prev = walk;
	}
	return node;
}

void* linked_add_first(void* l, void *n)
{
	linked_node** list = l;
	linked_node* node = n;
	if(node == NULL)
		return NULL;
	if(*list == NULL) {
		*list = node;
	}else{
		(*list)->prev = node;
		node->next = *list;
		*list = node;
	}
	return node;
}

void* linked_add_before(void* l, void *n, void* find)
{
	linked_node* list = l;
	linked_node* node = n;
	linked_node* walk;
	linked_walk(list, walk != find);
	if(walk == NULL) {
		/* No such entry. */
		return NULL;
	}
	if(node == NULL)
		return NULL;
	if(walk->prev)
		walk->prev->next = node;
	node->next = walk;
	node->prev = walk->prev;
	walk->prev = node;
	return node;
}

void* linked_add_after(void* l, void *n, void* find)
{
	linked_node* list = l;
	linked_node* node = n;
	linked_node* walk;
	linked_walk(list, walk != find);
	if(walk == NULL) {
		/* No such entry. */
		return NULL;
	}
	if(node == NULL)
		return NULL;
	if(walk->next)
		walk->next->prev = node;
	node->prev = walk;
	node->next = walk->next;
	walk->next = node;
	return node;
}

void linked_del(void* l, void* node)
{
	linked_node* list = l;
	linked_node* walk;
	linked_walk(list, walk != node);
	if(walk == NULL) {
		/* No such entry. Just silently fail. */
		return;
	}
	if(walk->next != NULL)
		walk->next->prev = walk->prev;
	if(walk->prev != NULL)
		walk->prev->next = walk->next;
	free(walk);
}

