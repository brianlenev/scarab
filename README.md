# scarab
ROS code for UPenn's scarab robot

Command used to launch the right settings:
```
roslaunch scarab dop.launch robot:=lucy map_file:=dop.yaml
```

The initial location was set to be 545K, point [0, 1] on the map.  
You can set it to other values in `scarab_ws/src/scarab/scarab/dop.launch`.

`dop.launch` was based on `umich.launch`.
