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
#include <cmath> 
#include <cstdlib>
#include <stdlib.h>
#include "food.h"

using namespace dlib;
using namespace std;
using namespace cv;

const int foodWidth = 40;
const int foodHeight = 40;

/**
* Check if given rectangle is in the bound box with a buffer
*
* @param testRect rectangle to test if it's encapsulated by the bound box
* @param boundBox rectangle bound box that the testRect could be in
* @param buffer integer buffer around the bound box that could encapsulate testRect
* @return true if testRect is in boundBox
*/
bool rectangleInBounds(cv::Rect testRect, cv::Rect boundBox, int buffer = 0){
    bool inBounds = false;
    if (testRect.x > boundBox.x - buffer && 
        testRect.x+testRect.width < boundBox.x+boundBox.width + buffer &&
        testRect.y > boundBox.y - buffer &&
        testRect.y+testRect.height < boundBox.y+boundBox.height + buffer){
            inBounds = true;
    }
    return inBounds;
}

/**
* Adds an alpha channel to an image
* source: https://gist.github.com/acarabott/5030cfd7f9af5f6ccd10ded1d65cc64c
*
* @param mat input 3 channel matrix
* @param dst destination 4 channel matrix 
*/
void createAlphaImage(const cv::Mat& mat, cv::Mat_<cv::Vec4b>& dst)
{
  std::vector<cv::Mat> matChannels;
  cv::split(mat, matChannels);
  
  // create alpha channel
  cv::Mat alpha = matChannels.at(0) + matChannels.at(1) + matChannels.at(2);
  matChannels.push_back(alpha);

  cv::merge(matChannels, dst);
}

/**
* Overlays an image over another at coordinates
* source: https://answers.opencv.org/question/73016/how-to-overlay-an-png-image-with-alpha-channel-to-another-png/
*
* @param src base matrix to overlay on, this matrix is edited
* @param overlay overlay matrix 
* @param location point where the top left of overlay should be on src
*/
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
        //initialize random
        srand(time(0));

        //start video capture
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }
        
        //create game window
        cv::namedWindow("gameWindow",cv::WINDOW_AUTOSIZE);

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("../shape_predictor_68_face_landmarks.dat") >> pose_model;

        // set up bounding box
        cv::Rect bound_box;

        //make foods
        std::vector<Food> foods;
        Rect coordinates;
        int randNumEdible;
        int numEdible = 2;
        for (int i = 0; i < numEdible; i++) {
            coordinates = Rect(std::rand()%(620-foodWidth-20 + 1)+20,1,foodWidth,foodHeight);
            foods.push_back(Food(coordinates));
        }
        int randNumPoision;
        int randNumLife;

        //overlay setup
        Mat mask;
        std::vector<Mat> rgbLayer;
        
        // four channel output
        cv::Mat_<cv::Vec4b> outputMat;

        // set up food motion
        Rect foodCoords;
        int velocityDelta;
        int posDelta;
        int timestep = 1;

        // initialize score and strikes
        int score = 0;
        int strikes = 0;
        Mat strikeImg;
        strikeImg = imread("../graphics/strike.png", -1);
        Point strikeLocation = Point(0, 0);

        // main gameplay loop
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

            // make new food
            randNumEdible = std::rand()%(foods.size()*25+2); 
            if (randNumEdible == 1) {
                coordinates = Rect(std::rand()%(620-foodWidth-20 + 1)+20,1,foodWidth,foodHeight);
                foods.push_back(Food(coordinates));
            }

            //make new poison
            randNumPoision = std::rand()%(100); 
            if (randNumPoision == 1) {
                coordinates = Rect(std::rand()%(620-foodWidth-20 + 1)+20,1,foodWidth,foodHeight);
                foods.push_back(Food(coordinates,FoodType::poison));
            }

            //make new hearts
            randNumLife = std::rand()%((int)(350 * 1/(strikes+1)));
            if (randNumLife == 1) {
                coordinates = Rect(std::rand()%(620-foodWidth-20 + 1)+20,1,foodWidth,foodHeight);
                foods.push_back(Food(coordinates,FoodType::life));
            }

            //detect and move foods
            for (int i = 0; i < foods.size(); i++) {
                foodCoords = foods[i].getCoordintes();
                if (rectangleInBounds(foodCoords,bound_box,18)) { //Eat food
                    //Increase strikes if poison
                    if (foods[i].GetFoodType() == poison){
                        score -= 1;
                        strikes += 1;
                    }
                    else if (foods[i].GetFoodType() == life && strikes > 0) {
                        strikes -= 1;
                    }
                    //otherwise increase score
                    else {
                        score += 1;
                    }
                    foods.erase(foods.begin()+i);

                } 
                else if (foodCoords.y+1 >= outputMat.size().height-foodCoords.height) {//delete food if out of frame
                    // Increase strikes if not poison
                    if (foods[i].GetFoodType() == food){
                        strikes += 1;
                    }
                    foods.erase(foods.begin()+i);
                }
                else { //move food down
                    velocityDelta = (int)foods[i].getAcceleration().y * timestep; //60 is arbitrary
                    foods[i].updateVelocity(0,velocityDelta);
                    posDelta = foods[i].getVelocity().y * timestep;
                    foods[i].updateCoordinates(foodCoords.x, foodCoords.y+posDelta);
                }
            }

            //overlay images
            for (int i = 0; i < foods.size(); i++) {
                Mat food = foods[i].getImg();
                overlayImage(&outputMat, &food, cv::Point(foods[i].getCoordintes().x,foods[i].getCoordintes().y));
            }

            //overlay strikes
            for (int i = 0; i < strikes; i++) {
                overlayImage(&outputMat, &strikeImg, Point(strikeLocation.x + (i * 75), 0));
            }
            //overlay score
            String scoreBoard = "Score: " + std::to_string(score); 
            putText(outputMat, scoreBoard, Point(outputMat.size().width - 200, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);

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
            }
            imshow("gameWindow",outputMat); //show game

            int keyPressed = waitKey(10); //wait to see if key pressed
            if (keyPressed == 27) break; //stop by ESC key
        }
        //user has lost (this happens in every case)
        destroyWindow("gameWindow");
        Mat loss;
        std::string textToShow = "Score: " + std::to_string(score); 
        loss = imread("../graphics/loss.jpg");
        putText(loss, textToShow, Point(loss.size().width - 200, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
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

