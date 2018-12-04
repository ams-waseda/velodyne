


#include <velodyne_pointcloud/pointcloudXYZIR.h>

namespace velodyne_pointcloud 
{
  void PointcloudXYZIR::addPoint(const float& x, const float& y, const float& z, const uint16_t& ring, const uint16_t& /*azimuth*/, const float& /*distance*/, const float& intensity, const uint32_t& time) 
  {
    // convert polar coordinates to Euclidean XYZ
    velodyne_rawdata::VPoint point;
    point.ring = ring;
    point.x = x;
    point.y = y;
    point.z = z;
    point.intensity = intensity;
    point.time = time;

    // append this point to the cloud
    pc->points.push_back(point);
    ++pc->width;
  }
}

