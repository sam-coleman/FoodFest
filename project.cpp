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

bool rectangle_in_bounds(cv::Rect test_rect, cv::Rect bound_box, int buffer = 0){
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


int main()
{
    try
    {
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            cerr << "Unable to connect to camera" << endl;
            return 1;
        }

        image_window win;


        cv::namedWindow("win2",cv::WINDOW_AUTOSIZE);

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("../shape_predictor_68_face_landmarks.dat") >> pose_model;

        // set up bounding box
        cv::Rect bound_box;

        //make a food
        Food new_food = Food();
        new_food.updateCoordinates(200,200);

        //overlay setup
        Mat mask;
        std::vector<Mat> rgbLayer;
        
        // four channel output
        cv::Mat_<cv::Vec4b> outputMat;

        int count = 0;
        // Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            if (!cap.read(temp))
            {
                break;
            }
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv::flip(temp, temp, +1); //mirror
            outputMat = temp;
            IplImage ipl_img = cvIplImage(temp);
            cv_image<bgr_pixel> cimg(&ipl_img);

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
                cv::rectangle(outputMat,bound_box,cv::Scalar(255,0,0));
                
                
                // split(new_food.getImg(),rgbLayer);
                // if(new_food.getImg().channels() == 4)
                // {
                //     split(new_food.getImg(),rgbLayer);         // seperate channels
                //     Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
                //     merge(cs,3,new_food.getImg());  // glue together again
                //     mask = rgbLayer[3];       // png's alpha channel used as mask
                // }
                
                createAlphaImage(temp,outputMat);

                cout<<outputMat.channels()<<endl;

                new_food.getImg().copyTo(outputMat(new_food.getCoordintes()),mask);

                // printf("cookie size %i %i \n",new_food.getImg().size().width,new_food.getImg().size().height);
                //new_food.getImg().copyTo(temp(new_food.getCoordintes())); 

                if (rectangle_in_bounds(new_food.getCoordintes(),bound_box,10)) { //switch color to blue
                    printf("intersect!!!!! %i \n", count);
                    count++;
                } 
            }

            // cvtColor(outputMat,outputMat,COLOR_BGRA2RGBA);            

            // IplImage ipl_img_test = cvIplImage(outputMat);
            // cv_image<rgb_alpha_pixel> testimg(&ipl_img_test);
            
            // Display it all on the screen
            win.clear_overlay();
            win.set_image(cimg);
            // win.
            // win.add_overlay(render_face_detections(shapes));
            imshow("win2",outputMat);

            
        }
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

