#include "food.h"

using namespace cv;
using namespace std;

std::vector<std::string> imgAddressesFoods {"../foods/cookie.png", "../foods/oreo.png"};
std::vector<std::string> imgAddressesPoisons {"../foods/lettuce.png"};

Food::Food(){
    string imgAddress = imgAddressesFoods[rand() % imgAddressesFoods.size()];
    //cout<<imgAddress<<endl;
    img = imread(imgAddress, -1);
    // cv::resize(img, img, Size(40,40)); 
    //srand(time(0));
    //coordinates = Rect(rand()%(620-20 + 1)+20,rand()%(460-50 + 1)+50,img.size().width,img.size().height);
    coordinates = Rect(rand()%(620-img.size().width-20 + 1)+20,1,img.size().width,img.size().height);

    velocity = Point(0,7);
    acceleration = cv::Point(0,1);
    //cout << "x cord: " << coordinates.x << "y cord: " << coordinates.y << endl;

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

void Food::updateVelocity(int xVel, int yVel){
    velocity.x += xVel;
    velocity.y += yVel;
}

Point Food::getVelocity(){
    return velocity;
}

Point Food::getAcceleration(){
    return acceleration;
}

Poison::Poison() : Food() {
    string imgAddress = imgAddressesPoisons[rand() % imgAddressesPoisons.size()];
    //cout<<imgAddress<<endl;
    img = imread(imgAddress, -1);
    // cv::resize(img, img, Size(40,40)); 
    //srand(time(0));
    //coordinates = Rect(rand()%(620-20 + 1)+20,rand()%(460-50 + 1)+50,img.size().width,img.size().height);
    coordinates = Rect(rand()%(620-img.size().width-20 + 1)+20,1,img.size().width,img.size().height);

    velocity = Point(0,7);
    acceleration = cv::Point(0,1);
}

