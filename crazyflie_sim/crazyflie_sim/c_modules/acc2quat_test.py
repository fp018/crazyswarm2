import crazyflie_sil_c
import numpy as np
import time
from scipy.spatial.transform import Rotation

p = np.array([1.4, 4.6, 0.0])
q = np.array([0.0, 0.0, 0.,1.0])
yaw =0.0

start_time = time.time()
crazyflie_sil_c.acc2quat(p, 0., q)
print("Time taken: ", time.time() - start_time)

start_time = time.time()
thrust = p + np.array([0, 0, 9.81])
z_body = thrust / np.linalg.norm(thrust)
x_world = np.array([np.cos(yaw), np.sin(yaw), 0])
y_body = np.cross(z_body, x_world)
# Mathematically not needed. This addresses numerical issues to ensure R is orthogonal
y_body /= np.linalg.norm(y_body)
x_body = np.cross(y_body, z_body)
# Mathematically not needed. This addresses numerical issues to ensure R is orthogonal
x_body /= np.linalg.norm(x_body)
R = np.column_stack([x_body, y_body, z_body])
start_time = time.time()
quat = Rotation.from_matrix(R).as_quat()
print("Time taken: ", time.time() - start_time)

print(q)
print(quat)