#include "food.h"


std::vector<std::string> imgAddresses {"../foods/cookie.png"};

using namespace cv;
using namespace std;


Food::Food(){
    string imgAddress = imgAddresses[0];//std::rand() % imgAddresses.size()];
    //cout<<imgAddress<<endl;
    img = imread(imgAddress, -1);
    cv::resize(img, img, Size(40,40)); 
    //srand(time(0));
    //coordinates = Rect(rand()%(620-20 + 1)+20,rand()%(460-50 + 1)+50,img.size().width,img.size().height);
    coordinates = Rect(rand()%(620-img.size().width-20 + 1)+20,rand()%(460-img.size().height-50 + 1)+50,img.size().width,img.size().height);

    cout << "x cord: " << coordinates.x << "y cord: " << coordinates.y << endl;

    //printf("image when made: %i, %i \n", img.size().width, img.size().height);
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
