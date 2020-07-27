#include "motor.h"

static inline void handleInterrupt(void);
inline void move(Motor *motor, uint8_t angle);
inline void home(Motor motor[MAX_MOTORS]);
inline void freeze(Motor *motor);
inline time_t positionMs(Motor *motor);
inline uint8_t position(Motor *motor);
