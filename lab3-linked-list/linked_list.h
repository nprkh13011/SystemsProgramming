/***
 * Name        : lab3.c
 * Author      : Chancelor Assiamah & Nidhi Parekh
 * Date        : June 4, 2021
 * Description : Lab on Doubly Linked
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 **/

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <string.h>
#include "node.h"

typedef struct linked_list {
    node *head;
    node *tail;
    size_t num_nodes;
} linked_list;

linked_list* create_linked_list() {
    return (linked_list *)calloc(1, sizeof(linked_list));
}

/**
 * Inserts a non-NULL node into a non-NULL linked list.
 * The node is inserted so that the data in the linked list is in
 * non-decreasing order.
 * A node with a value already in the list is inserted AFTER the node(s)
 * already in the list.
 */
void insert_in_order(linked_list *list, node *n,
                     int (*cmp)(const void*, const void*)) {
    // If the node is null, then return.
    if (n == NULL)
        return;
    // If the list is null, then insert the node into the list.
    if (list->head == NULL)
        list->head = list->tail = n;
    
    // Else, we have a non-null node and a non-null list.
    
    /** 
     * We have a few cases to consider when adding a node to the list:
     * 1. List has only one element.
     * 2. List has two or more elements.
     * 2a. The node should be placed at the front of the list.
     * 2b. The node should be placed in the middle of the list.
     * 2c. The node should be placed at the end of the list.
     */
    // Else, if there is only one element in the list...
    else if (!list->head->next) {
        // If n's data is less than the head's data, add it to the head.
        if ( cmp(n->data, list->head->data) < 0) {
            n->next = list->head;
            list->head->prev = n;
            list->head = n;
        }
        // Else, add it to the tail.
        else {
            n->prev = list->tail;
            list->tail->next = n;
            list->tail = n;
        }
    }
    // Else, there are at least two elements in the list.
    else {
        // Create two node pointers that hold the first and second elements 
        // respectively.
        node* curr1 = list->head;
        node* curr2 = list->head->next;
        // Initialize an integer to hold the result of the comparison.
        int cmp_value = cmp(n->data, curr1->data);
        // While n's data is greater than curr1's data AND curr2 is not null...
        while (cmp_value > 0 && curr2) {
            // Move up curr1 and curr2.
            curr1 = curr1->next;
            curr2 = curr2->next;
            // Make a new comparison.
            cmp_value = cmp(n->data, curr1->data);
        }

        /**
         * Once the while loop breaks, we know that curr1 is either at the start 
         * of the list/in the middle of the list (cmp_value <= 0) or it is at the 
         * end (curr2 will be null).
         */

        
        // Else, curr1 is at the start of the list or in the middle of the list.
        if (cmp_value <= 0) {
            // If curr1 is at the start of the list, then add n to the head.
            if (!curr1->prev) {
                curr1->prev = n;
                n->next = curr1;
                list->head = n;
            }
            // Else, add n to the current place in the list.
            else {
                curr1->prev->next = n;
                n->prev = curr1->prev;
                curr1->prev = n;
                n->next = curr1;
            }
        }

        // Else, curr2 is null, so we insert n at the end of the list.
        else {
            curr1->next = n;
            n->prev = curr1;
            list->tail = n;
        }
    }

    // Finally, increment the number of nodes by 1.
    list->num_nodes++;
}

void print_list(linked_list *list, void (*print_function)(void*)) {
    putchar('[');
    node *cur = list->head;
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    printf("]\n{length: %lu, head->data: ", list->num_nodes);
    list->head != NULL ? print_function(list->head->data) :
                         (void)printf("NULL");
    printf(", tail->data: ");
    list->tail != NULL ? print_function(list->tail->data) :
                         (void)printf("NULL");
    printf("}\n\n");
}

/**
 * Frees a list starting from the tail.
 * This will check if your previous pointers have been set up correctly.
 */
void free_list(linked_list *list, void (*free_data)(void *)) {
    while (list->tail != NULL) {
        node *prev = list->tail->prev;
        free_node(list->tail, free_data);
        list->tail = prev;
    }
}

#endif
