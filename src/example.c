#include <stdio.h>
#include "monads.h"
#include "functor.h"
#include "callable.h"

DefineCallable(double);
DefineCallable(int);

DefineResult(double, char *);
DefineCallable(Result_double);

DefineOption(double);
DefineCallable(Option_double);

FUNCTION(double, divide, double)
    (self.argc == 1) ? arg : arg / self.yield
FUNCTION_END(double, divide)

FUNCTION(Result_double, divide_res_pure, double)
    (self.argc == 1) ?
        Ok(Result_double, arg) :
        (self.yield.body.ok != 0) ?
            Ok(Result_double, CALL(divide, arg, self.yield.body.ok).yield) :
            Err(Result_double, "Divide by zero")
FUNCTION_END(Result_double, divide_res_pure)

FUNCTION(Option_double, checked_div_pure, double)
    (self.argc == 1) ?
        Some(Option_double, arg) :
        (self.yield.some != 0) ?
            Some(Option_double, CALL(divide, arg, self.yield.some).yield) :
            None(Option_double)
FUNCTION_END(Option_double, checked_div_pure)

FUNCTION(int, sub, int)
    arg - self.yield
FUNCTION_END(int, sub)

FUNCTION(int, add, int)
    arg + self.yield
FUNCTION_END(int, add)

FUNCTION(double, mult, double)
    (self.argc == 1) ? arg : self.yield * arg
FUNCTION_END(double, mult)

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
} Vec3Int; Vec3Int fmap_v3int(Vec3Int self, Callable_int function) {
    return (Vec3Int){.vector = {
        [0] = CALL(function, self.vector[0]).yield,
        [1] = CALL(function, self.vector[1]).yield,
        [2] = CALL(function, self.vector[2]).yield,
    }, .fmap = &fmap_v3int};
}

void test_result() {
    puts("Tests for result");
    Result_double r = CALL(divide_res_pure, 5.0, 2.0).yield;
    r.self ?
        printf("%.1f\n", r.body.ok) :
        printf("%s\n", r.body.err);

    r = CALL(divide_res_pure, 5.0, 0.0).yield;
    r.self ?
        printf("%.1f\n", r.body.ok) :
        printf("%s\n", r.body.err);
}

void test_option() {
    puts("Tests for Option");
    Option_double o = CALL(checked_div_pure, 5.0, 0.0).yield;
    o.self ?
        printf("Some(%.1f)\n", o.some):
        printf("None\n");

    o = CALL(checked_div_pure, 5.0, 2.0).yield;
    o.self ?
        printf("Some(%.1f)\n", o.some):
        printf("None\n");

    o = CALL(checked_div_pure, 15.0, 3.0).yield;
    o.self ?
        printf("Some(%.1f)\n", o.some):
        printf("None\n");

    puts("");
}

void test_add_sub() {
    FunctorInt test = {.x = 2, .fmap = &fmap_int};
    puts("Tests for add");
    printf("%d\n", MAP(CALL(add, 5, 2, 3), test).x);
    printf("%d\n", MAP(CALL(add, 5, 2, 3, 4), test).x);
    puts("");

    puts("Tests for sub");
    printf("%d\n", MAP(CALL(sub, 5), test).x);
    printf("%d\n", CALL(sub, 4, 6).yield);
    puts("");
}

void test_mult_div() {
    puts("Tests for mult and div");
    printf("%.2f\n", CALL(mult, 5.0, 2.0).yield);
    printf("%.2f\n", CALL(divide, 24.0, 6.0).yield);
    puts("");
}

void test_vec() {
    puts("Tests for vec");
    Vec3Int vec = {.vector = {1, 2, 3}, .fmap = &fmap_v3int};
    vec = MAP(CALL(add, 2), vec);
    printf("[%d, %d, %d]\n", vec.vector[0], vec.vector[1], vec.vector[2]);

    vec = MAP(CALL(sub, 2), vec);
    printf("[%d, %d, %d]\n", vec.vector[0], vec.vector[1], vec.vector[2]);
    puts("");
}

int main() {
    test_add_sub();
    test_mult_div();
    test_vec();
    test_option();
    test_result();
}
