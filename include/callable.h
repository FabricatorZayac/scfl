#ifndef CALLABLE_H_
#define CALLABLE_H_

// get number of arguments with __NARG__
#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
      9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// general definition for any function name
#define _VFUNC_(name, n) _##name##_##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

// definition for FOO
// #define FOO(...) VFUNC(FOO, __VA_ARGS__)

#define CALL(callable, ...) _VFUNC(CALL, __NARG__(__VA_ARGS__)) (__VA_ARGS__, callable)

#define _CALL(x1, callable) callable.function(callable, x1)

#define _CALL_1(x1, ...) _CALL(x1, __VA_ARGS__)
#define _CALL_2(x2, ...) _CALL(x2, _CALL_1(__VA_ARGS__))
#define _CALL_3(x3, ...) _CALL(x3, _CALL_2(__VA_ARGS__))
#define _CALL_4(x4, ...) _CALL(x4, _CALL_3(__VA_ARGS__))
#define _CALL_5(x5, ...) _CALL(x5, _CALL_4(__VA_ARGS__))
#define _CALL_6(x6, ...) _CALL(x6, _CALL_5(__VA_ARGS__))
#define _CALL_7(x7, ...) _CALL(x7, _CALL_6(__VA_ARGS__))
#define _CALL_8(x8, ...) _CALL(x8, _CALL_7(__VA_ARGS__))

#define DefineCallable(T)                       \
typedef struct Callable_##T {                   \
    struct Callable_##T (*function)();          \
    short argc;                                 \
    T yield;                                    \
} Callable_##T

#define FUNCTION(Ret_T, name, Arg_T)                            \
Callable_##Ret_T _##name(Callable_##Ret_T self, Arg_T arg) {    \
    return (Callable_##Ret_T){                                  \
        .argc = ++self.argc,                                    \
        .function = _##name,                                    \
        .yield =

#define FUNCTION_END(Ret_T, name) };}           \
static const Callable_##Ret_T name = {          \
    .function = _##name,                        \
    .argc = 0                                   \
};

#endif // CALLABLE_H_
