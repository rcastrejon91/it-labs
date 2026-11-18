# Autonomous Vehicle Control Validation Lab (C++17)

Educational portfolio project implementing a longitudinal controller in **C++17** with unit tests, deterministic scenario simulations, KPI telemetry exports, an incident-analysis template, and a *proposed* on-vehicle validation plan.

## Deliverables mapped to requirements

| Responsibility | Artifact | Status |
|---|---|---|
| Control subsystem KPIs | `tests/scenario_runner.cpp` generates CSV: speed, lead gap, acceleration, jerk, TTC, emergency intervention, collision | Implemented; run locally to produce measurements |
| Develop/test C/C++ control | `controller.cpp` C++17 longitudinal acceleration and emergency braking | Implemented prototype |
| Unit and simulation tests | `tests/control_tests.cpp`, `tests/scenario_runner.cpp`, CTest | Implemented; scenario outcomes are not independently validated |
| Coordinate on-vehicle tests | `docs/on_vehicle_validation.md` | Proposed only; **no vehicle testing** |
| Diagnose issues via May Field Response | `docs/field_response.md` | Illustrative investigation template, not employer's internal procedure |

## Build and run
```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/scenario_runner metrics.csv
```

The scenario runner emits a per-timestep CSV suitable for plotting KPI trends and investigating events.

## Browser demo
`index.html` loads `controller.cpp` compiled to WebAssembly via GitHub Actions. It **requires** the Pages build to complete and Pages to be enabled in repository settings. The browser view is a simplified longitudinal simulator, not a vehicle dynamics engine.

## Safety and scope
Not production-ready, not safety-certified, not state-of-the-art by demonstrated benchmark, and not integrated with any real vehicle. The current prototype does not implement lateral steering, perception, planning, or actual on-vehicle testing. Emergency braking uses simplified TTC logic and does not guarantee collision avoidance. This project was built with AI assistance.
