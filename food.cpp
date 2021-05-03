#include "food.h"

using namespace cv;
using namespace std;

std::vector<std::string> imgAddressesFoods {"../foods/cookie.png", "../foods/oreo.png"};
std::vector<std::string> imgAddressesPoisons {"../foods/poison.png"};
std::vector<std::string> imgAddressesLives {"../foods/heart.png"};

const int gravity = 1;
const int terminalVelocity = 12; 

Food::Food(Rect inputCoordinates, FoodType foodTypeInput, int width, int height){
    foodType = foodTypeInput;
    string imgAddress;

    switch(foodType) {
        case FoodType::poison:
            imgAddress = imgAddressesPoisons[rand() % imgAddressesPoisons.size()];
            break;
        case FoodType::life:
            imgAddress = imgAddressesLives[rand() % imgAddressesLives.size()];
            break;
        default:
            imgAddress = imgAddressesFoods[rand() % imgAddressesFoods.size()];
    }

    img = imread(imgAddress, -1);
    cv::resize(img, img, Size(width,height)); 
    // coordinates = Rect(rand()%(620-img.size().width-20 + 1)+20,1,img.size().width,img.size().height);
    coordinates = inputCoordinates;
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

FoodType Food::GetFoodType(){
    return foodType;
}
