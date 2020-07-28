#include "time.h"

static inline void updateMs(void) {
    now_ms = (time_t) (now_us / 1000UL);
}

inline time_t now(void) {
    return now_ms;
}

inline time_t nowUs(void) {
    return now_us;
}

inline void increment(void) {
    ++now_us;
    updateMs();
}

inline void set(time_t value_us) {
    now_us = value_us;
    updateMs();
}
