/**
 * Food Fest Game
 * 
 * @author Sam Coleman
 * @author Kate Mackowiak
 */

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <cmath> 
#include <iostream>    
#include <vector>

#include "food.h"

using namespace dlib;
using namespace std;
using namespace cv;

bool rectangleInBounds(cv::Rect test_rect, cv::Rect bound_box, int buffer = 0){
    bool in_bounds = false;
    if (test_rect.x > bound_box.x - buffer && 
        test_rect.x+test_rect.width < bound_box.x+bound_box.width + buffer &&
        test_rect.y > bound_box.y - buffer &&
        test_rect.y+test_rect.height < bound_box.y+bound_box.height + buffer){
            in_bounds = true;
    }
    return in_bounds;
}

//https://gist.github.com/acarabott/5030cfd7f9af5f6ccd10ded1d65cc64c
void createAlphaImage(const cv::Mat& mat, cv::Mat_<cv::Vec4b>& dst)
{
  std::vector<cv::Mat> matChannels;
  cv::split(mat, matChannels);
  
  // create alpha channel
  cv::Mat alpha = matChannels.at(0) + matChannels.at(1) + matChannels.at(2);
  matChannels.push_back(alpha);

  cv::merge(matChannels, dst);
}

//https://answers.opencv.org/question/73016/how-to-overlay-an-png-image-with-alpha-channel-to-another-png/
void overlayImage(cv::Mat_<cv::Vec4b>* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;

            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}

int main()
{
    try
    {
        srand(time(0));
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        
        cv::namedWindow("gameWindow",cv::WINDOW_AUTOSIZE);

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("../shape_predictor_68_face_landmarks.dat") >> pose_model;

        // set up bounding box
        cv::Rect bound_box;

        //make a food
        std::vector<Food> foods;
        int randNumFood;
        int numFoods = 2;
        for (int i = 0; i < numFoods; i++) {
            foods.push_back(Food());
        }
        int randNumPoision;
        std::vector<Poison> poisons;

        //overlay setup
        Mat mask;
        std::vector<Mat> rgbLayer;
        
        // four channel output
        cv::Mat_<cv::Vec4b> outputMat;

        int score = 0;
        Rect foodCoords, poisonCoords;
        int velocityDelta;
        int posDelta;
        int timestep = 1;
        int strikes = 0;
        while(strikes <=2)
        {
            // Grab a frame
            cv::Mat temp;
            if (!cap.read(temp))
            {
                break;
            }
            // Turn OpenCV's Mat into something dlib can deal with
            cv::flip(temp, temp, +1); //mirror
            outputMat = temp;
            IplImage ipl_img = cvIplImage(temp);
            cv_image<bgr_pixel> cimg(&ipl_img);

            //creates four channel image
            createAlphaImage(temp,outputMat);
            
            randNumFood = std::rand()%(foods.size()*15+2); // make new food?
            if (randNumFood == 1) {
                foods.push_back(Food());
            }

            randNumPoision = std::rand()%(60); //make new poison?
            if (randNumPoision == 1) {
                poisons.push_back(Poison());
            }

            //detect and move foods
            for (int i = 0; i < foods.size(); i++) {
                foodCoords = foods[i].getCoordintes();
                if (rectangleInBounds(foodCoords,bound_box,30)) { //Eat food
                    score += 1;
                    cout << "Score: " << score << endl;
                    foods.erase(foods.begin()+i);
                } 
                else if (foodCoords.y+1 >= outputMat.size().height-foodCoords.height) {//delete food if out of frame
                    strikes += 1;
                    foods.erase(foods.begin()+i);
                }
                else { //move food down
                    velocityDelta = (int)foods[i].getAcceleration().y * timestep; //60 is arbitrary
                    foods[i].updateVelocity(0,velocityDelta);
                    posDelta = foods[i].getVelocity().y * timestep;
                    foods[i].updateCoordinates(foodCoords.x, foodCoords.y+posDelta);
                }
            }

            //detect and move poisons
            for (int i = 0; i < poisons.size(); i++) {
                poisonCoords = poisons[i].getCoordintes();
                if (rectangleInBounds(poisonCoords,bound_box,30)) { //Eat poison
                    score -= 1;
                    strikes += 1;
                    cout << "Score: " << score << endl;
                    poisons.erase(poisons.begin()+i);
                } 
                else if (poisonCoords.y+1 >= outputMat.size().height-poisonCoords.height) {//delete poison if out of frame
                    poisons.erase(poisons.begin()+i);
                }
                else { //move poison down
                    velocityDelta = (int)poisons[i].getAcceleration().y * timestep;
                    poisons[i].updateVelocity(0,velocityDelta);
                    posDelta = poisons[i].getVelocity().y * timestep;
                    poisons[i].updateCoordinates(poisonCoords.x, poisonCoords.y+posDelta);
                }
            }

            //overlay images
            for (int i = 0; i < foods.size(); i++) {
                Mat food = foods[i].getImg();
                overlayImage(&outputMat, &food, cv::Point(foods[i].getCoordintes().x,foods[i].getCoordintes().y));
            }

            for (int i = 0; i < poisons.size(); i++) {
                Mat poison = poisons[i].getImg();
                overlayImage(&outputMat, &poison, cv::Point(poisons[i].getCoordintes().x,poisons[i].getCoordintes().y));
            }

            // Detect faces 
            std::vector<dlib::rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i){
                shapes.push_back(pose_model(cimg, faces[i]));

                //Find bound box of mouth
                // left: 61 right: 65 top: 63 bottom: 67
                bound_box = cv::Rect(shapes[i].part(49).x(),    
                                    shapes[i].part(63).y(), 
                                    abs(shapes[i].part(49).x()-shapes[i].part(55).x()), 
                                    abs(shapes[i].part(63).y()-shapes[i].part(67).y()));
                //cv::rectangle(outputMat,bound_box,cv::Scalar(255,0,0));
                
            }
            imshow("gameWindow",outputMat); //show game

            int keyPressed = waitKey(10); //wait to see if key pressed
            if (keyPressed == 27) break; //stop by ESC key
        }
        //user has lost
        destroyWindow("gameWindow");
        Mat loss;
        std::string textToShow = "Score: " + std::to_string(score); 
        loss = imread("../foods/loss.jpg");
        putText(loss, textToShow, Point(loss.size().width/10, (loss.size().height/4)), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
        imshow("Loser Screen", loss);
        waitKey(0);
        return 0;
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}

