#include <stdio.h>
#include "monads.h"

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

void print_result_double(Result(double, char *) *r) {
    r->self ?
        printf("%f\n", r->body.ok) :
        printf("%s\n", r->body.err);
}

int main() {
    Result(double, char *) *r = divide_res(5.0, 2.0);
    Result(double, char *) *d = divide_res(5.0, 0.0);

    print_result_double(r);
    print_result_double(d);

    free(r);
    free(d);

    Option(double) *o = checked_div(5.0, 0);
    o->self ?
        printf("Some(%f)\n", o->some):
        printf("None");

    free(o);
}
