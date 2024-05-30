import time
import sim
import numpy as np
import matplotlib.pyplot as plt

def check_return_code(return_code, error_message):
    if return_code != sim.simx_return_ok:
        raise RuntimeError(error_message)

# Connect to CoppeliaSim
sim.simxFinish(-1)  # Close all opened connections
client_id = sim.simxStart('127.0.0.1', 19997, True, True, 5000, 5)
if client_id == -1:
    raise RuntimeError('Failed to connect to remote API server')

# Start the simulation
sim.simxStartSimulation(client_id, sim.simx_opmode_oneshot)

# Get handles for the Pioneer 3-DX and its motors
_, left_motor_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx_leftMotor', sim.simx_opmode_blocking)
_, right_motor_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx_rightMotor', sim.simx_opmode_blocking)
_, robot_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx', sim.simx_opmode_blocking)
_, floor_handle = sim.simxGetObjectHandle(client_id, 'Floor', sim.simx_opmode_blocking)

leftWheelOdom = 0
lastLeftWheelPosition = 0
rightWheelOdom = 0
lastRightWheelPosition = 0

try:
    while True:
        # Set wheel velocities
        velocity_left = 0.1  # Left wheel velocity
        velocity_right = 0.1  # Right wheel velocity
        sim.simxSetJointTargetVelocity(client_id, left_motor_handle, velocity_left, sim.simx_opmode_streaming)
        sim.simxSetJointTargetVelocity(client_id, right_motor_handle, velocity_right, sim.simx_opmode_streaming)

        # Get odometry data
        leftWheelPosition = sim.simxGetJointPosition(client_id, left_motor_handle,sim.simx_opmode_streaming)[1]
        rightWheelPosition = sim.simxGetJointPosition(client_id, right_motor_handle,sim.simx_opmode_streaming)[1]

        # Getting the Diameter
        leftWheelDiameter = \
        sim.simxGetObjectFloatParameter(client_id, left_motor_handle, 18, sim.simx_opmode_streaming)[1] \
        - sim.simxGetObjectFloatParameter(client_id, left_motor_handle, 15, sim.simx_opmode_streaming)[1]

        rightWheelDiameter = \
        sim.simxGetObjectFloatParameter(client_id, right_motor_handle , 18, sim.simx_opmode_streaming)[1] \
        - sim.simxGetObjectFloatParameter(client_id, right_motor_handle, 15, sim.simx_opmode_streaming)[1]

        # Calculating the delta theta and odometery
        dTheta = leftWheelPosition - lastLeftWheelPosition
        if dTheta > np.pi:
            dTheta -= 2*np.pi
        elif dTheta < -np.pi:
            dTheta += 2*np.pi
        leftWheelOdom += dTheta * leftWheelDiameter / 2
        lastLeftWheelPosition = leftWheelPosition

        dTheta = rightWheelPosition - lastRightWheelPosition
        if dTheta > np.pi:
            dTheta -= 2*np.pi
        elif dTheta < -np.pi:
            dTheta += 2*np.pi
        rightWheelOdom += dTheta * rightWheelDiameter / 2
        lastRightWheelPosition = rightWheelPosition

        # Ground truth for the odometry
        position = sim.simxGetObjectPosition(client_id, robot_handle, floor_handle, sim.simx_opmode_streaming)[1]
        orientation = sim.simxGetObjectOrientation(client_id, robot_handle, floor_handle, sim.simx_opmode_streaming)[1]

finally:
    # Stop the simulation
    sim.simxStopSimulation(client_id, sim.simx_opmode_oneshot)
    # Close the connection
    sim.simxFinish(client_id)