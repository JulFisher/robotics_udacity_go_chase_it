#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <math.h>
#include <limits>


// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv)) ROS_ERROR("Failed to call service ball_chaser");
}

float check_direction(uint32_t width, uint32_t position_in_line)
{
    if (0 <= position_in_line < static_cast<uint32_t>(width*0.45)) {return 1.f;}
    else if (static_cast<uint32_t>(width*0.45) <= position_in_line <= static_cast<uint32_t>(width*0.55))
    {
        return 0.f;
    }
    else {return -1.f;}
}


// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    uint32_t postion_in_line{0};
    float linear_x{0.3};
    float angular_z{0.3};
    float direction_multiplicator = std::numeric_limits<float>::infinity();

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    
    ROS_INFO_STREAM("Image height: " + std::to_string(image.height) + " , step size" + std::to_string(image.step) + " image width: " + std::to_string(image.width) + "image data size: " + std::to_string(sizeof(image.data)) + "\n";

    //for (const std::vector<uint8_t> &line : img.data) {
    //    for (const uint8_t &pixel : line) {
    //        if (pixel == white_pixel) {
    //            direction_multiplicator = check_direction(img.width, postion_in_line);
    //            break;
    //            }
    //        postion_in_line++;
    //        }
    //    }
    //if (std::isinf(direction_multiplicator)){
    //    drive_robot(0.0,0.0);
    //}
    //else
    //    {drive_robot(linear_x, angular_z*direction_multiplicator);}

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}