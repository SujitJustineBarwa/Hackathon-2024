#ifndef LOCALIZATION_H
#define LOCALIZATION_H

class localization
{
public:
    localization();
    ~localization();

    // Function to set the robot's location
    void setLocation(float x, float y,float theta);

    // Functions to get the robot's location
    float getX() const;
    float getY() const;
    float get_cell_X() const;
    float get_cell_Y() const;
    void update_location_based_on_TOF(float front_TOF_reading,float right_TOF_reading,float back_TOF_reading,float left_TOF_reading,float gyro_reading);

private:
    float x; // X coordinate of the robot's real location
    float y; // Y coordinate of the robot's real location
    float theta,reference_theta;
    int cell_coordinate_x;
    int cell_coordinate_y;
    float prev_front_TOF_reading = 0;
    float prev_right_TOF_reading = 0;
    float prev_back_TOF_reading = 0;
    float prev_left_TOF_reading = 0;
    bool initialized;
};

#endif // LOCALIZATION_H
