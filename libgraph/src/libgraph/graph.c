#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* See graph.h */
int graph_init(graph_t *g, unsigned int n)
{
    if(n == 0)
        return EINVAL;

    g->n_vertices = n;

    g->vertices = calloc(n, sizeof(vertex_t));

    if(g->vertices == NULL)
        return ENOMEM;

    return SUCCESS;
}


/* See graph.h */
int graph_free(graph_t *g)
{
    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        free(g->vertices[i].label);

        edge_t *pe = g->vertices[i].edges;
        while(pe != NULL)
        {
            edge_t *next = pe->next;
            free(pe);
            pe = next;
        }
    }

    free(g->vertices);

    return SUCCESS;
}


/* See graph.h */
int graph_print(graph_t *g)
{
    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        if(g->vertices[i].label == NULL)
            printf("%i:", i);
        else
            printf("%s:", g->vertices[i].label);

        edge_t *pe = g->vertices[i].edges;

        while(pe != NULL)
        {
            if(pe->to->label == NULL)
                printf(" -> %li", graph_vertex_index(g, pe->to));
            else
                printf(" -> %s", pe->to->label);

            pe = pe->next;
        }
        printf("\n");
    }

    return SUCCESS;
}


/* See graph.h */
int graph_set_label(graph_t *g, unsigned int i, const char *label)
{
    /* Get vertex */
    if(i >= g->n_vertices)
        return EINDEX;

    vertex_t *v = &g->vertices[i];

    /* Free previous label, if one exists */
    if(v->label != NULL)
        free(v->label);

    if(label == NULL)
    {
        v->label = NULL;
    }
    else
    {
        char *new_label = strndup(label, MAX_LABEL_LEN);

        if(new_label == NULL)
            return ENOMEM;

        v->label = new_label;
    }

    return SUCCESS;
}

/* See graph.h */
int graph_get_vertex(graph_t *g, unsigned int i, vertex_t **v)
{
    /* Get vertex */
    if(i >= g->n_vertices)
        return EINDEX;

    *v = &g->vertices[i];

    return SUCCESS;
}

/* See graph.h */
long int graph_vertex_index(graph_t *g, vertex_t *v)
{
    long int i = (v) - (g)->vertices;

    if(i < 0 || i >= g->n_vertices)
        return ENOTFOUND;
    else
        return i;
}

/* See graph.h */
int graph_add_edge(graph_t *g, unsigned int from, unsigned int to, double weight)
{
    /* Get vertices */
    if(from >= g->n_vertices || to >= g->n_vertices)
        return EINDEX;

    vertex_t *from_v = &g->vertices[from];
    vertex_t *to_v = &g->vertices[to];

    /* Create edge */
    edge_t *e = calloc(1, sizeof(edge_t));

    if(e == NULL)
        return ENOMEM;

    e->to = to_v;
    e->weight = weight;

    /* Add to edge list */
    e->next = from_v->edges;
    from_v->edges = e;

    return SUCCESS;
}


/*
 * Helper function: retrieves an edge
 *
 * Parameters:
 *  - g: The graph
 *  - from, to: The numerical indices of the vertices
 *  - edge: Output parameter
 *
 * Returns:
 *  - 0: If the vertices exist. If so, 'edge' is set to NULL
 *    if there is no edge connecting them; otherwise, it will
 *    point to the edge_t struct for that edge.
 *  - EINDEX: If one of the provided indices is invalid
 */
static int graph_get_edge(graph_t *g, unsigned int from, unsigned int to, edge_t **e)
{
    /* Get vertices */
    if(from >= g->n_vertices || to >= g->n_vertices)
        return EINDEX;

    vertex_t *from_v = &g->vertices[from];
    vertex_t *to_v = &g->vertices[to];

    /* Iterate over list of edges */
    *e = from_v->edges;
    while(*e != NULL)
    {
        if((*e)->to == to_v)
            break;

        *e = (*e)->next;
    }

    return SUCCESS;
}


/* See graph.h */
int graph_is_vertex_adjacent(graph_t *g, unsigned int from, unsigned int to, double *weight)
{
    int rc;
    edge_t *e;

    rc = graph_get_edge(g, from, to, &e);
    if(rc == EINDEX)
        return EINDEX;

    if(e == NULL)
    {
        return 0;
    }
    else
    {
        if(weight != NULL)
            *weight = e->weight;

        return 1;
    }
}


/* See graph.h */
int graph_num_vertex_with_loops(graph_t *g)
{
    int n_with_loops = 0;

    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        edge_t *e = g->vertices[i].edges;
        while(e != NULL)
        {
            if(e->to == &g->vertices[i])
            {
                n_with_loops++;
                break;
            }

            e = e->next;
        }
    }

    return n_with_loops;
}


/* See graph.h */
int graph_from_file(graph_t *g, const char *filename)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    unsigned int n_vertices, n_edges;
    bool undirected;
    int rc;

    fp = fopen(filename, "r");

    if(fp == NULL)
        return EFILE;

    /* Directed or undirected graph? */
    read = getline(&line, &len, fp);
    if(read == -1)
        return EPARSE;
    line[read - 1] = '\0'; // Strip newline

    if(strncmp(line, "undirected", 10) == 0)
        undirected = true;
    else if (strncmp(line, "directed", 8) == 0)
        undirected = false;
    else
        return EPARSE;

    /* Read number of vertices and edges */
    read = getline(&line, &len, fp);
    if(read == -1)
        return EPARSE;

    read = sscanf(line, "%i %i", &n_vertices, &n_edges);
    if(read != 2)
        return EPARSE;
    if(n_vertices == 0)
        return EPARSE;

    rc = graph_init(g, n_vertices);
    if(rc == ENOMEM)
        return ENOMEM;
    if(rc == EINVAL)
        return EPARSE;

    /* Read vertex labels */
    for(unsigned int i = 0; i < n_vertices; i++)
    {
        read = getline(&line, &len, fp);
        if(read == -1)
            return EPARSE;

        /* Strip newline if present */
        if (line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }

        rc = graph_set_label(g, i, line);
    }

    /* Read edges */
    for(unsigned int i = 0; i < n_edges; i++)
    {
        char label1[MAX_LABEL_LEN + 1], label2[MAX_LABEL_LEN + 1];
        double weight;

        read = getline(&line, &len, fp);
        if(read == -1)
            return EPARSE;

        read = sscanf(line, "%100s %100s %lf", label1, label2, &weight);
        if(read != 3)
            return EPARSE;

        rc = graph_add_edge_lbl(g, label1, label2, weight);
        if(rc == ENOTFOUND)
            return EPARSE;

        if(undirected)
        {
            rc = graph_add_edge_lbl(g, label2, label1, weight);
            if(rc == ENOTFOUND)
                return EPARSE;
        }
    }

    return SUCCESS;
}


/* See graph.h */
int graph_to_dot(graph_t *g, const char *filename, bool undirected, bool weights)
{
    char *edge_str;
    FILE *f;
    int rc;

    f = fopen(filename, "w");
    if (f == NULL)
    {
        return EFILE;
    }

    if(undirected)
    {
        fprintf(f, "graph g { concentrate=true\n");
        edge_str = "--";
    }
    else
    {
        fprintf(f, "digraph g {\n");
        edge_str = "->";
    }

    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        if(g->vertices[i].label != NULL)
            fprintf(f, "%i [label=\"%s\"];\n", i, g->vertices[i].label);
    }

    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        edge_t *pe = g->vertices[i].edges;

        while(pe != NULL)
        {
            fprintf(f, "%i %s %li", i, edge_str, graph_vertex_index(g, pe->to));

            if(weights)
                fprintf(f, " [label=\"%.2lf\"]", pe->weight);

            fprintf(f, ";\n");

            pe = pe->next;
        }
    }

    fprintf(f, "}\n");

    rc = fclose(f);
    if(rc != 0)
        return EFILE;

    return SUCCESS;
}


/* BY-LABEL FUNCTIONS
 * See graph.h */

/*
 * Helper function: finds the index of a vertex given its label
 *
 * Parameters:
 *  - g: The graph
 *  - label: The vertex label
 *
 * Returns:
 *  - If there is a vertex with the given label, returns its index
 *  - Otherwise, returns ENOTFOUND
 */
static int graph_label_to_index(graph_t *g, const char *label)
{
    for(unsigned int i=0; i < g->n_vertices; i++)
    {
        if(strncmp(g->vertices[i].label, label, MAX_LABEL_LEN) == 0)
            return i;
    }

    return ENOTFOUND;
}


/* By-label equivalent of graph_get_vertex */
int graph_get_vertex_lbl(graph_t *g, const char *label, vertex_t **v)
{
    int i;

    i = graph_label_to_index(g, label);
    if(i == ENOTFOUND)
        return ENOTFOUND;

    return graph_get_vertex(g, i, v);
}


/* By-label equivalent of graph_add_edge */
int graph_add_edge_lbl(graph_t *g, const char *from, const char *to, double weight)
{
    int from_i, to_i;

    from_i = graph_label_to_index(g, from);
    if(from_i == ENOTFOUND)
        return ENOTFOUND;

    to_i = graph_label_to_index(g, to);
    if(to_i == ENOTFOUND)
        return ENOTFOUND;

    return graph_add_edge(g, from_i, to_i, weight);
}