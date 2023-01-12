#include <stdio.h>
#include "monads.h"
#include "functor.h"
#include "callable.h"

Result(double, char *) *divide_res(double x, double y) {
    return y == 0 ?
        Err(Result(double, char *), "Divide by zero"):
        Ok(Result(double, char *), x / y);
}

Option(double) *checked_div(double x, double y) {
    return y == 0 ?
        None(Option(double)):
        Some(Option(double), x / y);
}

/* void print_result_double(Result(double, char *) *r) { */
/*     r->self ? */
/*         printf("%f\n", r->body.ok) : */
/*         printf("%s\n", r->body.err); */
/* } */

/* void test_result() { */
/*     Result(double, char *) *r = divide_res(5.0, 2.0); */
/*     Result(double, char *) *d = divide_res(5.0, 0.0); */
/*     print_result_double(r); */
/*     print_result_double(d); */
/*     free(r); */
/*     free(d); */
/* } */

void test_option() {
    Option(double) *o = checked_div(5.0, 0);
    o->self ?
        printf("Some(%f)\n", o->some):
        printf("None\n");
    free(o);
}

typedef struct FunctorInt FunctorInt;
struct FunctorInt {
    FunctorInt (*fmap)();
    int x;
};

typedef struct CallableInt CallableInt;
struct CallableInt {
    CallableInt (*function)();
    int a;
};

CallableInt _add(CallableInt self, int x) {
    return (CallableInt){.a = self.a + x, .function = &_add};
}
static const CallableInt add = {.function = _add};

FunctorInt fmap(FunctorInt self, CallableInt function) {
    return (FunctorInt){.x = CALL(function, self.x).a, .fmap = &fmap};
}

int main() {
    FunctorInt test = {.x = 2, .fmap = &fmap};

    printf("%d\n", MAP(CALL(add, 5), test).x);
    printf("%d\n", MAP(CALL(add, 5, 2), test).x);
    printf("%d\n", MAP(CALL(add, 5, 2, 3), test).x);
    printf("%d\n", MAP(CALL(add, 5, 2, 3, 4), test).x);
}
