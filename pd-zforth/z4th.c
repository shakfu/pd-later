/*

pd wrapper of zforth (https://github.com/zevv/zForth)

*/
#include "m_pd.h"

#include "zforth.h"

#define SYMBOL_LENGTH 100
// #define DEBUG(x) x
#define DEBUG(x)


static t_class *z4th_class;


typedef struct _z4th {
    t_object  x_obj;

    // zforth-specific
    int trace;
} t_z4th;

// class methods
// ---------------------------------------------------------------------------
void z4th_symbol(t_z4th *x, t_symbol *s) {
    post("s: %s", s->s_name);

    const char *msg = NULL;

    zf_result rv = zf_eval(s->s_name);

    switch(rv)
    {
        case ZF_OK: break;
        case ZF_ABORT_INTERNAL_ERROR: msg = "internal error"; break;
        case ZF_ABORT_OUTSIDE_MEM: msg = "outside memory"; break;
        case ZF_ABORT_DSTACK_OVERRUN: msg = "dstack overrun"; break;
        case ZF_ABORT_DSTACK_UNDERRUN: msg = "dstack underrun"; break;
        case ZF_ABORT_RSTACK_OVERRUN: msg = "rstack overrun"; break;
        case ZF_ABORT_RSTACK_UNDERRUN: msg = "rstack underrun"; break;
        case ZF_ABORT_NOT_A_WORD: msg = "not a word"; break;
        case ZF_ABORT_COMPILE_ONLY_WORD: msg = "compile-only word"; break;
        case ZF_ABORT_INVALID_SIZE: msg = "invalid size"; break;
        case ZF_ABORT_DIVISION_BY_ZERO: msg = "division by zero"; break;
        default: msg = "unknown error";
    }

    if(msg) {
        pd_error(x, "error: %s", msg);
    }
    return;   
}


void z4th_bang(t_z4th *x)
{
    DEBUG(post("this is a debugging statement."));

}

// class constructor, destructor and setup
// ---------------------------------------------------------------------------
void *z4th_new(void)
{
    t_z4th *x = (t_z4th *)pd_new(z4th_class);

    x->trace = 0; // set tracing on/off

    // zforth initialization
    zf_init(x->trace);
    zf_bootstrap();

    return (void *)x;
}

// void z4th_free(t_z4th *x) {
//     
// }


void z4th_setup(void) {

    z4th_class = class_new(gensym("z4th"),
                               (t_newmethod)z4th_new,
                               0, // (t_method)z4th_free, 
                               sizeof(t_z4th),
                               CLASS_DEFAULT,
                               0);

    class_addbang(z4th_class, z4th_bang);
    class_addsymbol(z4th_class, z4th_symbol);

}


// ------------------------------------------------------------------
// required by zforth

/*
zf_input_state zf_host_sys(t_z4th *x, zf_syscall_id id, const char *input)
{
    switch((int)id) {


        // The core system callbacks

        case ZF_SYSCALL_EMIT:
            putchar((char)zf_pop());
            fflush(stdout);
            break;

        case ZF_SYSCALL_PRINT:
            printf(ZF_CELL_FMT " ", zf_pop());
            break;

        case ZF_SYSCALL_TELL: {
            zf_cell len = zf_pop();
            void *buf = (uint8_t *)zf_dump(NULL) + (int)zf_pop();
            (void)fwrite(buf, 1, len, stdout);
            fflush(stdout); }
            break;


        // Application specific callbacks

        case ZF_SYSCALL_USER + 0:
            printf("\n");
            exit(0);
            break;

        case ZF_SYSCALL_USER + 1:
            zf_push(sin(zf_pop()));
            break;

        case ZF_SYSCALL_USER + 2:
            if(input == NULL) {
                return ZF_INPUT_PASS_WORD;
            }
            include(input);
            break;
        
        case ZF_SYSCALL_USER + 3:
            save("zforth.save");
            break;

        default:
            printf("unhandled syscall %d\n", id);
            break;
    }

    return ZF_INPUT_INTERPRET;
}


zf_cell zf_host_parse_num(const char *buf)
{
    zf_cell v;
    int n = 0;
    int r = sscanf(buf, "%f%n", &v, &n);
    if(r == 0 || buf[n] != '\0') {
        zf_abort(ZF_ABORT_NOT_A_WORD);
    }
    return v;
}

*/
