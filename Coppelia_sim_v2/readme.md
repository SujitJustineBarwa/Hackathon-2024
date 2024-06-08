1) Install ROS humble using the following [link](https://docs.ros.org/en/humble/Installation/Alternatives/Ubuntu-Development-Setup.html).
2) Source it :
```bash
source /opt/ros/humble/setup.bash
```
3) Make a basic publisher subscriber node using the help of this [link](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html).
4) If you want to learn how to make workspace using colcon use this [link](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html).
5) If you want to learn how to make package using colcon use this [link](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Creating-Your-First-ROS2-Package.html).
6) Once the workspace is built, copy the data_hub.py in the src folder of your package.You can refer the folder structure by unzipping the ros2_ws.zip file.
7) Then, in setup.cfg add tranmission as shown below :
   ![Setup](setup_config.png)
8) Add these lines in package.xml :
   ```bash
     <exec_depend>rclpy</exec_depend>
     <exec_depend>std_msgs</exec_depend>
   ```
   ![package](package.png)
   
9) Build the project using : 
   ```bash
     colcon build --packages-select my_package
   ```
   Note : my_package is my package's name.Also pip install coppeliasim_zmqremoteapi_client and pyzml for coppelia sim.
   
10) Load model Robot_Sim1.ttt in coppelia sim and start the topics by running :
    ```bash
      ros2 run my_package transmission
    ```
    
11) Check the topic :
    ```bash
      ros2 topic list
    ```
