#include "time.h"
#include "types.h"

volatile time_t _now_us = 0ULL;
volatile time_t _now_ms = 0ULL;

inline void TIME_updateMs(void) {
    _now_ms = (time_t) (_now_us / 1000ULL);
}

inline time_t TIME_now(void) {
    return _now_ms;
}

inline time_t TIME_now_us(void) {
    return _now_us;
}

inline void TIME_increment_us(void) {
    _now_us += 1ULL;
    TIME_updateMs();
}

inline void TIME_set_time(time_t value_us) {
    _now_us = value_us;
    TIME_updateMs();
}
