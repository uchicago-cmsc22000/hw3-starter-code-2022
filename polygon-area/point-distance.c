#include <stdio.h>
#include "point.h"

int main()
{
    point_t p1, p2;

    printf("[Point 1] Please enter a value for x: ");
    scanf("%lf", &p1.x);

    printf("[Point 1] Please enter a value for y: ");
    scanf("%lf", &p1.y);

    printf("[Point 2] Please enter a value for x: ");
    scanf("%lf", &p2.x);

    printf("[Point 2] Please enter a value for y: ");
    scanf("%lf", &p2.y);

    printf("The distance between (%.2lf, %.2lf) and "
           "(%.2lf, %.2lf) is %.2lf\n", p1.x, p1.y, p2.x, p2.y,
           point_distance(&p1, &p2));
}
