import sim
from localization import bot

# Connect to CoppeliaSim
sim.simxFinish(-1)  # Close all opened connections
client_id = sim.simxStart('127.0.0.1', 19997, True, True, 5000, 5)
if client_id == -1:
    raise RuntimeError('Failed to connect to remote API server')

# Start the simulation
sim.simxStartSimulation(client_id, sim.simx_opmode_oneshot)

# Initialize the bot
robot = bot(client_id)

while True:
    robot.set_wheel_speed(1,0.1)
    robot.pose_estimation()
    #print(robot.get_gyro())
    robot.visualize()