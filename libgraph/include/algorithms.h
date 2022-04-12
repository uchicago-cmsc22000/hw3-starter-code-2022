/*
 * Graph algorithms
 *
 */

#ifndef INCLUDE_ALGORITHMS_H_
#define INCLUDE_ALGORITHMS_H_

#include "graph.h"
#include "vlist.h"

/*
 * Does a breadth-first traversal of a graph,
 * printing each vertex it visits.
 *
 * Parameters:
 *  - g: The graph
 *  - start: The numerical index of the start vertex
 *
 * Returns:
 *  - 0 on success
 *  - EINDEX: if the start index is invalid
 */
int graph_bfs(graph_t *g, unsigned int start);

/*
 * Does a depth-first traversal of a graph,
 * printing each vertex it visits.
 *
 * Parameters:
 *  - g: The graph
 *  - start: The numerical index of the start vertex
 *
 * Returns:
 *  - 0 on success
 *  - EINDEX: if the start index is invalid
 */
int graph_dfs(graph_t *g, unsigned int start);

/*
 * Does a topological sort of a graph
 *
 * Parameters:
 *  - g: The graph. Must be a DAG (directed acyclic graph)
 *  - start: The numerical index of the start vertex
 *  - l: Out parameter to return the list of vertices,
 *       topologically ordered
 *
 * Returns:
 *  - 0 on success. If so, this function allocates a list_t
 *    in the heap, and stores the pointer to the list_t in *l.
 *  - EINDEX: if the start index is invalid
 *  - ENOMEM: If there was insufficient memory
 */
int graph_toposort(graph_t *g, unsigned int start, vlist_t **l);

/*
 * Does a depth-first traversal of a graph,
 * printing each vertex it visits.
 *
 * Parameters:
 *  - g: The graph
 *  - start: The numerical index of the start vertex
 *
 * Returns:
 *  - 0 on success
 *  - EINDEX: if the start index is invalid
 */
int graph_dfs_iter(graph_t *g, unsigned int start);

/*
 * Produces a spanning tree from a graph (and, specifically
 * the DFS predecessor tree).
 *
 * Parameters:
 *  - g: The graph
 *  - start: The numerical index of the start vertex
 *  - tree: Out parameter for the predecessor tree
 *
 * Returns:
 *  - 0 on success. If so, this function allocates a graph_t
 *    in the heap, and stores the pointer to the graph_t in *tree.
 *  - EINDEX: if the start index is invalid
 *  - ENOMEM: If there was insufficient memory
 */
int graph_spanning_tree(graph_t *g, unsigned int start, graph_t **tree);

#endif