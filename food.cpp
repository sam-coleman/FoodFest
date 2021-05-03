#include "food.h"

using namespace cv;
using namespace std;

//define options for each type of object
std::vector<std::string> imgAddressesFoods {"../foods/cookie.png", "../foods/oreo.png"};
std::vector<std::string> imgAddressesPoisons {"../foods/poison.png"};
std::vector<std::string> imgAddressesLives {"../foods/heart.png"};

//define consyants
const int gravity = 1;
const int terminalVelocity = 12; 


Food::Food(Rect inputCoordinates, FoodType foodTypeInput, int width, int height){
    string imgAddress; //initialize image location

    foodType = foodTypeInput; //Food, Poision, or Life

    //get random image address corresponding with object to eat
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

    //read and resize image
    img = imread(imgAddress, -1);
    cv::resize(img, img, Size(width,height)); 

    //initialize class variables
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
