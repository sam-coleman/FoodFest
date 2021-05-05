# Food Fest
Sam Coleman and Kate Mackowiak

## Project Goals
Our goal is to replicate a messenger or snapchat style game in which a player uses their camera and laptop to catch "food" in their mouth and avoid "poison". We will implement this using C++ and OpenCV. Our lower bound is identifying a face and finding the intersection between the face and a food image overlayed on top. Our stretch goal is to actually identify the mouth in the face to see if the person has "eaten" the food. We also want to make sure our game works for all skin tones. Additionally, we want to make a fun game that will also be portfolio worthy.

## Learning Goals
Our learning goals are to continue to deepen our understanding of C++ and OpenCV. We started down this path with project one, but recognize that we still have a long way to go. We are hoping that facial recognition is a common enough task that we will be able to find a significant number of resources to use when implementing our project.

Additionally, we want to make a professional looking project that will spice up our portfolios (and our lives).

## Resources
The main resource we used was [DLib Facial landmark detector]( http://dlib.net/webcam_face_pose_ex.cpp.html). This was the primary code we used to detect the user's mouth.

Additionally, we had two main resources to help with specific features of our game. To overlay a png on the Video feed, we used a function from [Acarabott](https://gist.github.com/acarabott/5030cfd7f9af5f6ccd10ded1d65cc64c) to add an alpha channel to an image. Additionally, we used the [OpenCV forum](https://answers.opencv.org/question/73016/how-to-overlay-an-png-image-with-alpha-channel-to-another-png/) to overlay a png on the Video feed.

## What We Did

We were succesfully able to implement two versions of our game - one in which there are just [static foods](https://github.com/sam-coleman/FoodFest/blob/main/staticFood.cpp), and one in which [foods fall](https://github.com/sam-coleman/FoodFest/blob/main/fallingFood.cpp) from the sky along with various other interactive items. We were able to reach our upper bound goal of implementing facial feature detection using the Dlib library. 

The facial feature and mouth bound box detection portion of our code is below.

``` c
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
```
This code section detect a face in an image using Dlib detector. It then adds the set of coordinates corresponding to various features of the face to the vector shapes. Then, it identifies a rectangle which spans the width and height of the mouth. This bound box is later used to identify if the player has "eaten" and of the items on screen.

We implemented a [class](https://github.com/sam-coleman/FoodFest/blob/main/food.cpp) to handle all of our item's locations and images, which works for both our static and falling games. This allowed our code to be more versatile as well as increasing the ease of adding and removing images from the screen.

Additionally, we implemented code to allow our items to fall with a simulated form of gravity. This is shown in the snipped below:

```c
//move food down
velocityDelta = (int)foods[i].getAcceleration().y * timestep;
foods[i].updateVelocity(0,velocityDelta);
posDelta = foods[i].getVelocity().y * timestep;
foods[i].updateCoordinates(foodCoords.x, foodCoords.y+posDelta);
```
This code updates the acceleration, velocity, and position of an item. The acceleration is calculated based on a constant gravity value and the velocity of the item, allowing us to have the item reach a terminal velocity, making the game easier to play.

[Here](https://drive.google.com/drive/folders/1G7O1n7blfaDjjyArPd5SApPYuCYoTphK?usp=sharing) is a link to a Google Drive Folder with videos of Food Fest in action.
## Design Decision

One design decision we made was how to make the items appear on screen with a degree of randomness, while avoiding a scenario where too any items appear at once and overwhelm the user. We decided to do this by making the chance that an item appears dependent on the number of items already on the screen. The calculations for each type of item are shown below.

```c
//cookies number
randNumEdible = std::rand()%(foods.size()*25+2); 
//poison number
randNumPoision = std::rand()%(100); 
//heart number
randNumLife = std::rand()%((int)(350 * 1/(strikes+1)));
```

The maximum for each given item type is calculated by either the number of items on screen multiplied by a constant (cookies and poison) or the number of stikes a player has gained (lives). 

While this isn't a perfect strategy, we feel that this design decision worked out pretty well, as it allows the items falling to flow depending on the current state of the game.

## Reflection

We are really proud of this project and what we were able to accomplish and learn. We were able to achieve our upper bound of detecting intersection with the mouth, as opposed to the entire face. Furthermore, we were able to implement addtionally game mechanics to improve the user experience. These include the multiple options of what foods (cookies) which add a point (and lose a life and a point if you miss one), poisons which lose a life and a point if you eat one, and hearts which give you one life back. We also thought about the relative frequency of each type of item appearing to optimize gameplay.

With more time, we would have liked to write a bash script to make it easier for others to install everything needed to play Food Fest. We would also like to add multiplayer capibilities. Finally, we would like to modify the random nature of items appearing to ensure it is not impossible to catch all the foods at times.s

One of our other learning goals was to further explore the world of C++ and OpenCV, which we defintiely did. We are proud of our final outputs and what we were able to accomplish - and definitely would consider this portfolio worthy.
