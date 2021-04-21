#include "food.h"


std::vector<std::string> imgAddresses {"../foods/cookie.png"};

using namespace cv;
using namespace std;

Food::Food(){
    string imgAddress = imgAddresses[0];//std::rand() % imgAddresses.size()];
    cout<<imgAddress<<endl;
    img = imread(imgAddress, 0);
    cv::resize(img, img, Size(30,30)); 
    coordinates = Rect(0,0,img.size().width,img.size().height);

    printf("image when made: %i, %i \n", img.size().width, img.size().height);
}

Mat Food::getImg(){
    return img;
}       

Rect Food::getCoordintes(){
    return coordinates;
}

void Food::updateCoordinates(int x, int y){
    coordinates.x = x;
    coordinates.y = y;
}
