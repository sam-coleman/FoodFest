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

enum FoodType {food, poison, life}; //define types of eatable objects

/**
* Creates all eatable objects: Food, Poision, and Life
*
* Creates all eatable objects: Food, Poision, and Life. Creates class variables to control motion
*/
class Food {       
    public:
        /**
        * Constructor to create a food object of a specific type and initialize with its values
        *
        * @param <inputCoordinates> Rect representing the x and y coordinate locations
        * @param <foodTypeInput> Type of food (food, poison, or life). Default value is Food
        * @param <width> Integer width of image to resize to
        * @param <height> Integer height of image to resize to
        */
        Food(cv::Rect inputCoordinates, FoodType foodTypeInput = FoodType::food, int width=40, int height=40);


         /**
        * Return image of object
        * @return cv::Mat file holding image
        */
        cv::Mat getImg();    

        /**
        * Get current coordinates of Food Object
        *        
        * @return Rectangle holdinng the x and y coordinates
        */
        cv::Rect getCoordintes();

        /**
        * Update the coordinates of Fodo Object
        *
        * @param <x> Integer of new x coordinate
        * @param <y> Integer of new y coordinate
        */
        void updateCoordinates(int x, int y);

        /**
        * Update the velocity of Food Object
        *
        * @param <xVelDelta> Integer representing change of x velocity
        * @return <yVelDelta> Integer representing change of y velocity
        */
        void updateVelocity(int xVelDelta, int yVelDelta);


        /**
        * Get current velocity of Food Object
        *
        * @return Point representing velocity
        */
        cv::Point getVelocity();

        /**
        * Get current acceleration of Food Object
        *
        * @return Point representing acceleration
        */
        cv::Point getAcceleration();

        /**
        * Get type of Food
        *
        * @return Foodtype of Food, Poision, or Life
        */
        FoodType GetFoodType();

    private:
        cv::Mat img;
        cv::Rect coordinates;
        cv::Point velocity; 
        cv::Point acceleration;
        FoodType foodType;
};
