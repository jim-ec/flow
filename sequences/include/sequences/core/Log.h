//
// Created by jim on 11/22/19.
//

#pragma once

#include <cstdio>

#ifdef ENABLE_LOGGING
constexpr bool enable_logs = true;
#else
constexpr bool enable_logs = false;
#endif

inline void
log(char const *const format, ...)
{
    va_list ap;
    va_start(ap, format);
    if constexpr (enable_logs) {
        vprintf(format, ap);
        putc('\n', stdout);
    }
    va_end(ap);
}
