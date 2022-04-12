#include "vlist.h"
#include "stdlib.h"


/* See vlist.h */
int vlist_init(vlist_t *l)
{
    l->head = NULL;
    l->tail = NULL;
    l->length = 0;

    return SUCCESS;
}


/* See vlist.h */
int vlist_free(vlist_t *l)
{
    vlist_node_t *pn = l->head;
    while(pn != NULL)
    {
        vlist_node_t *next = pn->next;
        free(pn);
        pn = next;
    }

    return SUCCESS;
}


/* See vlist.h */
int vlist_print(vlist_t *l)
{
    if(l->length == 0)
    {
        printf("EMPTY LIST\n");
    }
    else
    {
        vlist_node_t *pn = l->head;
        while (pn != NULL) {
            printf("%s ", pn->v->label ? pn->v->label : "NO LABEL");
            pn = pn->next;
        }
        printf("\n");
    }
    return SUCCESS;
}


/* See vlist.h */
int vlist_insert_head(vlist_t *l, vertex_t *v)
{
    vlist_node_t *node = calloc(1, sizeof(vlist_node_t));
    if(node == NULL)
        return ENOMEM;

    /* Set values in node */
    node->v = v;
    node->next = l->head;
    node->prev = NULL;

    /* If the list is non-empty, update the prev pointer
     * of the head node */
    if(l->head != NULL)
        l->head->prev = node;

    /* Update list values */

    /* If this is the first node we insert, we need to update
     * the tail pointer too */
    if(l->tail == NULL)
        l->tail = node;
    l->head = node;
    l->length++;

    return SUCCESS;
}


/* See vlist.h */
int vlist_insert_tail(vlist_t *l, vertex_t *v)
{
    vlist_node_t *node = calloc(1, sizeof(vlist_node_t));
    if(node == NULL)
        return ENOMEM;

    /* Set values in node */
    node->v = v;
    node->next = NULL;
    node->prev = l->tail;

    /* If the list is non-empty, update the next pointer
     * of the tail node */
    if(l->tail != NULL)
        l->tail->next = node;

    /* Update list values */

    /* If this is the first node we insert, we need to update
     * the head pointer too */
    if(l->head == NULL)
        l->head = node;
    l->tail = node;
    l->length++;

    return SUCCESS;
}


/* See vlist.h */
int vlist_remove_head(vlist_t *l, vertex_t **v)
{
    if(l->length == 0)
        return EEMPTY;

    if(v != NULL)
        *v = l->head->v;

    /* Remove head node */
    vlist_node_t *node = l->head;
    l->head = node->next;
    free(node);
    if(l->head != NULL)
        l->head->prev = NULL;
    else
        l->tail = NULL;

    l->length--;

    return SUCCESS;
}


/* See vlist.h */
int vlist_remove_tail(vlist_t *l, vertex_t **v)
{
    if(l->length == 0)
        return EEMPTY;

    if(v != NULL)
        *v = l->tail->v;

    /* Remove tail node */
    vlist_node_t *node = l->tail;
    l->tail = node->prev;
    free(node);
    if(l->tail != NULL)
        l->tail->next = NULL;
    else
        l->head = NULL;

    l->length--;

    return SUCCESS;
}


/* See vlist.h */
int vlist_peek_head(vlist_t *l, vertex_t **v)
{
    if(l->length == 0)
        return EEMPTY;

    *v = l->head->v;

    return SUCCESS;
}


/* See vlist.h */
int vlist_peek_tail(vlist_t *l, vertex_t **v)
{
    if(l->length == 0)
        return EEMPTY;

    *v = l->tail->v;

    return SUCCESS;
}


/* See vlist.h */
int vlist_enqueue(vlist_t *l, vertex_t *v)
{
    return vlist_insert_head(l, v);
}


/* See vlist.h */
int vlist_dequeue(vlist_t *l, vertex_t **v)
{
    return vlist_remove_tail(l, v);
}


/* See vlist.h */
int vlist_push(vlist_t *l, vertex_t *v)
{
    return vlist_insert_head(l, v);
}


/* See vlist.h */
int vlist_pop(vlist_t *l, vertex_t **v)
{
    return vlist_remove_head(l, v);
}