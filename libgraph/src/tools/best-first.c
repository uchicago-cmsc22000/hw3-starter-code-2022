#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>
#include "graph.h"

int main(int argc, char *argv[])
{
    int opt;
    char *graphfile = NULL;
    char *start_label = NULL, *final_label = NULL;

    /* Parse command-line options */
    while ((opt = getopt(argc, argv, "g:s:f:h")) != -1)
        switch (opt)
        {
            case 'g':
                graphfile = strdup(optarg);
                break;
            case 's':
                start_label = strdup(optarg);
                break;
            case 'f':
                final_label = strdup(optarg);
                break;
            case 'h':
                printf("Usage: best-first -g GRAPH_FILE -s START_VERTEX -f FINAL_VERTEX\n");
                exit(0);
                break;
            default:
                printf("ERROR: Unknown option -%c\n", opt);
                exit(-1);
        }

    /* Validate parameters */
    if(graphfile == NULL)
    {
        printf("You must specify files a graph file with the -g option\n");
        exit(-1);
    }

    if(start_label == NULL || final_label == NULL)
    {
        printf("You must specify files a start and final vertex with -s and -f\n");
        exit(-1);
    }

    int rc;
    vertex_t *start_vertex, *final_vertex;
    graph_t g;

    rc = graph_from_file(&g, graphfile);
    CHECK_STATUS(rc);

    rc = graph_get_vertex_lbl(&g, start_label, &start_vertex);
    if(rc == ENOTFOUND)
    {
        printf("No such vertex in graph: %s\n", start_label);
        return ENOTFOUND;
    }

    rc = graph_get_vertex_lbl(&g, final_label, &final_vertex);
    if(rc == ENOTFOUND)
    {
        printf("No such vertex in graph: %s\n", final_label);
        return ENOTFOUND;
    }

    bool *visited = calloc(g.n_vertices, sizeof(bool));
    vertex_t *cur = start_vertex;
    edge_t *e;
    double total_weight = 0.0;

    printf("%s", cur->label);
    while(cur != final_vertex)
    {
        /* Mark current vertex as visited */
        visited[graph_vertex_index(&g, cur)] = true;

        e = cur->edges;
        double best_weight = INFINITY;
        vertex_t *best_vertex = NULL;

        while(e != NULL)
        {
            if(e->to == final_vertex)
            {
                best_weight = e->weight;
                best_vertex = e->to;
                break;
            }

            if(! visited[graph_vertex_index(&g, e->to)] )
            {
                if (e->weight < best_weight) {
                    best_weight = e->weight;
                    best_vertex = e->to;
                }
            }

            e = e->next;
        }

        if(best_vertex == NULL)
        {
            printf(" -> DEAD END!");
            break;
        }

        printf(" -> %s", best_vertex->label);

        cur = best_vertex;
        total_weight += best_weight;
    }

    printf("\nTotal weight: %.2f\n", total_weight);

    return SUCCESS;
}