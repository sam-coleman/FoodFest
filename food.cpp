#include "food.h"

using namespace cv;
using namespace std;

std::vector<std::string> imgAddressesFoods {"../foods/cookie.png", "../foods/oreo.png"};
std::vector<std::string> imgAddressesPoisons {"../foods/lettuce.png"};

const int gravity = 1;
const int terminalVelocity = 12; 

Food::Food(bool poisonInput){
    poison = poisonInput;
    string imgAddress;
    if (poison){
        imgAddress = imgAddressesPoisons[rand() % imgAddressesPoisons.size()];
    }
    else {
        imgAddress = imgAddressesFoods[rand() % imgAddressesFoods.size()];
    }
    img = imread(imgAddress, -1);
    coordinates = Rect(rand()%(620-img.size().width-20 + 1)+20,1,img.size().width,img.size().height);
    velocity = Point(0,rand()%(7));
    acceleration = cv::Point(0,1);
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
    acceleration.y = gravity - velocity.y*velocity.y/(terminalVelocity*terminalVelocity);
    return acceleration;
}

bool Food::IsPoison(){
    return poison;
}
