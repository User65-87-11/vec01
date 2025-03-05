#pragma once



typedef struct ListNode {
    struct ListNode * next;
	void*  data;
} ListNode;

typedef struct {
    ListNode* head;       

} List;



void list_create(List * list[static 1]);
void list_free(List *list[static 1]);
void list_del(List list[static 1], void * data);
void list_ins_end(List list[static 1], void * data );
void list_ins_beg(List list[static 1], void * data );

#ifdef LIST_IMPLEMENTATION
 
 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
 
 
ListNode* createListNode(void * data );

ListNode* createListNode(void * data) {
    ListNode* newListNode = malloc(sizeof(ListNode));
    if (newListNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newListNode->data = data;

    newListNode->next = NULL;
    return newListNode;
}

void list_create(List * list[static 1]){
	(*list) = calloc(1,sizeof(List));
}
// Function to insert a ListNode at the beginning of the list
void list_ins_beg(List list[static 1], void * data) {
    ListNode* newListNode = createListNode(data);
    newListNode->next = list->head;
    list->head = newListNode;
 
}
 

// Function to insert a ListNode at the end of the list
void list_ins_end(List list[static 1],void * data) {
    ListNode* newListNode = createListNode(data);
    if (list->head == NULL) {
        list->head = newListNode;
    } else {
        ListNode* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newListNode;
    }
	 
}


// Function to delete a ListNode by value
void list_del(List list[static 1], void * data) {
    ListNode* temp = list->head;
    ListNode* prev = NULL;

    // If the ListNode to be deleted is the head
    if (temp != NULL && temp->data == data) {
        list->head = temp->next;
        free(temp);
        return;
    }

    // Search for the ListNode to be deleted
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }

    // If the ListNode was not found
    if (temp == NULL) {
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
    while (temp != NULL) {
        ListNode* next = temp->next;
	 
        free(temp);
        temp = next;
    }
	free((*list));
    (*list) = nullptr;
 
}

 

#endif