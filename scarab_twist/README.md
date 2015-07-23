Scarab Controllers
==================

Keyboard
--------
(key_to_twist)

### Start a launch file

    ROS_MASTER_URI=http://<roscore IP address>:11311/ ROS_IP=<robot IP address> ROS_NAMESPACE=<namespace> roslaunch scarab <launch file>

### Run the keyboard controller

    ROS_MASTER_URI=http://<roscore IP address>:11311/ ROS_IP=<robot IP address> ROS_NAMESPACE=<namespace> rosrun scarab_twist key_to_twist <namespace>/cmd_vel

The scarab launch files all nest topics at `/<namespace>/<namespace>/<topic>`,
so the key_to_twist app needs to do so also. It could instead set
`ROS_NAMESPACE=<namespace>/<namespace>` and then include no argument for
key_to_twist.


Joystick
--------
(joy_to_twist)

Who has a joystick?


Scarab + PS3 Remote
-------------------
(ps3_to_twist)

Controlling a scarab with a bluetooth PS3 remote. Needs to post to a rostopic,
but can be connected to either the robot or another computer running ROS. For
the robot to work, you must attach a USB BLE dongle.

### Setup the Remote

    # install QtSixA
    sudo add-apt-repository ppa:falk-t-j/qtsixa
    sudo apt-get update
    sudo apt-get install qtsixa

    # form initial connection
    # plug controller into computer with usb
    sixpair

    # disconnect controller from usb
    sixad -s

    # press and hold the PS button to create a connection

(http://askubuntu.com/questions/409761/how-do-i-use-a-ps3-sixaxis-controller-with-ubuntu-to-control-games)

    # enable connections at boot
    sixad --boot-yes

    # press and hold the PS button to create a connection whenever


In order to test the connection, use `jstest`.

### Start a launch file

    ROS_MASTER_URI=http://<roscore IP address>:11311/ ROS_IP=<robot IP address> ROS_NAMESPACE=<namespace> roslaunch scarab <launch file>

### Run the PS3 Controller


    # Locate the device for ROS
    rosparam set joy_node/dev "/dev/input/js0"

    # Control the Scarab
    ROS_MASTER_URI=http://<roscore IP address>:11311/ ROS_IP=<robot IP address> ROS_NAMESPACE=<namespace> roslaunch scarab_twist ps3.launch


### Using the PS3 Controller

- Left joystick: drive the robot in a very slow mode. This is good for mapping.
- Right joystick: drive the robot as fast as it will go.
- Select: toggle between joystick drive and accelerometer drive (fast mode).
- △: Full speed ahead in the slow mode.
- ✕: Full speed ahead in the fast mode.
- Left and Right Triggers: Fire missiles. (work in progress)

