#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "hashmap.h"
#include "m_pd.h"


#define HASHMAP_SIZE 1024

typedef struct _dict {
    t_hashmap map;
} t_dict;



void test_create(t_dict *x) {
    hashmap_create(HASHMAP_SIZE, &x->map);
}

void test_atom_put(t_dict *x, t_symbol *s, int argc, t_atom *argv) {

    if (0 != hashmap_put(&x->map, s->s_name, strlen(s->s_name), argv)) {
        printf("hashmap put error\n");
    }
}

void test_atom_geet(t_dict *x, t_symbol *s, int argc, t_atom *argv) {
    t_atom *res = NULL;
    res = (t_atom *)hashmap_get(&x->map, s->s_name, strlen(s->s_name));
    for (int i=0; i < argc; i++) {
        switch((argv+i)->a_type) {
            case A_FLOAT:
                printf("argv+%i: %0.2f\n", i, atom_getfloat(argv+i));
                break;
            case A_SYMBOL:
                printf("argv+%i: %s\n", i, atom_getsymbol(argv+i)->s_name);
                break;

            default:
                printf("argv+%i: ??\n", i);
        }        
    }
}


void test_int_put(t_dict *x, char *key, int value) {
    if (0 != hashmap_put(&x->map, key, strlen(key), &value)) {
        printf("hashmap put error\n");
    }
}

int test_int_get(t_dict *x, char *key) {
    int res = *(int *)hashmap_get(&x->map, key, strlen(key));
    return res;
}


void test_float_put(t_dict *x, char *key, float value) {
    if (0 != hashmap_put(&x->map, key, strlen(key), &value)) {
        printf("hashmap put error\n");
    }
}

float test_float_get(t_dict *x, char *key) {
    float res = *(float *)hashmap_get(&x->map, key, strlen(key));
    return res;
}


void test_string_put(t_dict *x, char *key, char *value) {
    if (0 != hashmap_put(&x->map, key, strlen(key), value)) {
        printf("hashmap put error\n");
    }
}


void test_string_get(t_dict *x, char *key, char *value) {
    char *str = NULL;
    str = (char *)hashmap_get(&x->map, key, strlen(key));
    assert(strcmp(str, value) == 0);
}


void test_destroy(t_dict *x) {
    hashmap_destroy(&x->map);
}


int main(void) {

    t_dict *x = (t_dict *) malloc(sizeof(t_dict));

    test_create(x);
    test_int_put(x, "hello", 10);
    int n = test_int_get(x, "hello");
    assert(n == 10);
    test_destroy(x);

    test_create(x);
    test_float_put(x, "hello", 10.20);
    float f = test_float_get(x, "hello");
    assert(fabs(f - 10.2) < 0.001);
    test_destroy(x);

    test_create(x);
    test_string_put(x, "hello", "world");
    test_string_get(x, "hello", "world");
    test_destroy(x);

    free(x);

}