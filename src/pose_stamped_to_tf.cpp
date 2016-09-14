#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>

std::string parent_frame, child_frame;

void
poseCallback(const geometry_msgs::PoseStamped& msg)
{
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin(tf::Vector3(msg.pose.position.x, msg.pose.position.y, msg.pose.position.z));
  tf::Quaternion q(msg.pose.orientation.x, msg.pose.orientation.y, msg.pose.orientation.z, msg.pose.orientation.w );
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), parent_frame, child_frame));
}

int
main(int argc, char** argv)
{
  ros::init(argc, argv, "pose_broadcaster", ros::init_options::AnonymousName);
  if (argc < 4)
  {
    ROS_ERROR("syntax:\n%s parent_frame child_frame topic_name\n", argv[0]);
    return -1;
  };
  parent_frame = argv[1];
  child_frame = argv[2];
  std::string topic_name(argv[3]);

  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe(topic_name.c_str(), 10, &poseCallback);

  ros::spin();
  return 0;
};
