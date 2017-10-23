//
// Created by Matthew Connor Day on 23/10/17.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "date.h"


#define MAXLINE 100

int main() {
    printf("Currently creating Date Structs\n");
    Date * matt = date_create("31/03/1997");
    Date * emma = date_create("07/11/1999");
    Date * katie = date_create("16/08/2001");
    Date * katiecpy = date_duplicate(katie);

    printf("Currently comparing Date Structs\n");
    printf("%d\n", date_compare(matt, katie));
    printf("%d\n", date_compare(emma, emma));
    printf("%d\n", date_compare(katiecpy, emma));

    printf("Currently destroying Date Structs\n");
    date_destroy(matt);
    date_destroy(emma);
    date_destroy(katie);
    date_destroy(katiecpy);
}