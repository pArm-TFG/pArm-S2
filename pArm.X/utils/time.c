#include "time.h"
#include "types.h"

volatile time_t _now_us = 0ULL;
volatile time_t _now_ms = 0ULL;

inline void _updateMs(void) {
    _now_ms = (time_t) (_now_us / 1000ULL);
}

inline time_t now(void) {
    return _now_ms;
}

inline time_t now_us(void) {
    return _now_us;
}

inline void increment_us(void) {
    _now_us += 1ULL;
    _updateMs();
}

inline void set_time(time_t value_us) {
    _now_us = value_us;
    _updateMs();
}
