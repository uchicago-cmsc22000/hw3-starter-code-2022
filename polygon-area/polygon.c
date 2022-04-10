/*
 * A two-dimensional polygon data structure.
 *
 * See polygon.h for function documentation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "utils.h"
#include "polygon.h"

/* Amount of space (in points) to allocate in a new polygon */
#define INITIAL_ALLOC (10)

/* See polygon.h */
polygon_t* polygon_new()
{
    polygon_t *poly;
    int rc;

    poly = malloc(sizeof(polygon_t));

    if(poly == NULL)
    {
        error("Could not allocate memory");
        return NULL;
    }

    rc = polygon_init(poly);
    if(rc != SUCCESS)
    {
        error("Could not initialize polygon");
        return NULL;
    }

    return poly;
}

/* See polygon.h */
int polygon_init(polygon_t *poly)
{
    assert(poly != NULL);


    poly->n_points = 0;
    poly->n_alloc = 10;
    poly->points = malloc(INITIAL_ALLOC * sizeof(point_t));

    if(poly->points == NULL)
    {
        error("Could not allocate memory for points array");
        return FAILURE;
    }

    return SUCCESS;
}

/* See polygon.h */
int polygon_free(polygon_t *poly)
{
    assert(poly != NULL);

    free(poly->points);
    free(poly);

    return SUCCESS;
}

/* See polygon.h */
int polygon_add_xy(polygon_t *poly, double x, double y)
{
    assert(poly != NULL);

    point_t p;

    point_init(&p, x, y);

    return polygon_add_point(poly, &p);
}


/* See polygon.h */
int polygon_add_point(polygon_t *poly, point_t *p)
{
    assert(poly != NULL);
    assert(p != NULL);

    /* Verify that this point is not already in the polygon */
    for(unsigned int i=0; i < poly->n_points; i++)
    {
        point_t *cur = &poly->points[i];

        if(cur->x == p->x && cur->y == p->y)
        {
            error("Tried adding a point that is already in the polygon (%.2f, %.2f)", p->x, p->y);
            return FAILURE;
        }
    }

    /* Make sure the additional segment does not intersect with any
     * existing segments */
    if(poly->n_points >= 2)
    {
        point_t *new_seg1, *new_seg2, *cur_seg1, *cur_seg2;

        new_seg1 = &poly->points[poly->n_points - 1];
        new_seg2 = p;

        for(unsigned int i=0; i < poly->n_points - 1; i++)
        {
            cur_seg1 = &poly->points[i];
            cur_seg2 = &poly->points[i+1];

            if(segment_intersect(cur_seg1, cur_seg2, new_seg1, new_seg2))
            {
                error("Adding point creates segment (%.2f, %.2f) -> (%.2f, %.2f) "
                      "which intersects with (%.2f, %.2f) -> (%.2f, %.2f)",
                      new_seg1->x, new_seg1->y, new_seg2->x, new_seg2->y,
                      cur_seg1->x, cur_seg1->y, cur_seg2->x, cur_seg2->y);
                return FAILURE;
            }
        }
    }

    /* Allocate more memory if necessary */
    if(poly->n_points == poly->n_alloc)
    {
        unsigned int new_size = (unsigned int) (poly->n_alloc * 1.5);
        poly->points = realloc(poly->points, new_size * sizeof(point_t));

        if(poly->points == NULL)
        {
            error("Could not re-allocate memory for points array");
            return FAILURE;
        }
    }

    poly->points[poly->n_points].x = p->x;
    poly->points[poly->n_points].y = p->y;

    poly->n_points++;

    return SUCCESS;
}


/* See polygon.h */
double polygon_perimeter(polygon_t *poly)
{
    if(poly->n_points <= 2)
        return 0.0;

    double perimeter = 0.0;

    point_t *cur_seg1, *cur_seg2;

    for(unsigned int i=0; i < poly->n_points; i++)
    {
        cur_seg1 = &poly->points[i];

        if(i == poly->n_points - 1)
            /* Last segment wraps around to first point */
            cur_seg2 = &poly->points[0];
        else
            cur_seg2 = &poly->points[i+1];


        perimeter += point_distance(cur_seg1, cur_seg2);
    }

    return perimeter;
}


/* See polygon.h */
double polygon_area(polygon_t *poly)
{
    if(poly->n_points <= 2)
        return 0.0;

    /* We use the shoelace formula to compute the area
     * https://en.wikipedia.org/wiki/Shoelace_formula */

    double area = 0.0;

    point_t *cur, *prev;

    prev = &poly->points[poly->n_points - 1];
    for(unsigned int i=0; i < poly->n_points; i++)
    {
        cur = &poly->points[i];

        area += (prev->x + cur->x) * (prev->y - cur->y);

        prev = cur;
    }

    return fabs(area / 2.0);
}
