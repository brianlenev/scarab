#!/usr/bin/env python
#
# A node that listens for PoseWithCovarianceStamped and publishes PoseStamped
import roslib; roslib.load_manifest('scarab')
import rospy

import geometry_msgs.msg

#last_pose = None

class Stripper(object):
    def __init__(self):
        self._pub = rospy.Publisher("pose_stamped",
                                    geometry_msgs.msg.PoseStamped,
                                    latch=True)
        self._pose_sub = \
            rospy.Subscriber("amcl_pose",
                             geometry_msgs.msg.PoseWithCovarianceStamped,
                             self._pose_callback)

	# Publish where we are every 1 second
       # self.last_pose = None
#	rospy.Timer(rospy.Duration(1), self._pose_periodic_callback)


    def _pose_callback(self, pose_msg):
        pose = pose_msg.pose.pose
        msg = geometry_msgs.msg.PoseStamped(header = pose_msg.header,
                                            pose = pose)
        msg.header.frame_id = pose_msg.header.frame_id
        if (msg.header.frame_id[0] != '/'):
          msg.header.frame_id = "/" + msg.header.frame_id
        self._pub.publish(msg)
#        self.last_pose = msg

    #def _pose_periodic_callback(self, event):
#	if self.last_pose:
 #           self._pub.publish(self.last_pose)
 

def main():
    rospy.init_node('posestamped_node')
    strip = Stripper()
    rospy.spin()

if __name__ == "__main__":
    main()
