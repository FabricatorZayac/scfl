#include <stdio.h>
#include "monads.h"
#include "functor.h"
#include "callable.h"

DefineCallable(double);

DefineResult(double, char *);
Result_double divide_res(double x, double y) {
    return y == 0 ?
        Err(Result_double, "Divide by zero"):
        Ok(Result_double, x / y);
}

void test_result() {
    Result_double r = divide_res(5.0, 2.0);
    r.self ?
        printf("%f\n", r.body.ok):
        printf("%s\n", r.body.err);

    r = divide_res(5.0, 0.0);
    r.self ?
        printf("%f\n", r.body.ok):
        printf("%s\n", r.body.err);
}

DefineOption(double);
Option_double checked_div(double x, double y) {
    return y != 0 ?
        Some(Option_double, x / y):
        None(Option_double);
}

void test_option() {
    Option_double o = checked_div(5.0, 0);
    o.self ?
        printf("Some(%f)\n", o.some):
        printf("None\n");

    o = checked_div(5.0, 2);
    o.self ?
        printf("Some(%f)\n", o.some):
        printf("None\n");
}

DefineCallable(int);
FUNCTION(int, sub)
    .yield = x - self.yield,
    .function = _sub
FUNCTION_END(int, sub)

FUNCTION(int, add)
    .yield = x + self.yield,
    .function = _add
FUNCTION_END(int, add)

typedef struct FunctorInt {
    struct FunctorInt (*fmap)();
    int x;
} FunctorInt;
FunctorInt fmap_int(FunctorInt self, Callable_int function) {
    return (FunctorInt){.x = CALL(function, self.x).yield, .fmap = &fmap_int};
}

typedef struct Vec3Int {
    struct Vec3Int (*fmap)();
    int vector[3];
} Vec3Int;
Vec3Int fmap_v3int(Vec3Int self, Callable_int function) {
    return (Vec3Int){.vector = {
        [0] = CALL(function, self.vector[0]).yield,
        [1] = CALL(function, self.vector[1]).yield,
        [2] = CALL(function, self.vector[2]).yield,
    }, .fmap = &fmap_v3int};
}

int main() {
    FunctorInt test = {.x = 2, .fmap = &fmap_int};

    printf("%d\n", MAP(CALL(add, 5, 2, 3), test).x);
    printf("%d\n", MAP(CALL(add, 5, 2, 3, 4), test).x);

    printf("%d\n", MAP(CALL(sub, 5), test).x);
    printf("%d\n", CALL(sub, 4, 6).yield);


    Vec3Int vec = {.vector = {1, 2, 3}, .fmap = &fmap_v3int};
    vec = MAP(CALL(add, 2), vec);
    printf("[%d, %d, %d]\n", vec.vector[0], vec.vector[1], vec.vector[2]);

    vec = MAP(CALL(sub, 2), vec);
    printf("[%d, %d, %d]\n", vec.vector[0], vec.vector[1], vec.vector[2]);

    /* test_option(); */
    /* test_result(); */
}
