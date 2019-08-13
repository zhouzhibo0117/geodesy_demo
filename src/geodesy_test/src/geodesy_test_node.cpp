#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"
#include "geodesy/utm.h"
#include "geodesy/wgs84.h"
#include "geographic_msgs/GeoPointStamped.h"

void chatterCallback(const sensor_msgs::NavSatFixConstPtr &NavSatFixMsg)
{
    geographic_msgs::GeoPointStampedPtr gps_msg(new geographic_msgs::GeoPointStamped());
    gps_msg->header = NavSatFixMsg->header;
    gps_msg->position.latitude = NavSatFixMsg->latitude;
    gps_msg->position.longitude = NavSatFixMsg->longitude;
    gps_msg->position.altitude = NavSatFixMsg->altitude;

    geodesy::UTMPoint utm_point;
    geodesy::fromMsg(gps_msg->position, utm_point);

    std::cout << std::fixed << std::setprecision(4) << " E " << utm_point.easting << " N " << utm_point.northing << " Z " << utm_point.altitude << '\n';

    geographic_msgs::GeoPoint out_gps = geodesy::toMsg(utm_point);

    std::cout << std::fixed << std::setprecision(4) << " Lat " << out_gps.latitude - NavSatFixMsg->latitude << " Long " << out_gps.longitude - NavSatFixMsg->longitude << " Z " << out_gps.altitude - NavSatFixMsg->altitude << '\n';
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "GeodesyTest");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/Inertial/gps/fix", 1000, chatterCallback);

    ros::spin();

    return 0;
}