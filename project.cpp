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

using namespace dlib;
using namespace std;

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

        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("../shape_predictor_68_face_landmarks.dat") >> pose_model;

        // set up bounding box
        cv::Rect bound_box;

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
                cv::rectangle(temp,bound_box,cv::Scalar(255,0,0));

                cv::Rect new_rect = cv::Rect(200,200,20,20);
                cv::rectangle(temp,new_rect,cv::Scalar(255,255,0));


                if (rectangle_in_bounds(new_rect,bound_box,10)) { //switch color to blue
                    printf("intersect!!!!! %i \n", count);
                    count++;
                } 
            }

            // Display it all on the screen
            win.clear_overlay();
            win.set_image(cimg);
            // win.add_overlay(render_face_detections(shapes));
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


