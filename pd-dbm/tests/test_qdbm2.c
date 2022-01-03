#include <depot.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME "mikio"
#define NUMBER "000-1234-5678"
#define DBNAME "book"

typedef struct _db {
    DEPOT *depot;
    char *val;
} t_db;

void test_create(t_db *x) {
    if (!(x->depot = dpopen(DBNAME, DP_OWRITER | DP_OCREAT, -1))) {
        fprintf(stderr, "dpopen: %s\n", dperrmsg(dpecode));
    }
}

void test_store(t_db *x) {
    if (!dpput(x->depot, NAME, -1, NUMBER, -1, DP_DOVER)) {
        fprintf(stderr, "dpput: %s\n", dperrmsg(dpecode));
    }
}

void test_get(t_db *x) {
    char *val;
    if (!(val = dpget(x->depot, NAME, -1, 0, -1, NULL))) {
        fprintf(stderr, "dpget: %s\n", dperrmsg(dpecode));
    } else {
        printf("Name: %s\n", NAME);
        printf("Number: %s\n", val);
    }
}

void test_close(t_db *x) {
    if (!dpclose(x->depot)) {
        fprintf(stderr, "dpclose: %s\n", dperrmsg(dpecode));
    }
}

int main(void) {
    t_db *x = (t_db *)malloc(sizeof(t_db));

    test_create(x);
    test_store(x);
    test_get(x);
    test_close(x);

    free(x);

    return 0;
}
