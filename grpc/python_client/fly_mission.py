#!/usr/bin/env python


from __future__ import print_function
import grpc
from threading import Thread
import time
import dronecore_pb2 as dc
# import dronecore_pb2_grpc
import action_pb2 as dc_action
import action_pb2_grpc
import mission_pb2 as dc_mission
import mission_pb2_grpc


thread_status = True


def wait_func(future_status):
    global thread_status
    ret = future_status.result()
    print(ret.result_str)
    thread_status = False


def run():
    global thread_status
    channel = grpc.insecure_channel('0.0.0.0:50051')
    # stub = dronecore_pb2_grpc.DroneCoreRPCStub(channel)
    action_stub = action_pb2_grpc.ActionRPCStub(channel)
    mission_stub = mission_pb2_grpc.MissionRPCStub(channel)

    mission_items = []
    mission_items.append(dc_mission.MissionItem(latitude_deg=47.398170327054473,
                                        longitude_deg=8.5456490218639658,
                                        relative_altitude_m=10,
                                        speed_m_s=2,
                                        is_fly_through=True,
                                        gimbal_pitch_deg=0,
                                        gimbal_yaw_deg=-60,
                                        camera_action=dc_mission.MissionItem.NONE))
    mission_items.append(dc_mission.MissionItem(latitude_deg=47.398241338125118,
                                        longitude_deg=8.5455360114574432,
                                        relative_altitude_m=10,
                                        speed_m_s=2,
                                        is_fly_through=True,
                                        gimbal_pitch_deg=0,
                                        gimbal_yaw_deg=-60,
                                        camera_action=dc_mission.MissionItem.NONE))

    mission_stub.SendMission(dc_mission.Mission(mission_items=mission_items))
    time.sleep(1)

    action_stub.Arm(dc_action.ActionEmpty())
    time.sleep(1)

    future_status = mission_stub.StartMission.future(dc_mission.MissionEmpty())
    t = Thread(target=wait_func, args=(future_status,))
    t.start()

    while(thread_status):
        print("Waiting for thread to exit")
        time.sleep(5)


if __name__ == '__main__':
    run()
