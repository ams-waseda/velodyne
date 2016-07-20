#include <ros/ros.h>
#include <ros/package.h>
#include <pcl/point_types.h>
#include <velodyne_pointcloud/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef velodyne_pointcloud::PointXYZIR VPoint;
typedef pcl::PointCloud<VPoint> VPointCloud;

using namespace std;

ofstream fout;

void laserconv(const VPointCloud::ConstPtr &msg)
{
  static uint32_t cnt=0;

  for (uint32_t i=0; i<msg->width*msg->height; i++)
  { 
    fout.write((char *)&msg->points[i].time,sizeof(uint32_t));
    fout.write((char *)&cnt,sizeof(uint32_t));
    fout.write((char *)&i,sizeof(uint32_t));
    fout.write((char *)&msg->points[i].x,sizeof(float));
    fout.write((char *)&msg->points[i].y,sizeof(float));
    fout.write((char *)&msg->points[i].z,sizeof(float));
    fout.write((char *)&msg->points[i].intensity,sizeof(float));
    fout.write((char *)&msg->points[i].ring,sizeof(float));
    fout.write((char *)&msg->points[i].azimuth,sizeof(float));
    fout.write((char *)&msg->points[i].firing,sizeof(float));
  }
  cnt++;
}

/** Main node entry point. */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "laser_logger");
  ros::NodeHandle node;
  ros::NodeHandle priv_nh("~");
  
  boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());
  string path = ros::package::getPath("velodyne_pointcloud")+"/log/log_" + to_iso_string(now) +".bin";
  
  fout.open(path.c_str(),ios::binary|ios::out);

  ros::Subscriber sub=node.subscribe("velodyne_points_gps", 1000, laserconv);
  
  ros::spin();
  
  fout.close();
}
