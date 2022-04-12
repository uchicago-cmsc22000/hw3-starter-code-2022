#include "algorithms.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


/* See algorithms.h */
int graph_bfs(graph_t *g, unsigned int start)
{
    int rc;
    bool *visited;
    vertex_t *v_start;
    vlist_t queue;

    /* Create boolean array to keep track of what vertices
     * have been visited */
    visited = calloc(g->n_vertices, sizeof(bool));
    if(visited == NULL)
        return ENOMEM;

    /* Create queue */
    rc = vlist_init(&queue);
    if(rc != SUCCESS)
        return rc;

    /* Get the start vertex. Enqueue it and mask it as visited */
    rc = graph_get_vertex(g, start, &v_start);
    if(rc != SUCCESS)
        return rc;
    vlist_enqueue(&queue, v_start);
    visited[start] = true;

    while(queue.length > 0)
    {
        /* Dequeue a vertex from the queue*/
        vertex_t *v;
        rc = vlist_dequeue(&queue, &v);
        assert(rc == SUCCESS);

        /* Process the vertex (we just print it) */
        int i = graph_vertex_index(g, v);
        assert(i >= 0);
        printf("%i: %s\n", i, v->label? v->label : "NO LABEL");

        /* Iterate over the edges of the vertex */
        edge_t *e = v->edges;
        while(e != NULL)
        {
            vertex_t *v_next = e->to;
            int i_next = graph_vertex_index(g, v_next);
            assert(i_next >= 0);

            /* Process the vertex if we haven't already visited it */
            if(!visited[i_next])
            {
                visited[i_next] = true;
                rc = vlist_enqueue(&queue, v_next);
            }

            e = e->next;
        }

        assert(rc == SUCCESS);
    }

    return SUCCESS;
}


/*
 * This is the recursive implementation of DFS, called
 * by graph_dfs.
 *
 * Parameters:
 *  - g: The graph
 *  - i: The numerical index of the vertex to start with
 *  - visited: boolean array of visited vertices
 *
 * Returns:
 *  - Always returns 0
 */
static int graph_dfs_visit(graph_t *g, unsigned int i, bool *visited)
{
    int rc;
    vertex_t *v;

    /* Get the vertex and process it */
    rc = graph_get_vertex(g, i, &v);
    if(rc != SUCCESS)
        return rc;
    printf("%i: %s\n", i, v->label? v->label : "NO LABEL");

    /* Iterate over the edges of the vertex */
    edge_t *e = v->edges;
    while(e != NULL)
    {
        int i_next = graph_vertex_index(g, e->to);
        assert(i_next >= 0);

        /* If we haven't visited the next vertex, recursively process it*/
        if(!visited[i_next])
        {
            visited[i_next] = true;
            rc = graph_dfs_visit(g, i_next, visited);
            assert(rc == SUCCESS);
        }

        e = e->next;
    }

    return SUCCESS;
}


/* See algorithms.h */
int graph_dfs(graph_t *g, unsigned int start)
{
    /* Create boolean array to keep track of what vertices
     * have been visited */
    bool *visited = calloc(g->n_vertices, sizeof(bool));

    /* Mark start vertex visited */
    visited[start] = true;

    /* Make first call to recursive DFS function */
    int rc;
    rc = graph_dfs_visit(g, start, visited);
    if(rc != SUCCESS)
        return rc;

    unsigned int connected = 1;

    for(unsigned int i=0; i < g->n_vertices; i++)
        if(!visited[i])
        {
            connected++;
            rc = graph_dfs_visit(g, i, visited);
            assert(rc == SUCCESS);
        }

    return connected;
}


/*
 * This is the recursive implementation of toposort, called
 * by graph_toposort.
 *
 * Parameters:
 *  - g: The graph
 *  - i: The numerical index of the vertex to start with
 *  - visited: boolean array of visited vertices
 *  - l: the list of vertices to be returned. Assumed to be
 *       initialized by graph_toposort
 *
 * Returns:
 *  - Always returns 0
 */
static int graph_toposort_visit(graph_t *g, unsigned int i, bool *visited, vlist_t *l)
{
    int rc;
    vertex_t *v;

    rc = graph_get_vertex(g, i, &v);
    if(rc != SUCCESS)
        return rc;

    edge_t *e = v->edges;
    while(e != NULL)
    {
        int i_next = graph_vertex_index(g, e->to);
        if(i_next < 0)
            return i_next;

        if(!visited[i_next])
        {
            visited[i_next] = true;
            rc = graph_toposort_visit(g, i_next, visited, l);
            assert(rc == SUCCESS);
        }

        e = e->next;
    }

    vlist_insert_head(l, v);

    return SUCCESS;
}


/* See algorithms.h */
int graph_toposort(graph_t *g, unsigned int start, vlist_t **l)
{
    /* Create boolean array to keep track of what vertices
     * have been visited */
    bool *visited = calloc(g->n_vertices, sizeof(bool));

    /* Create empty list of vertices, where the topologically-sorted
     * vertices will be stored */
    int rc;
    *l = calloc(1, sizeof(vlist_t));
    rc = vlist_init(*l);
    if(rc != SUCCESS)
        return rc;

    /* Mark start vertex visited */
    visited[start] = true;

    /* Make first recursive call */
    rc = graph_toposort_visit(g, start, visited, *l);
    if(rc != SUCCESS)
        return rc;

    return SUCCESS;
}


/* See algorithms.h */
int graph_dfs_iter(graph_t *g, unsigned int start)
{
    int rc;
    bool *visited;
    vertex_t *v_start;
    vlist_t stack;

    visited = calloc(g->n_vertices, sizeof(bool));
    if(visited == NULL)
        return ENOMEM;

    rc = vlist_init(&stack);
    if(rc != SUCCESS)
        return rc;

    rc = graph_get_vertex(g, start, &v_start);
    if(rc != SUCCESS)
        return rc;
    vlist_push(&stack, v_start);
    visited[start] = true;

    while(stack.length > 0)
    {
        vertex_t *v;
        rc = vlist_pop(&stack, &v);
        assert(rc == SUCCESS);

        /* Process the vertex (we just print it) */
        int i = graph_vertex_index(g, v);
        assert(i >= 0);
        printf("%i: %s\n", i, v->label? v->label : "NO LABEL");

        edge_t *e = v->edges;
        while(e != NULL)
        {
            vertex_t *v_next = e->to;
            int i_next = graph_vertex_index(g, v_next);
            assert(i_next >= 0);

            if(!visited[i_next])
            {
                visited[i_next] = true;
                rc = vlist_push(&stack, v_next);
            }

            e = e->next;
        }

        assert(rc == SUCCESS);
    }

    return SUCCESS;
}



static int graph_spanning_tree_visit(graph_t *g, unsigned int i, bool *visited, graph_t *tree)
{
    int rc;
    vertex_t *v;

    rc = graph_get_vertex(g, i, &v);
    assert(rc == SUCCESS);

    edge_t *e = v->edges;
    while(e != NULL)
    {
        int i_next = graph_vertex_index(g, e->to);
        if(i_next < 0)
            return i_next;

        if(!visited[i_next])
        {
            visited[i_next] = true;
            printf("%i %i\n", i, i_next);
            rc = graph_add_edge(tree, i, i_next, e->weight);
            assert(rc == SUCCESS);

            rc = graph_spanning_tree_visit(g, i_next, visited, tree);
            assert(rc == SUCCESS);
        }

        e = e->next;
    }

    return SUCCESS;
}

/* See algorithms.h */
int graph_spanning_tree(graph_t *g, unsigned int start, graph_t **tree)
{
    bool *visited = calloc(g->n_vertices, sizeof(bool));
    int rc;

    *tree = calloc(1, sizeof(graph_t));
    rc = graph_init(*tree, g->n_vertices);

    for(unsigned int i=0; i < g->n_vertices; i++)
        graph_set_label(*tree, i, g->vertices[i].label);

    /* Mark start vertex visited */
    visited[start] = true;

    rc = graph_spanning_tree_visit(g, start, visited, *tree);
    assert(rc == SUCCESS);

    return SUCCESS;
}