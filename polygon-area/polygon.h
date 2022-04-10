/*
 * A two-dimensional polygon data structure
 */

#ifndef INCLUDE_POLYGON_H_
#define INCLUDE_POLYGON_H_

#include "common.h"
#include "point.h"

/* A polygon in two-dimensional space */
typedef struct {
    // Array of points in the polygon
    point_t *points;

    // Number of points in the polygon
    unsigned int n_points;

    // Space allocated for points (in number of points)
    unsigned int n_alloc;
} polygon_t;


/*
 * Allocates an empty polygon in the heap.
 *
 * Parameters:
 *  - None
 *
 * Returns:
 *  - A pointer to the polygon, or NULL if a point
 *    cannot be allocated
 */
polygon_t* polygon_new();


/*
 * Initializes a polygon (as an empty polygon(
 *
 * Parameters:
 *  - poly: A polygon. Must point to already allocated memory.
 *
 * Returns:
 *  - 0 on success, 1 if an error occurs.
 */
int polygon_init(polygon_t *poly);


/*
 * Frees the resources associated with a polygon
 *
 * Parameters:
 *  - poly: A polygon. Must point to a polygon allocated with polygon_new
 *
 * Returns:
 *  - Always returns 0.
 */
int polygon_free(polygon_t *poly);


/*
 * Adds a point (expressed as x, y coordinates) to a polygon
 *
 * Parameters:
 *  - poly: The polygon
 *  - x, y: The coordinates of the point to add
 *
 * Returns:
 *  - 0 on success, 1 if an error occurs.
 */
int polygon_add_xy(polygon_t *poly, double x, double y);


/*
 * Adds a point (expressed as x, y coordinates) to a polygon
 *
 * Parameters:
 *  - poly: The polygon
 *  - p: The point to add
 *
 * Returns:
 *  - 0 on success, 1 if an error occurs.
 */
int polygon_add_point(polygon_t *poly, point_t *p);


/*
 * Computes the perimeter of a polygon
 *
 * Parameters:
 *  - poly: The polygon
 *
 * Returns:
 *  - The perimeter of the polygon, or 0.0 if the polygon
 *    currently only has two points
 */
double polygon_perimeter(polygon_t *poly);


/*
 * Computes the area of a polygon
 *
 * Parameters:
 *  - poly: The polygon
 *
 * Returns:
 *  - The area of the polygon, or 0.0 if the polygon
 *    currently only has two points
 */
double polygon_area(polygon_t *poly);




#endif /* INCLUDE_POLYGON_H_ */
