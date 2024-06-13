#include "localization.h"
#include "constants.h"
#include <cmath>

// Constructor
localization::localization()
    : x(0.0), y(0.0) // Initialize x and y to 0
{
    // ctor
}

// Destructor
localization::~localization()
{
    // dtor
}

// Function to get the robot's X coordinate
float localization::getX() const
{
    return x;
}

// Function to get the robot's Y coordinate
float localization::getY() const
{
    return y;
}

// Function to get the robot's cell X coordinate
float localization::get_cell_X() const
{
    return cell_coordinate_x;
}

// Function to get the robot's cell Y coordinate
float localization::get_cell_Y() const
{
    return cell_coordinate_y;
}

// Function to set the robot's location
void localization::setLocation(float x, float y,float theta)
{
    this -> x = x;
    this -> y = y;
    this -> theta = theta;
    this -> cell_coordinate_x = x/WALL_SIZE;
    this -> cell_coordinate_y = y/WALL_SIZE;
}

// Function to update the bot's position based on TOF reading
void localization::update_location_based_on_TOF(float front_TOF_reading,float right_TOF_reading,float back_TOF_reading,float left_TOF_reading,float gyro_reading)
{
    if (this->initialized == false){
        this->reference_theta = gyro_reading;
    }

    // Theta updating
    this->theta = this -> reference_theta - gyro_reading;

    // Removing the n*180mm components
    float corrected_front_TOF_reading = front_TOF_reading - std::floor(front_TOF_reading/WALL_SIZE)*WALL_SIZE;
    float corrected_back_TOF_reading = back_TOF_reading - std::floor(back_TOF_reading/WALL_SIZE)*WALL_SIZE;
    float corrected_left_TOF_reading = left_TOF_reading - std::floor(left_TOF_reading/WALL_SIZE)*WALL_SIZE;
    float corrected_right_TOF_reading = right_TOF_reading - std::floor(right_TOF_reading/WALL_SIZE)*WALL_SIZE;

    float d_front_TOF = corrected_front_TOF_reading - this->prev_back_TOF_reading;
    float d_back_TOF = corrected_back_TOF_reading - this->prev_back_TOF_reading;
    float d_left_TOF = corrected_left_TOF_reading - this->prev_left_TOF_reading;
    float d_right_TOF = corrected_right_TOF_reading - this->prev_right_TOF_reading;

    float dx_estimate_from_front = d_front_TOF * std::cos(this->theta);
    float dx_estimate_from_back = -d_back_TOF * std::cos(this->theta);
    float dx_estimate_from_right = -d_right_TOF * std::sin(this->theta);
    float dx_estimate_from_left = d_left_TOF * std::sin(this->theta);

    float dy_estimate_from_front = d_front_TOF * std::sin(this->theta);
    float dy_estimate_from_back = -d_back_TOF * std::sin(this->theta);
    float dy_estimate_from_right = d_right_TOF * std::cos(this->theta);
    float dy_estimate_from_left = -d_left_TOF * std::cos(this->theta);

    float x_estimate_from_front = this->getX() - dx_estimate_from_front;
    float x_estimate_from_back = this->getX() - dx_estimate_from_back;
    float x_estimate_from_right = this->getX() - dx_estimate_from_right;
    float x_estimate_from_left = this->getX() - dx_estimate_from_left;

    float y_estimate_from_front = this->getY() - dy_estimate_from_front;
    float y_estimate_from_back = this->getY() - dy_estimate_from_back;
    float y_estimate_from_right = this->getY() - dy_estimate_from_right;
    float y_estimate_from_left = this->getY() - dy_estimate_from_left;

    // Average all the estimate
    float x_avg = (x_estimate_from_front + x_estimate_from_back + x_estimate_from_left + x_estimate_from_right)/4;
    float y_avg = (y_estimate_from_front + y_estimate_from_back + y_estimate_from_left + y_estimate_from_right)/4;

    this->setLocation(x_avg,y_avg,this->theta);

    this->prev_back_TOF_reading = back_TOF_reading;
    this->prev_front_TOF_reading = front_TOF_reading;
    this->prev_left_TOF_reading = left_TOF_reading;
    this->prev_right_TOF_reading = right_TOF_reading;
}
