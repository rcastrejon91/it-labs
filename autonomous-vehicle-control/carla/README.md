# CARLA integration status

CARLA native simulator is **not running or deployed**. The previous browser replay is not CARLA.

This directory tracks the real CARLA integration. A GPU-hosted CARLA server and matching client must be provisioned before a live link can be verified. The existing Python runner is not yet wired to the C++ controller, so no C++-driven CARLA result is claimed.

Next integration acceptance checks:
1. CARLA server responds on TCP 2000.
2. Ego vehicle spawns in a 3D map and receives control commands.
3. C++ controller outputs are used to command the vehicle (not duplicated Python logic).
4. Scenario telemetry and failures are captured and replayable.
5. A remote interactive viewer URL is tested in a browser.
