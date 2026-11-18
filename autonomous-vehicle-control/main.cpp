#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

struct State { double x, v, a; };
struct Command { double acceleration; bool emergency; };

class SafeLongitudinalController {
public:
    Command update(const State& ego, double lead_gap, double lead_speed, double dt) {
        if (!(dt > 0 && dt <= 0.2) || !std::isfinite(lead_gap) || !std::isfinite(lead_speed) || !std::isfinite(ego.v) || !std::isfinite(ego.a))
            return {-6.0, true};
        constexpr double max_brake = 6.0, max_accel = 2.0, max_jerk = 3.0;
        constexpr double time_headway = 2.0, standstill_gap = 5.0;
        const double desired_gap = standstill_gap + time_headway * std::max(0.0, ego.v);
        const double closing_speed = std::max(0.0, ego.v - lead_speed);
        const double ttc = closing_speed > 0.01 ? lead_gap / closing_speed : INFINITY;
        if (lead_gap <= 0 || ttc < 1.5) return {-max_brake, true};
        double desired_a = std::clamp(0.35 * (lead_gap - desired_gap) + 0.7 * (lead_speed - ego.v), -max_brake, max_accel);
        double smooth_a = std::clamp(desired_a, ego.a - max_jerk * dt, ego.a + max_jerk * dt);
        return {std::clamp(smooth_a, -max_brake, max_accel), false};
    }
};

void check(bool ok, const char* name) { if (!ok) throw std::runtime_error(name); std::cout << "PASS " << name << '\n'; }
int main() {
    SafeLongitudinalController c;
    auto normal = c.update({0, 15, 0}, 80, 15, 0.1);
    check(!normal.emergency && normal.acceleration >= 0 && normal.acceleration <= 0.3 + 1e-9, "smooth acceleration and jerk limit");
    auto following = c.update({0, 20, 0}, 25, 18, 0.1);
    check(!following.emergency && following.acceleration < 0, "comfortable deceleration when gap is short");
    auto hazard = c.update({0, 25, 0}, 10, 5, 0.1);
    check(hazard.emergency && hazard.acceleration == -6.0, "emergency collision mitigation");
    auto invalid = c.update({0, 10, 0}, NAN, 5, 0.1);
    check(invalid.emergency, "invalid sensor data fails safe");
    auto bad_dt = c.update({0, 10, 0}, 30, 10, 0);
    check(bad_dt.emergency, "invalid timestep fails safe");
    std::cout << "All tests passed (simulation prototype only).\n";
}
