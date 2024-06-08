import rclpy
from rclpy.node import Node

from std_msgs.msg import String,Float32

from coppeliasim_zmqremoteapi_client import RemoteAPIClient

class coppelia:

    def __init__(self):

        client = RemoteAPIClient()
        sim = client.require('sim')
        self.sim = sim

        self.base_handle = sim.getObject('/Base')

        self.left_joint_handle = sim.getObject('/Left_joint')
        self.right_joint_handle = sim.getObject('/Right_joint')

        self.left_TOF_handle =  sim.getObject('/Left_TOF')
        self.front_TOF_handle =  sim.getObject('/Front_TOF')
        self.right_TOF_handle =  sim.getObject('/Right_TOF') 
        self.back_TOF_handle =  sim.getObject('/Back_TOF') 

        self.data_list = ['left_wheel_angle',
                          'right_wheel_angle',
                          'yaw_rate_reading',
                          'front_TOF_reading',
                          'left_TOF_reading',
                          'right_TOF_reading',
                          'back_TOF_reading']

    def get_data(self):
        left_wheel_jointAngle = self.sim.getJointPosition(self.left_joint_handle)
        right_wheel_jointAngle = self.sim.getJointPosition(self.right_joint_handle)

        linear_velocity, angular_velocity = self.sim.getObjectVelocity(self.base_handle)
        yaw_rate = angular_velocity[2]

        res,left_distance,_,_,_ = self.sim.readProximitySensor(self.left_TOF_handle)
        if res == -1:
            left_distance = float(-1)

        res,right_distance,_,_,_ = self.sim.readProximitySensor(self.right_TOF_handle)
        if res == -1:
            right_distance = float(-1)

        res,front_distance,_,_,_ = self.sim.readProximitySensor(self.front_TOF_handle)
        if res == -1:
            front_distance = float(-1)

        res,back_distance,_,_,_ = self.sim.readProximitySensor(self.back_TOF_handle)
        if res == -1:
            back_distance = float(-1)

        data = {'left_wheel_angle' : left_wheel_jointAngle,
                'right_wheel_angle' : right_wheel_jointAngle,
                'yaw_rate_reading' : yaw_rate,
                'front_TOF_reading' : front_distance,
                'left_TOF_reading' : left_distance,
                'right_TOF_reading' : right_distance,
                'back_TOF_reading' : back_distance,
        }

        return data

    
    def set_left_wheel_speed(vel_left):
        self.sim.setJointTargetVelocity(self.left_joint_handle,vel_left)

    def set_right_wheel_speed(vel_right):
        self.sim.setJointTargetVelocity(self.right_joint_handle,vel_right)


class MinimalPublisher(Node):

    def __init__(self,coppelia_obj):
        super().__init__('minimal_publisher')

        self.coppelia_obj = coppelia_obj
        #self.publisher_ = self.create_publisher(String, 'topic', 10)
        self.publisher = [self.create_publisher(Float32, topic, 10) for topic in self.coppelia_obj.data_list]
        timer_period = 0.01  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    def timer_callback(self):
        msg_list = {topic : Float32() for topic in self.coppelia_obj.data_list}
        data = self.coppelia_obj.get_data()

        counter = 0
        for topic,msg in msg_list.items():
            msg.data = data[topic]
            self.publisher[counter].publish(msg)
            self.get_logger().info('Publishing in topic name %s : %s ' % (topic,msg.data))
            self.i += 1
            counter += 1


class MinimalSubscriber(Node):

    def __init__(self,coppelia_obj):
        super().__init__('minimal_subscriber')
        
        '''
        self.subscription = self.create_subscription(
            String,
            'topic',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        '''

        self.subscription = self.create_subscription(
            Float32,
            'set_left_wheel_velocity',
            self.listener_callback_for_left_wheel,
            10)
        self.subscription  # prevent unused variable warning

        self.subscription = self.create_subscription(
            Float32,
            'set_right_wheel_velocity',
            self.listener_callback_for_right_wheel,
            10)
        self.subscription  # prevent unused variable warning

    def listener_callback_for_left_wheel(self, msg):
        #self.get_logger().info('I heard: "%s"' % msg.data)
        self.coppelia_obj.set_left_wheel_speed(msg.data)

    def listener_callback_for_right_wheel(self, msg):
        #self.get_logger().info('I heard: "%s"' % msg.data)
        self.coppelia_obj.set_right_wheel_speed(msg.data)

    
def main(args=None):

    rclpy.init(args=args)
    coppelia_obj = coppelia()

    minimal_publisher = MinimalPublisher(coppelia_obj)
    minimal_subscriber = MinimalSubscriber(coppelia_obj)

    rclpy.spin(minimal_publisher)
    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()