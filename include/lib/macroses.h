#ifndef MACROSES_H

#define MACROSES_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR(message, error_code) { \
    printf(message);                 \
    exit(error_code);                \
}

#define ASSERT_ALLOCATED(p, message) {      \
    if ((p) == NULL) {             \
        ERROR(                     \
            message, \
            RUNTIME_ERROR          \
        );                         \
    }                              \
}

#define RUNTIME_ERROR -1
#define LOGIC_ERROR -2
#define INVALID_ARGUMENTS_ERROR -3

#endif