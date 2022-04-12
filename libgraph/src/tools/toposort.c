#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include "algorithms.h"

int main(int argc, char *argv[])
{
    int opt;
    char *graphfile = NULL;
    int start_vertex = 0;

    /* Parse command-line options */
    while ((opt = getopt(argc, argv, "g:s:h")) != -1)
        switch (opt)
        {
            case 'g':
                graphfile = strdup(optarg);
                break;
            case 's':
                start_vertex = strtol(optarg, NULL, 10);
                break;
            case 'h':
                printf("Usage: toposort -g GRAPH_FILE -s START_VERTEX \n");
                exit(0);
                break;
            default:
                printf("ERROR: Unknown option -%c\n", opt);
                exit(-1);
        }

    /* Validate parameters */
    if(graphfile == NULL)
    {
        printf("You must specify files with the -g and -o options\n");
        exit(-1);
    }

    int rc;
    graph_t g;
    vlist_t *l;

    rc = graph_from_file(&g, graphfile);
    CHECK_STATUS(rc);

    rc = graph_toposort(&g, start_vertex, &l);
    CHECK_STATUS(rc);
    vlist_print(l);
    printf("\n");

    return SUCCESS;
}