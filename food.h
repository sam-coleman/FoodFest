#pragma once

#include <tuple> 
#include <stdlib.h>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <cmath> 
#include <vector>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <cmath> 
#include <iostream>    
#include <cstdlib>

enum FoodType {food, poison, life};
class Food {       
    public:
        Food(FoodType foodTypeInput = FoodType::food);

        cv::Mat getImg();    

        cv::Rect getCoordintes();

        void updateCoordinates(int x, int y);

        //Takes in delta for vel
        void updateVelocity(int xVelDelta, int yVelDelta);

        cv::Point getVelocity();

        cv::Point getAcceleration();

        FoodType GetFoodType();

    private:
        cv::Mat img;
        cv::Rect coordinates;
        cv::Point velocity; 
        cv::Point acceleration;
        FoodType foodType;
};
