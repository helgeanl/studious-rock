#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"


list_t list_create()
{
	list_t new_list = (list_t)malloc(sizeof(struct list));
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->length = 0;
	return new_list;
}

void list_delete(list_t list)
{
	struct node *tmp_node;
	while(list->head != NULL ){
		tmp_node = list->head;
		list->head = list->head->next;
		free(tmp_node);
	}
	free(list);
}

void list_insert(list_t list, int index, int data)
{
	struct node *tmp_node;
	struct node *new_node = (struct node*)malloc(sizeof(struct node));
	new_node->data = data;
	if(list->head == NULL){
		list->head = new_node;
		list->tail = new_node;
	}else{
		if(index >= list->length){
			return;
		}
		tmp_node = list->head;
		for(int i=0;i<index-1;i++){
			tmp_node = tmp_node->next;
		}

		new_node->next = tmp_node->next;
		new_node->next->prev = new_node;
		new_node->prev = tmp_node;
		tmp_node->next = new_node;
	}

	list->length++;
	if(index == 0){
		list->head = new_node;
	}else if(index == list->length-1){
		list->tail = new_node;
	}
}

void list_append(list_t list, int data)
{
	struct node *new_node = (struct node*)malloc(sizeof(struct node));
	if(list->head == NULL){
		list->head = new_node;
		list->tail = new_node;
	}else{
		list->tail->next = new_node;
		new_node->prev = list->tail;
		list->tail = new_node;
	}
	new_node->data = data;
	list->length++;
}

void list_print(list_t list)
{
	struct node *tmp_node = list->head;
	for(int i=0; i < list->length;i++){
		printf("%i ",tmp_node->data );
		tmp_node = tmp_node->next;
	}
	printf("\n");
}

long list_sum(list_t list)
{
	long sum = 0;
	struct node *tmp_node = list->head;
	for(int i=0; i < list->length;i++){
		sum += tmp_node->data;
		tmp_node = tmp_node->next;
	}
	return sum;
}

int list_get(list_t list, int index)
{
	struct node *tmp_node = list->head;
	for(int i=0; i < index;i++){
		tmp_node = tmp_node->next;
	}
	return tmp_node->data;
}

int list_extract(list_t list, int index)
{
	struct node *tmp_node = list->head;
	int data;
	for(int i=0; i < index;i++){
		tmp_node = tmp_node->next;
	}
	data = tmp_node->data;
	if (list->length == 1){
		list->head = NULL;
		list->tail = NULL;
	}else if(index == 0 && list->length > 1){
		tmp_node->next->prev = NULL;
		list->head = tmp_node->next;
	}else if(index == list->length-1){
		tmp_node->prev->next = NULL;
		list->tail = tmp_node->prev;
	}else{
		tmp_node->prev->next = tmp_node->next;
		tmp_node->next->prev = tmp_node->prev;
	}

	free(tmp_node);
	list->length--;
	return data;
}
