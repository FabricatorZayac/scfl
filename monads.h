#ifndef MONADS_H_
#define MONADS_H_

#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#define New(T, o) memcpy(malloc(sizeof(T)), &o, sizeof(T))

enum option {None, Some};
#define Option(T) struct { enum option self; T some; }
#define Some(O, t) New(O, ((O){Some, .some = t}))
#define None(O) New(O, ((O){None}))

enum result {Err, Ok};
#define Result(T, E) struct { enum result self; union {T ok; E err;} body; }
#define Ok(R, t) New(R, ((R){Ok, {.ok = t}}))
#define Err(R, e) New(R, ((R){Err, {.err = e}}))

#endif // MONADS_H_
