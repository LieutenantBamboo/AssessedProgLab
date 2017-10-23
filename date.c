// Created by Matthew Connor Day

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "date.h"

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr) {
    // Define struct and copy variables
    struct date *d = (struct date *) malloc(sizeof(struct date));
    char *datestr_cpy = malloc(sizeof(datestr));

    // Copy string across
    strcpy(datestr_cpy, datestr);
    // Get the next part of the string until the delim: "/"
    datestr_cpy = strtok(datestr_cpy, "/");

    // While there is still another part of the date to copy
    if (datestr_cpy != NULL && d != NULL){
        d->day = atoi(datestr_cpy);
        datestr_cpy = strtok(NULL, "/");
        d->month = atoi(datestr_cpy);
        datestr_cpy = strtok(NULL, "/");
        d->year = atoi(datestr_cpy);
        printf("Day: %d Month: %d Year: %d\n", d->day, d->month, d->year);
    }
    return d;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d) {
    // Copy by value, using dereferencing
    struct date *cpy = (struct date *) malloc(sizeof(struct date));

    if (cpy != NULL) {
        cpy->day = d->day;
        cpy->month = d->month;
        cpy->year = d->year;
    }
    return cpy;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2) {
    int cmp;
    // printf("Date1 Day: %d Month: %d Year: %d\n", date1->day, date1->month, date1->year);
    // printf("Date2 Day: %d Month: %d Year: %d\n", date2->day, date2->month, date2->year);

    if(date1->year == date2->year) {
        if(date1->month == date2->month){
            if(date1->day == date2->day){
                cmp = 0;
            } else {
                cmp = (date1->day - date2->day);
            }
        } else {
            cmp =  (date1->month - date2->month);
        }
    } else {
        cmp =  (date1->year - date2->year);
    }

    return cmp;
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d) {
    free(d);
}