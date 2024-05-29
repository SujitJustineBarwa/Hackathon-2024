import sim
import numpy as np
import matplotlib.pyplot as plt
import time

class bot:
    def __init__(self,client_id):
        # Get handles for the Pioneer 3-DX and its motors
        self.client_id = client_id
        _, self.left_motor_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx_leftMotor', sim.simx_opmode_blocking)
        _, self.right_motor_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx_rightMotor', sim.simx_opmode_blocking)
        _, self.robot_handle = sim.simxGetObjectHandle(client_id, 'Pioneer_p3dx', sim.simx_opmode_blocking)
        _, self.floor_handle = sim.simxGetObjectHandle(client_id, 'Floor', sim.simx_opmode_blocking)

        # Bot properties
        # Wheel Diameter
        self.leftWheelDiameter = \
        sim.simxGetObjectFloatParameter(client_id, self.left_motor_handle, 18, sim.simx_opmode_streaming)[1] \
        - sim.simxGetObjectFloatParameter(client_id, self.left_motor_handle, 15, sim.simx_opmode_streaming)[1]

        self.rightWheelDiameter = \
        sim.simxGetObjectFloatParameter(client_id, self.right_motor_handle , 18, sim.simx_opmode_streaming)[1] \
        - sim.simxGetObjectFloatParameter(client_id, self.right_motor_handle, 15, sim.simx_opmode_streaming)[1]

        _, left_wheel_pos = sim.simxGetObjectPosition(client_id, self.left_motor_handle, -1, sim.simx_opmode_blocking)  
        _, right_wheel_pos = sim.simxGetObjectPosition(client_id, self.right_motor_handle, -1, sim.simx_opmode_blocking)

        # Assuming the wheels are aligned along the X-axis in the robot's local frame
        self.wheelbase = abs(right_wheel_pos[0] - left_wheel_pos[0])


        # Odometery varaible initialization
        self.leftWheelOdom = 0
        self.lastLeftWheelPosition = 0
        self.rightWheelOdom = 0
        self.lastRightWheelPosition = 0
        self.current_pos = np.array([0.0,0.0,0.0]).T
        self.coordinates = []

        # Plotting
        plt.figure()

    def set_wheel_speed(self,leftwheelspeed,rightwheelspeed):
        sim.simxSetJointTargetVelocity(self.client_id, self.left_motor_handle, leftwheelspeed, sim.simx_opmode_streaming)
        sim.simxSetJointTargetVelocity(self.client_id, self.right_motor_handle, rightwheelspeed, sim.simx_opmode_streaming)

    def get_wheel_anglular_position(self):
        self.leftWheelAngPos = sim.simxGetJointPosition(self.client_id, self.left_motor_handle,sim.simx_opmode_streaming)[1]
        self.rightWheelAngPos = sim.simxGetJointPosition(self.client_id, self.right_motor_handle,sim.simx_opmode_streaming)[1]
        return self.leftWheelAngPos,self.rightWheelAngPos
    
    def get_gyro(self):
        self.orientation = sim.simxGetObjectOrientation(self.client_id, self.robot_handle, self.floor_handle, sim.simx_opmode_streaming)[1]
        return self.orientation

    # Calculating the delta theta and odometery
    def pose_estimation(self,track_path=False):

        # Gettingn the angular rotation of wheels
        self.get_wheel_anglular_position()
        dTheta = self.leftWheelAngPos - self.lastLeftWheelPosition
        if dTheta > np.pi:
            dTheta -= 2*np.pi
        elif dTheta < -np.pi:
            dTheta += 2*np.pi
        self.leftWheelOdom += dTheta * self.leftWheelDiameter / 2
        self.left_wheel_tangential_vel = dTheta * self.leftWheelDiameter / 2      # radius of wheel into wheel's rotation
        self.lastLeftWheelPosition = self.leftWheelAngPos 

        dTheta = self.rightWheelAngPos - self.lastRightWheelPosition
        if dTheta > np.pi:
            dTheta -= 2*np.pi
        elif dTheta < -np.pi:
            dTheta += 2*np.pi
        self.rightWheelOdom += dTheta * self.rightWheelDiameter / 2
        self.right_wheel_tangential_vel = dTheta * self.rightWheelDiameter / 2
        self.lastRightWheelPosition = self.rightWheelAngPos

        self.V_x = (self.left_wheel_tangential_vel + self.right_wheel_tangential_vel)/2
        self.omega = (self.left_wheel_tangential_vel - self.right_wheel_tangential_vel)*self.rightWheelDiameter/self.wheelbase    # Assuming both the diameters are same
        self.body_frame_vel_vec = np.array([self.V_x,0,self.omega]).transpose()

        theta = self.get_gyro()[2]
        self.world_frame_vel_vec = np.array([self.V_x*np.cos(theta),self.V_x*np.sin(theta),self.omega]).transpose()

        self.current_pos += self.world_frame_vel_vec

        if track_path:
            self.coordinates.append(self.current_pos)

    def visualize(self):
        print(self.current_pos)