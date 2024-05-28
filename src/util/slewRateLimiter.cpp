class SlewRateLimiter {
private:
    float target;
    float current;
    float maxChange;
public:
    SlewRateLimiter(float maxChange) : maxChange(maxChange) {
        // Initialize target and current to 0
        target = 0.0f;
        current = 0.0f;
    }
    float slew(float target, float current) {
        // Update the target and current values
        this->target = target;
        this->current = current;
        // Calculate the change required to reach the target
        float change = target - current;
        // Limit the change to the maximum allowed change
        if (change > maxChange) {
            change = maxChange;
        } else if (change < -maxChange) {
            change = -maxChange;
        }
        // Update the current value based on the change
        current += change;
        return current;
    }
};
