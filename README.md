# SoftSysFoodFest
OpenCV food catching game utilizing dlib for face identification

Identifies any faces within a webcam frame and uses that information to let players "eat" food images on screen.
This project includes two versions of the FoodFest game:
#### StaticFood:
Food appears on the screen and remains in one location. The game never ends. 
#### FallingFood:
This game has food, poison, and hearts which fall from the top of the screen. The player must catch food, and avoid poison, otherwise they lose a life. The game ends when the player loses three lives, however until that point they can regain lives by eating hearts. There is no way to win this game.

## Requirements:
- Opencv >= 3.0 
- Dlib >= 19.6 

## Installation

Create a build directory, run cmake .., and then run make.

```bash
mkdir build
cd build
cmake ..
make
```

Additionally, move dlib library source folder to build.

## Usage
### Static Food Game
To play, run staticFood from the build directory.
```bash
cd build
./staticFood
```
Eat the cookies that appear on screen by opening one's mouth and moving to the cookie location.

Press the 'esc' key to exit the program. 

### Falling Food Game
To play, run fallingFood from the build directory.
```bash
cd build
./fallingFood
```
Eat the cookies that fall from the top of the screen by opening one's mouth and moving to the cookie location. If a cookie reaches the bottom of the screen un eaten, the player loses a life. If the player eats a poison bottle, the player also loses a life. The player can regain a life by eating a heart. The game ends when the player loses all three of their lives.

Press the 'esc' key to exit the program. 

## Contributors
Written by [Sam Coleman](https://github.com/sam-coleman) and [Kate Mackowiak](https://kviiim.github.io/) for Software Systems at Olin College of Engineering 
