#pragma once

typedef struct List_ns List_ns;
typedef struct ListNode ListNode;
typedef struct List List;



List_ns list_ns();
void list_create(List ** list);
void list_free(List **list);
void list_rem(List *list , void * data);
void list_add(List *list , void * data );



typedef struct List_ns {
	void (*create)(List ** list);
	void (*free)(List **list);
	void (*rem)(List *list , void * data);
	void (*ins)(List *list , void * data );


}List_ns;

typedef struct ListNode {
    struct ListNode * next;
	void *  data;
} ListNode;

typedef struct List {
    ListNode* head;       
} List;



#ifdef LIST_IMPLEMENTATION
 
 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 

List_ns list_ns(){
	static List_ns ret;
	ret.free = list_free;
	ret.ins = list_add;
	ret.rem = list_rem ;
	ret.create = list_create;
	return ret;
}


ListNode* createListNode(void * data );

ListNode* createListNode(void * data) {
    ListNode* newListNode = malloc(sizeof(ListNode));
	assert(newListNode);
 
    newListNode->data = data;

    newListNode->next = nullptr;
    return newListNode;
}

void list_create(List ** list){
	(*list) = calloc(1,sizeof(List));
 
}
// Function to insert a ListNode at the beginning of the list
void list_add(List list[static 1], void * data) {
	assert(list);  

    ListNode* newListNode = createListNode(data);
	newListNode->next =  list->head;
    list->head = newListNode;
 
 
}

 

// Function to delete a ListNode by value
void list_rem(List list[static 1], void * data) {
	assert(list);  // Ensure list is not NULL
    ListNode* temp = list->head;
    ListNode* prev = nullptr;

    // If the ListNode to be deleted is the head
    if (temp != nullptr && temp->data == data) {
        list->head = temp->next;
        free(temp);
	 
        return;
    }

    // Search for the ListNode to be deleted
    while (temp != nullptr && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    // If the ListNode was not found
    if (temp == nullptr) {
        printf("ListNode with data %08x not found!\n", data);
        return;
    }

    // Unlink the ListNode from the list
    prev->next = temp->next;
    free(temp);
 
}

 
// Function to free the entire list
void list_free(List * list[static 1]) {
    ListNode* temp = (*list)->head;
    while (temp != nullptr) {
        ListNode* next = temp->next;
	 
        free(temp);
        temp = next;
    }
	free((*list));
    (*list) = nullptr;
 
}

#endif