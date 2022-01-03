
#include <assert.h>

#include "m_pd.h"

#include <depot.h>
#include <stdlib.h>

#define NAME     "mikio"
#define NUMBER   "000-1234-5678"
#define DBNAME   "book"


static t_class *dbm_class;


typedef struct _dbm {
    t_object  x_obj;
    DEPOT *depot;
    t_outlet *out;
} t_dbm;



void dbm_set(t_dbm *x, t_symbol *s, int argc, t_atom *argv) {
    post("s: %s", s->s_name);

    if (argc == 2) {

        if (argv->a_type == A_SYMBOL) {

            const char *key = atom_getsymbol(argv)->s_name;
            post("key: %s", key);

            if ((argv+1)->a_type == A_SYMBOL) {

                const char *value = atom_getsymbol(argv+1)->s_name;

                /* store the record */
                if(!dpput(x->depot, key, -1, value, -1, DP_DOVER)){
                    pd_error(x, "dpput: %s", dperrmsg(dpecode));
                    goto error;
                }

                post("set SUCCESS: %s -> %0.2f", key, value);
                return;

            } else {
                pd_error(x, "only symbol can be value");
                goto error;
            }

        } else {
            pd_error(x, "only symbol can be key");
            goto error;
        }

    } else {
        pd_error(x, "'set' can only handle two args currently");
    }

    error:
        pd_error(x, "dbm_failed FAILED");
}




void dbm_get(t_dbm *x, t_symbol *s) {
    const char *key = s->s_name;
    char *value;

    post("key: %s", key);
    
    /* retrieve the record */
    if(!(value = dpget(x->depot, key, -1, 0, -1, NULL))){
        pd_error(x, "dpget: %s", dperrmsg(dpecode));
        return;

    } else {
        post("key: %s", key);
        printf("value: %s", value);
        // free(value);
    }

    outlet_symbol(x->out, gensym(value));
}

void *dbm_new(void)
{
    t_dbm *x = (t_dbm *)pd_new(dbm_class);

    // open database
    if (!(x->depot = dpopen(DBNAME, DP_OWRITER | DP_OCREAT, -1))) {
        // FIXME: ERROR here???
        pd_error(x, "dpopen: %s", dperrmsg(dpecode));
    }

    x->out = outlet_new(&x->x_obj, &s_float);

    return (void *)x;
}


void dbm_destroy(t_dbm *x)
{
    if(!dpclose(x->depot)){
        pd_error(x, "dpclose: %s", dperrmsg(dpecode));
    }
}



void dbm_setup(void) {
    dbm_class = class_new(gensym("dbm"),
                               (t_newmethod)dbm_new,
                               (t_method)dbm_destroy,
                               sizeof(t_dbm),
                               CLASS_DEFAULT,        
                               0);               

    // class_addbmang(dbm_class, dbm_bang);
    // class_addsymbol(dbm_class, dbm_symbol);

    class_addmethod(dbm_class, (t_method)dbm_set,  gensym("set"), A_GIMME, 0);
    class_addmethod(dbm_class, (t_method)dbm_get,  gensym("get"), A_SYMBOL, 0);

}