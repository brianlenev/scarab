Scarab Controllers
==================

Keyboard
--------


Joystick
--------


Scarab + PS3 Remote
-------------------

Controlling a scarab with a bluetooth PS3 remote.

### Setup the Remote


On the machine the remote is connected to (likely the robot):


### Launch the PS3 Controller


    # Create the local joy stream
    rosparam set joy_node/dev "/dev/input/js1"
    rosrun joy joy_node

    # Control the Scarab
    roslaunch scarab_twist ps3.launch



### Using the PS3 Controller

- Left joystick: drive the robot in a very slow mode. This is good for mapping.
- Right joystick: drive the robot as fast as it will go.
- Select: toggle between joystick drive and accelerometer drive.
- △: Full speed ahead in the slow mode.
- ✕: Full speed ahead in the fast mode.
