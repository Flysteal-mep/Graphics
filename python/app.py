# import SkyLands as sky
# import time

# def MsPerFrame():
#     global frames, last_time

#     frames += 1
#     now = time.perf_counter()

#     if now - last_time >= 1.0:
#         ms_per_frame = (now - last_time) * 1000 / frames
#         print(f"Frame time: {ms_per_frame:.2f} ms")
#         frames = 0
#         last_time = now
# frames = 0
# last_time = time.perf_counter()


# # float data[6] =
# # {
# #      0.0,  0.5,
# #     -0.5, -0.5,
# #      0.5, -0.5
# # };



# def Run():
#     MsPerFrame()
#     sky.SetBackground(0.1, 0.1, 0.1, 0.1)
