/*
 * Linked list of vertices
 *
 * This module provides a double-headed, doubly-linked list data
 * structure that can be used to implement graph algorithms that
 * require a stack or queue (or any sort of list of vertices)
 *
 */

#ifndef INCLUDE_VLIST_H_
#define INCLUDE_VLIST_H_

#include "graph.h"


/* ADDITIONAL RESULT CODES */
#define EEMPTY    (-128)


/* DATA STRUCTURES */

/* Forward declaration */
typedef struct vlist_node vlist_node_t;

/* A node in the linked list */
typedef struct vlist_node {
    /* Pointer to vertex contained in this node */
    vertex_t *v;

    /* Pointers to previous and next node in the list */
    vlist_node_t *prev;
    vlist_node_t *next;
} vlist_node_t;

/* List container */
typedef struct vlist {
    /* Length of the list */
    unsigned int length;

    /* Pointers to the head and tail nodes */
    vlist_node_t *head;
    vlist_node_t *tail;
} vlist_t;


/*
 * Initializes a list
 *
 * Parameters:
 *  - l: The list to initialize. Must point to allocated memory.
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 */
int vlist_init(vlist_t *l);

/*
 * Frees resources associated with a graph
 *
 * Parameters:
 *  - l: The list
 *
 * Returns:
 *  - Always returns 0
 */
int vlist_free(vlist_t *l);

/*
 * Prints the contents of a list
 * (from head to tail, left to right)
 *
 * Parameters:
 *  - l: The list
 *
 * Returns:
 *  - Always returns 0
 */
int vlist_print(vlist_t *l);

/*
 * Inserts a vertex at the head of the list
 *
 * Parameters:
 *  - l: The list
 *  - v: The vertex to insert
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 */
int vlist_insert_head(vlist_t *l, vertex_t *v);

/*
 * Inserts a vertex at the tail of the list
 *
 * Parameters:
 *  - l: The list
 *  - v: The vertex to insert
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 */
int vlist_insert_tail(vlist_t *l, vertex_t *v);

/*
 * Remove the vertex at the head of the list
 * and return its value.
 *
 * Parameters:
 *  - l: The list
 *  - v: Out parameter to return the vertex
 *
 * Returns:
 *  - 0 on success. If so, *v will contain the pointer
 *    to the vertex that was removed from the list.
 *  - EEMPTY: If the list was empty (nothing to return)
 */
int vlist_remove_head(vlist_t *l, vertex_t **v);

/*
 * Remove the vertex at the head of the list
 * and return its value.
 *
 * Parameters:
 *  - l: The list
 *  - v: Out parameter to return the vertex
 *
 * Returns:
 *  - 0 on success. If so, *v will contain the pointer
 *    to the vertex that was removed from the list.
 *  - EEMPTY: If the list was empty (nothing to return)
 */
int vlist_remove_tail(vlist_t *l, vertex_t **v);


/* Same as vlist_remove_head and vlist_remove_tail but
 * without removing the vertex from the list (it just returns
 * its value) */
int vlist_peek_head(vlist_t *l, vertex_t **v);
int vlist_peek_tail(vlist_t *l, vertex_t **v);


/* Functions for interacting with the list as a queue
 *
 * - vlist_enqueue calls vlist_insert_head and
 *   behaves in the same way
 * - vlist_dequeue calls vlist_remove_tail and
 *   behaves in the same way
 */
int vlist_enqueue(vlist_t *l, vertex_t *v);
int vlist_dequeue(vlist_t *l, vertex_t **v);


/* Functions for interacting with the list as a stack
 *
 * - vlist_push calls vlist_insert_head and
 *   behaves in the same way
 * - vlist_pop calls vlist_remove_head and
 *   behaves in the same way
 */
int vlist_push(vlist_t *l, vertex_t *v);
int vlist_pop(vlist_t *l, vertex_t **v);


#endif