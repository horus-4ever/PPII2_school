#ifndef STRUCTURES_COMMON_H
#define STRUCTURES_COMMON_H

#include <stddef.h>

typedef void(*deleter_f)();
typedef size_t(*hash_f)();
typedef bool(*compare_f)();

#endif