#include "ll.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDENTICAL 0

//Linked list ADT identifier
struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

//Creates a linked list which starts off with only 2 dummy nodes,the head and tail which
//are null.
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));

    if (ll) {
        ll->length = 0;
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);

        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;

        ll->mtf = mtf;
    }

    return ll;
}

//Deletes a linked list. Uses the node_delete function to free each node in the list
//before freeing the list itself.
void ll_delete(LinkedList **ll) {
    Node *temp_node1 = (*ll)->head;
    while (temp_node1 != NULL) {
        Node *temp_node2 = temp_node1;
        temp_node1 = temp_node2->next;
        node_delete(&temp_node2);
    }
    free(*ll);
    *ll = NULL;
}

//Returns the length of a ll.
uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

//Looks up a node in a list. Traverses by creating a temporary node ADT.
//Uses strcmp to compare oldspeak and nodes oldspeak.
//If mtf specified then moves the node to the front of the ll.
//If node is not found then return a null node.
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    Node *temp;
    temp = ll->head->next;

    int string_comparison;

    for (uint32_t i = 0; i < ll_length(ll); i++) {

        string_comparison = strcmp(oldspeak, temp->oldspeak);
        if (string_comparison == IDENTICAL) {
            if (ll->mtf == true) {
                temp->next->prev = temp->prev;
                temp->prev->next = temp->next;

                temp->next = ll->head->next;  
                temp->prev = ll->head;

                ll->head->next = temp;
                temp->next->prev = temp;
				
				
                return temp;

            }

            else {
                return temp;
            }
        }

        temp = temp->next;
    }

    temp = NULL;
    return temp;
}

//Inserts a node into a ll.
//Creates a new node with node_create and the specified oldspeak and newspeak.
//Inserts at head of ll.
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
	Node *temp = ll_lookup(ll,oldspeak);
    if (temp == NULL) {
        Node *new_node;
        new_node = node_create(oldspeak, newspeak);

        new_node->next = ll->head->next;
        new_node->prev = ll->head;
        ll->head->next = new_node;
        new_node->next->prev = new_node;
        ll->length += 1;
    }
}

//Helper function to print ll.
void ll_print(LinkedList *ll) {
    Node *temp;
    temp = ll->head->next;

    for (uint32_t i = 0; i < ll_length(ll); i++) {
        node_print(temp);
        temp = temp->next;
    }
}




