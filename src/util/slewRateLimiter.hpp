#ifndef SLEW_RATE_LIMITER_HPP
#define SLEW_RATE_LIMITER_HPP


class SlewRateLimiter {
private:
    float target;
    float current;
    float maxChange;
public:
    SlewRateLimiter(float maxChange);
    float slew(float target, float current);
};


#endif // SLEW_RATE_LIMITER_HPP