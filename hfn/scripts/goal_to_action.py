#! /usr/bin/env python

import roslib; roslib.load_manifest('hfn')
import rospy
import actionlib

from std_msgs.msg import Empty
from scarab_msgs.msg import MoveAction, MoveGoal
from scarab_msgs.msg import ScarabState
from geometry_msgs.msg import PoseStamped
import tf.transformations as tft
import math

class GoalToAction ():

    def __init__ (self):
        rospy.init_node('move_client')

        self.client = actionlib.SimpleActionClient('move', MoveAction)
        rospy.loginfo("Waiting for server")
        self.client.wait_for_server()
        rospy.loginfo("Connected!")

        self.set_height = rospy.get_param("~z", None)
        self.yaw_adjust = rospy.get_param("~yaw_adjust", 0.0)

        # Create a publisher to keep track of the current state of the robot
        # This will basically reflect HFN, and will let listeners know if
        # the robot is
        #    IDLE   - Not navigating, just waiting for a goal
        #    BUSY   - Currently navigating
        #    STUCK  - Got stuck while navigating. Failed.
        #    FAILED - Failed while navigating for any other reason
        self.state_pub = rospy.Publisher('state', ScarabState, latch=True, queue_size=1)

        rospy.Subscriber("goal", PoseStamped, self.callback)
        rospy.Subscriber("cancel", Empty, self.cancel_callback)
        rospy.spin()


    def callback (self, data):
        goal = MoveGoal()
        rospy.loginfo("Got goal to %s" % data)

        if self.set_height is not None and math.fabs(data.pose.position.z) < 1e-3:
            data.pose.position.z = self.set_height

        rot = tft.numpy.array([data.pose.orientation.x,
                               data.pose.orientation.y,
                               data.pose.orientation.z,
                               data.pose.orientation.w])
        adjust = tft.quaternion_from_euler(0.0, 0.0, self.yaw_adjust)
        orientation = tft.quaternion_multiply(rot, adjust)
        data.pose.orientation.x = orientation[0]
        data.pose.orientation.y = orientation[1]
        data.pose.orientation.z = orientation[2]
        data.pose.orientation.w = orientation[3]

        goal.target_poses.append(data)
        self.client.send_goal(goal)
	'''
        while True:
            scarab_state = ScarabState()
            ret = self.client.wait_for_result(rospy.Duration.from_sec(1.0))
            if ret:
                # Goal finished
                result = self.client.get_result().final_status
                state = self.client.get_state()
                if state == 3 and result == 0:
                    # STATE = 3: SUCCEEDED
                    # RESULT = 0: FINISHED
                    # We are back in the IDLE state
                    scarab_state.state = 'IDLE'
                elif state != 3 and result == 2:
                    # Check for the stuck case
                    scarab_state.state = 'STUCK'
                else:
                    # Any other failure just group
                    scarab_state.state = 'FAILED'

                self.state_pub.publish(scarab_state)
                break

            else:
                # Goal not finished, still waiting
                scarab_state.state = 'BUSY'
                self.state_pub.publish(scarab_state)
	'''

    def cancel_callback (self, data):
        self.client.cancel_goal()

if __name__ == '__main__':
    gta = GoalToAction()

