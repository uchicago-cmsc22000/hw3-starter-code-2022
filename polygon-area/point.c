/*
 * A two-dimensional point data structure.
 *
 * See point.h for function documentation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "utils.h"
#include "point.h"

/* See point.h */
point_t* point_new(double x, double y)
{
    point_t *p;
    int rc;

    p = malloc(sizeof(point_t));

    if(p == NULL)
    {
        error("Could not allocate memory");
        return NULL;
    }

    rc = point_init(p, x, y);
    if(rc != SUCCESS)
    {
        error("Could not initialize point with (%.2f, %.2f)", x, y);
        return NULL;
    }

    return p;
}

/* See point.h */
int point_init(point_t *p, double x, double y)
{
    assert(p != NULL);

    p->x = x;
    p->y = y;

    return SUCCESS;
}

/* See point.h */
int point_free(point_t *p)
{
    assert(p != NULL);

    free(p);

    return SUCCESS;
}

/* See point.h */
double point_distance(point_t *p1, point_t *p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);

    return sqrt( pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) );
}

/* Code to determine segment intersection adapted from
 * https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ *
 *
 * Modified to not consider connected segments to intersect (unless they overlap) */


/* Given three colinear points p, q, r, the function checks if
   point q lies on line segment 'pr' */
bool on_segment(point_t *p, point_t *q, point_t *r)
{
    if (q->x == p->x && q->y == p->y)
        return false;

    if (q->x == r->x && q->y == r->y)
        return false;

    if (q->x <= max(p->x, r->x) && q->x >= min(p->x, r->x) &&
        q->y <= max(p->y, r->y) && q->y >= min(p->y, r->y))
       return true;

    return false;
}


/* Finds the orientation of the ordered triplet (p, q, r).
 *
 * Parameters:
 *  - p, q, r: points
 *
 * Return:
 *  - 0 if p, q and r are colinear
 *  - 1 if p, q, and r are clockwise
 *  - 2 if p, q, and r are counterclockwise
 */
int orientation(point_t *p, point_t *q, point_t *r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q->y - p->y) * (r->x - q->x) -
              (q->x - p->x) * (r->y - q->y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}


/* See point.h */
bool segment_intersect(point_t *p1, point_t *q1, point_t *p2, point_t *q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    bool connected = (q1->x == p2->x && q1->y == p2->y);

    // General case
    if (o1 != o2 && o3 != o4 && !connected)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && on_segment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && on_segment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && on_segment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}



