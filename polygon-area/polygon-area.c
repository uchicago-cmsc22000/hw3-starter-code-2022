#include <stdio.h>
#include "polygon.h"

int main()
{
    polygon_t *poly;

    poly = polygon_new();

    while(true)
    {
        float x, y;
        int rc;
        char yesno;

        printf("Please enter a value for x: ");
        scanf("%f", &x);

        printf("Please enter a value for y: ");
        scanf("%f", &y);

        rc = polygon_add_xy(poly, x, y);

        if(rc != SUCCESS)
        {
            printf("I'm sorry, I can't add that point\n\n");
            continue;
        }

        printf("Would you like to enter another point? (y/n) ");
        scanf(" %c", &yesno);

        if(yesno != 'y')
            break;
    }

    if(poly->n_points < 3)
    {
        printf("I'm sorry, I need at least three points to compute an area.\n");
    }
    else
    {
        printf("The area is %.2f\n", polygon_area(poly));
    }
}
