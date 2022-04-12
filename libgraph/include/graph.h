/*
 * A graph implementation using adjacency lists
 */

#ifndef INCLUDE_GRAPH_H_
#define INCLUDE_GRAPH_H_

#include <stdio.h>
#include <stdbool.h>

/* CONSTANTS */
#define MAX_LABEL_LEN (100)

/* RESULT CODES */
#define SUCCESS   (0)
#define ENOMEM    (-1)
#define EINDEX    (-2)
#define ENOTFOUND (-3)
#define EFILE     (-4)
#define EPARSE    (-5)
#define EINVAL    (-6)


#define CHECK_STATUS(rc)  {\
                            if (rc!=SUCCESS)\
                            {\
                                printf("%s:%i  Unexpected error %i\n", __FILE__, __LINE__, rc);\
                                exit(rc);\
                            }\
                           }

/* DATA STRUCTURES
 *
 * The graph stores the vertices as an array of vertex_t structs.
 * A vertex can be identified by its index in the array. Each vertex
 * also has an optional string label, which can be used to identify
 * the vertex.
 *
 * Each vertex has a singly-linked list of edge_t struct, containing
 * information on the vertex's outgoing edges.
 *
 */

/* Forward declarations */
typedef struct vertex vertex_t;
typedef struct edge edge_t;

/* A node in the list of edges */
typedef struct edge {
    /* The vertex this edge leads to */
    vertex_t* to;

    /* The weight of this edge */
    double weight;

    /* The next edge in the list (NULL if none) */
    edge_t* next;
} edge_t;


/* A graph vertex */
typedef struct vertex {
    /* String label for the vertex. Can be NULL. */
    char* label;

    /* Linked list of edges from this vertex */
    edge_t* edges;
} vertex_t;


/* A graph */
typedef struct graph {
    /* The number of vertices in the graph */
    unsigned int n_vertices;

    /* Dynamically allocated array of vertices */
    vertex_t* vertices;
} graph_t;


/* FUNCTIONS */

/*
 * Initializes a graph with some number of vertices
 *
 * Parameters:
 *  - g: The graph to initialize. Must point to allocated memory.
 *  - n: The number of vertices
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 *  - ENINVAL: If the number of vertices is zero
 *
 */
int graph_init(graph_t *g, unsigned int n);

/*
 * Frees resources associated with a graph
 *
 * Parameters:
 *  - g: The graph
 *
 * Returns:
 *  - 0 on success
 *
 */
int graph_free(graph_t *g);

/*
 * Prints the vertices and edges in a graph
 *
 * Parameters:
 *  - g: The graph
 *
 * Returns:
 *  - 0 on success
 *
 */
int graph_print(graph_t *g);

/*
 * Sets the label of a vertex
 *
 * Parameters:
 *  - g: A graph
 *  - i: Index of vertex
 *  - label: The label
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 *  - EINDEX: If the provided index is invalid
 *
 */
int graph_set_label(graph_t *g, unsigned int i, const char *label);

/*
 * Gets a vertex in the graph
 *
 * Parameters:
 *  - g: The graph
 *  - i The numerical index of the vertex
 *  - v: Output parameter. Must point to enough memory to
 *    store a vertex_t*.
 *
 * Returns:
 *  - 0 if the vertex exists. The vertex_t* pointed by 'v' will store
 *    a pointer to the vertex.
 *  - EINDEX: If the provided index is invalid
 */
int graph_get_vertex(graph_t *g, unsigned int i, vertex_t **v);

/*
 * Given a vertex, returns its index in the vertex array
 *
 * Parameters:
 *  - g: The graph
 *  - v: The vertex
 *
 * Returns:
 *  - The vertex index, if the given vertex is valid.
 *  - Otherwise, ENOTFOUND.
 */
long int graph_vertex_index(graph_t *g, vertex_t *v);

/*
 * Adds a directed edge to a graph
 *
 * Parameters:
 *  - g: The graph where the edge will be added
 *  - from, to: The numerical indices of the vertices connected by this edge
 *  - n: The number of vertices
 *
 * Returns:
 *  - 0 on success
 *  - EINDEX: If one of the provided indices is invalid
 *  - ENOMEM: If there was insufficient memory
 */
int graph_add_edge(graph_t *g, unsigned int from, unsigned int to, double weight);

/*
 * Checks whether two vertices are adjacent
 *
 * Parameters:
 *  - g: The graph where the edge will be added
 *  - from, to: The numerical indices of the vertices
 *  - weight: Output parameter. Must either be NULL, or must point
 *    to a double.
 *
 * Returns:
 *  - 0 if there is NO edge going from vertex "from" to vertex "to"
 *  - 1 if such an edge exists. If 'weight' is not NULL, then the
 *    weight is stored in the location referenced by 'weight'.
 *  - EINDEX: If one of the provided indices is invalid
 */
int graph_is_vertex_adjacent(graph_t *g, unsigned int from, unsigned int to, double *weight);


/*
 * Computes the number of vertices with a self-edge
 *
 * Parameters:
 *  - g: The graph
 *
 * Returns:
 *  - The number of vertices (>=0)
 */
int graph_num_vertex_with_loops(graph_t *g);


/*
 * Loads a graph from a file
 *
 * Parameters:
 *  - g: The graph to initialize. Must point to allocated memory.
 *  - filename: The file containing the graph specification.
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 *  - EFILE: Error when opening/reading the file
 *  - EPARSE: If the graph file could not be parsed
 */
int graph_from_file(graph_t *g, const char *filename);

/*
 * Saves a graph to a .dot file
 *
 * This .dot file can then be visualized with Graphviz (https://www.graphviz.org/)
 *
 * Parameters:
 *  - g: The graph to save.
 *  - filename: The file to save to
 *  - undirected: If true, generate an undirected graph (otherwise, an directed one)
 *  - weights: If true, print the weights.
 *
 * Returns:
 *  - 0 on success
 *  - ENOMEM: If there was insufficient memory
 *  - ENOFILE: If the specified file does not exist
 *  - EPARSE: If the graph file could not be parsed
 */
int graph_to_dot(graph_t *g, const char *filename, bool undirected, bool weights);


/* BY-LABEL FUNCTIONS
 *
 * All these functions (ending in _lbl) behave like their counterparts above, except they
 * take vertex labels as parameters instead of vertex indices.
 *
 * Instead of EINDEX, they return ENOTFOUND if either of the string labels is NULL
 * or does not have a corresponding vertex.
 */
int graph_get_vertex_lbl(graph_t *g, const char *label, vertex_t **v);
int graph_add_edge_lbl(graph_t *g, const char *from, const char *to, double weight);

#endif