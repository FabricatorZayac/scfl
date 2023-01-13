#ifndef MONADS_H_
#define MONADS_H_

#include <stdlib.h>
#include <string.h>
#include "functor.h"
#include "callable.h"

enum option {None, Some};
#define DefineOption(T)                                                \
typedef struct Option_##T {                                            \
    struct Option_##T (*fmap)();                                       \
    enum option self;                                                  \
    T some;                                                            \
} Option_##T;                                                          \
Option_##T fmap_Option_##T(Option_##T self, Callable_##T function) {   \
    return (self.self) ?                                               \
        Some(Option_##T, CALL(function, self.some).yield) :            \
        None(Option_##T);                                              \
}

#define Some(O, t) (O){.self = Some, .some = t, .fmap = fmap_##O}
#define None(O) (O){.self = None}


enum result {Err, Ok};
#define DefineResult(T, E)                                             \
typedef struct Result_##T {                                            \
    struct Result_##T (*fmap)();                                       \
    enum result self;                                                  \
    union {                                                            \
        T ok;                                                          \
        E err;                                                         \
    } body;                                                            \
} Result_##T;                                                          \
Result_##T fmap_Result_##T(Result_##T self, Callable_##T function) {   \
    return (self.self) ?                                               \
        Ok(Result_##T, CALL(function, self.body.ok).yield) :           \
        Err(Result_##T, self.body.err);                                \
}

#define Ok(R, t) (R){.self = Ok, {.ok = t}, .fmap = fmap_##R}
#define Err(R, e) (R){.self = Err, {.err = e}}

#endif // MONADS_H_
