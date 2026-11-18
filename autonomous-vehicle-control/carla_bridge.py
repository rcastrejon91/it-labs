"""Drive a CARLA ego vehicle using the repository's compiled C++ controller.

Requires a running CARLA server and the matching CARLA Python package.
Example:
  cmake -S . -B build && cmake --build build
  python3 carla_bridge.py --library build/libav_controller.so --seconds 60

No simulator is launched by this script; it connects to an existing server.
"""
import argparse
import csv
import ctypes
import math
import pathlib
import time

class Output(ctypes.Structure):
    _fields_ = [("accel", ctypes.c_double), ("ttc", ctypes.c_double),
                ("emergency", ctypes.c_int)]

def load_controller(path):
    lib = ctypes.CDLL(str(pathlib.Path(path).resolve()))
    lib.av_reset.argtypes = []
    lib.av_reset.restype = None
    lib.av_step.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double,
                            ctypes.c_double, ctypes.c_int]
    lib.av_step.restype = Output
    return lib

def speed_mps(vehicle):
    v = vehicle.get_velocity()
    return math.sqrt(v.x*v.x + v.y*v.y + v.z*v.z)

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=2000)
    p.add_argument("--library", required=True)
    p.add_argument("--seconds", type=float, default=60.0)
    p.add_argument("--csv", default="carla_telemetry.csv")
    args = p.parse_args()
    if args.seconds <= 0:
        p.error("--seconds must be positive")

    import carla
    controller = load_controller(args.library)
    client = carla.Client(args.host, args.port)
    client.set_timeout(10.0)
    world = client.get_world()
    original_settings = world.get_settings()
    actors = []
    try:
        settings = world.get_settings()
        settings.synchronous_mode = True
        settings.fixed_delta_seconds = 0.05
        world.apply_settings(settings)
        blueprints = world.get_blueprint_library()
        vehicle_bp = blueprints.filter("vehicle.*")
        if not vehicle_bp:
            raise RuntimeError("No vehicle blueprints in loaded CARLA map")
        spawns = world.get_map().get_spawn_points()
        if len(spawns) < 2:
            raise RuntimeError("Map needs at least two spawn points")
        ego = None
        for spawn in spawns:
            ego = world.try_spawn_actor(vehicle_bp[0], spawn)
            if ego:
                break
        if ego is None:
            raise RuntimeError("No free ego spawn point")
        actors.append(ego)
        lead = None
        for spawn in reversed(spawns):
            candidate = world.try_spawn_actor(vehicle_bp[0], spawn)
            if candidate:
                lead = candidate
                break
        if lead is None:
            raise RuntimeError("No free lead vehicle spawn point")
        actors.append(lead)
        lead.set_autopilot(True)
        controller.av_reset()
        dt = settings.fixed_delta_seconds
        with open(args.csv, "w", newline="") as out:
            writer = csv.writer(out)
            writer.writerow(["tick", "speed_mps", "lead_speed_mps", "gap_m",
                             "accel_cmd_mps2", "ttc_s", "emergency", "sensor_valid"])
            for tick in range(math.ceil(args.seconds / dt)):
                world.tick()
                ego_tf = ego.get_transform()
                lead_tf = lead.get_transform()
                delta = lead_tf.location - ego_tf.location
                forward = ego_tf.get_forward_vector()
                longitudinal = delta.x*forward.x + delta.y*forward.y
                lateral = abs(delta.x*forward.y - delta.y*forward.x)
                # Only regard a car as a lead when ahead in approximately same lane.
                valid = int(longitudinal > 0 and lateral < 2.5)
                gap = max(0.0, longitudinal - 4.5) if valid else 1000.0
                ego_speed = speed_mps(ego)
                lead_speed = speed_mps(lead) if valid else ego_speed
                cmd = controller.av_step(ego_speed, lead_speed, gap, dt, 1)
                throttle = min(1.0, max(0.0, cmd.accel / 3.0))
                brake = min(1.0, max(0.0, -cmd.accel / 6.0))
                ego.apply_control(carla.VehicleControl(throttle=throttle,
                                                       brake=brake, steer=0.0))
                writer.writerow([tick, ego_speed, lead_speed, gap, cmd.accel,
                                 cmd.ttc, cmd.emergency, valid])
        print("Telemetry:", pathlib.Path(args.csv).resolve())
    finally:
        for actor in reversed(actors):
            if actor.is_alive:
                actor.destroy()
        world.apply_settings(original_settings)

if __name__ == "__main__":
    main()
