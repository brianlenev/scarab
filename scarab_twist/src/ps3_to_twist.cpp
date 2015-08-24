#include <iostream>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

using namespace std;

geometry_msgs::Twist twist;
ros::Publisher pub;
ros::NodeHandle *node;
double max_v_slow = 0.0;
double max_w_slow = 1.0;
double max_v_fast = 0.0;
double max_w_fast = 1.0;

// Mappings as returned by ros_joy
#define BUTTON_SELECT 0
#define BUTTON_LEFT_JOY 1
#define BUTTON_RIGHT_JOY 2
#define BUTTON_START 3
#define BUTTON_UP 4
#define BUTTON_RIGHT 5
#define BUTTON_DOWN 6
#define BUTTON_LEFT 7
#define BUTTON_L2 8
#define BUTTON_R2 9
#define BUTTON_L1 10
#define BUTTON_R1 11
#define BUTTON_TRIANGLE 12
#define BUTTON_CIRCLE 13
#define BUTTON_X 14
#define BUTTON_SQUARE 15
#define BUTTON_PLAYSTATION 16

#define LEFT_JOY_IDX_LINEAR 1
#define LEFT_JOY_IDX_ANGULA 0
#define RIGHT_JOY_IDX_LINEAR 3
#define RIGHT_JOY_IDX_ANGULA 2
#define ACCEL_IDX_LINEAR 5
#define ACCEL_IDX_ANGULA 4

// Use the joysticks or the accelerometer
typedef enum {
 STATE_JOYSTICKS,
 STATE_ACCEL
} state_e;

state_e state = STATE_JOYSTICKS;

// Keep track of whether select was pushed last time
int last_select = 0;


int linear_axis;
int angular_axis;
bool flip_angular;

void joy_callback(const sensor_msgs::Joy::ConstPtr& msg)
{
  // Goal: set these two values
  double v = 0.0;
  double w = 0.0;

  // Start by checking select
  if (last_select == 0 && msg->buttons[BUTTON_SELECT] == 1) {
    // Select was pushed. Change control method.
    if (state == STATE_JOYSTICKS) {
      state = STATE_ACCEL;
    } else {
      state = STATE_JOYSTICKS;
    }
  }

  // Record select button state
  last_select = msg->buttons[BUTTON_SELECT];


  // Send velocity commands based on which mode we are in
  if (state == STATE_JOYSTICKS) {
    // Determine which joystick to use.
    // Our algorithm is: if the slow (left) joystick is used, that takes priority.
    if (msg->axes[LEFT_JOY_IDX_LINEAR] != 0.0 || msg->axes[LEFT_JOY_IDX_ANGULA] != 0.0) {
      v = msg->axes[LEFT_JOY_IDX_LINEAR] * max_v_slow;
      w = msg->axes[LEFT_JOY_IDX_ANGULA] * max_w_slow;
    } else {
      // Use the fast joystick (right)
      v = msg->axes[RIGHT_JOY_IDX_LINEAR] * max_v_fast;
      w = msg->axes[RIGHT_JOY_IDX_ANGULA] * max_w_fast;
    }

  } else {
    // Use the accelerometer
    v = msg->axes[ACCEL_IDX_LINEAR] * max_v_fast;
    w = msg->axes[ACCEL_IDX_ANGULA] * max_w_fast;
  }


  if (msg->buttons[BUTTON_TRIANGLE] == 1) {
    // Make triangle FULL SPEED AHEAD (slow)
    v = max_v_slow;

  } else if (msg->buttons[BUTTON_X] == 1) {
    // Make x FULL SPEED AHEAD (fast)
    v = max_v_fast;
  }

  double vout = v;
  double wout = flip_angular ? -w: w;

  twist.linear.x  = vout;
  twist.angular.z = wout;
  pub.publish(twist);
}

bool spin()
{
  ros::Rate r(20);
  while(node->ok())
  {
    ros::spinOnce();
    r.sleep();
  }
  return true;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "joy_to_twist");
  ros::NodeHandle n("~");
  node = &n;

  std::string input_topic;
  std::string output_topic;
  node->param("max_v_slow", max_v_slow, 0.2);
  node->param("max_w_slow", max_w_slow, 1.0);
  node->param("max_v_fast", max_v_fast, 1.0);
  node->param("max_w_fast", max_w_fast, 1.0);
  node->param("flip", flip_angular, false);
  node->param("input_topic", input_topic, std::string("joy"));
  node->param("output_topic", output_topic, std::string("twist"));

  ros::Subscriber sub1 = n.subscribe(input_topic, 10, joy_callback);
  pub = n.advertise<geometry_msgs::Twist>(output_topic, 10);

  spin();

  return 0;
}
