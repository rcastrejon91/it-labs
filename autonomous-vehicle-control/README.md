# Autonomous Vehicle Control Lab

An educational **C++17 longitudinal autonomous-vehicle control prototype** with an interactive browser visualization. Designed as a portfolio demonstration of robotics control concepts.

## Explore
- [Interactive demo source](./index.html) — open locally in any browser (GitHub Pages hosting is not configured here)
- [C++ controller and five executable checks](./main.cpp)

## Features
- Adaptive cruise / following-gap regulation
- Time-to-collision emergency braking
- Acceleration clamp (−6 to +2 m/s²)
- Jerk-limited normal acceleration changes (3 m/s³)
- Fail-safe braking for invalid input or timestep

## Build and test
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o av_control
./av_control
```

The five tests cover smooth acceleration, short-gap deceleration, emergency braking, invalid sensor input, and invalid timestep.

## Limitations
The HTML visualization independently mirrors the controller math; it does not run the C++ binary. Slider adjustments are individual snapshots with acceleration history initialized to zero. No full physics engine, vehicle dynamics, or validated collision avoidance is implemented. **Not suitable for deployment on real vehicles or safety-critical systems.**

Created as a learning/portfolio prototype with AI assistance; it does not represent professional autonomous-vehicle software engineering experience.
