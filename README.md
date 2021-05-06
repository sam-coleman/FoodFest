# Food Fest Game
OpenCV food catching game utilizing dlib for face identification

Identifies any faces within a webcam frame and uses that information to let players "eat" food images on screen.
This project includes two versions of the FoodFest game:
#### StaticFood:
Food appears on the screen and remains in one location. The game never ends. 
#### FallingFood:
This game has food, poison, and hearts which fall from the top of the screen. The player must catch food, and avoid poison, otherwise they lose a life. The game ends when the player loses three lives, however until that point they can regain lives by eating hearts. There is no way to win this game.

## Requirements:
- Opencv >= 3.0 
- Dlib >= 19.6 (See below for installation process)

## Installation

To install Dlib and add it to the build, we followed this set of instructions: https://learnopencv.com/install-dlib-on-ubuntu/ which are detailed as following:

Install OS libraries
```bash
sudo apt-get install build-essential cmake pkg-config
sudo apt-get install libx11-dev libatlas-base-dev
sudo apt-get install libgtk-3-dev libboost-python-dev
```
Install Python libraries
```bash
sudo apt-get install python-dev python-pip python3-dev python3-pip
sudo -H pip3 install -U pip numpy
```
In the FoodFest directory:

Setup directories
```bash
mkdir build
cd build
mkdir dlib
cd dlib
```
Make Dlib
```bash
wget http://dlib.net/files/dlib-19.6.tar.bz2
tar xvf dlib-19.6.tar.bz2
cd dlib-19.6/
mkdir build
cd build
cmake ..
cmake --build . --config Release
sudo make install
sudo ldconfig
cd ..
```
Compile game
```bash
cd ../..
cmake ..
make
```

## Usage
### Static Food Game
To play, run staticFood from the build directory.
```bash
./staticFood
```
Eat the cookies that appear on screen by opening one's mouth and moving to the cookie location.

Press the 'esc' key to exit the program. 

### Falling Food Game
To play, run fallingFood from the build directory.
```bash
./fallingFood
```
Eat the cookies that fall from the top of the screen by opening one's mouth and moving to the cookie location. If a cookie reaches the bottom of the screen un eaten, the player loses a life. If the player eats a poison bottle, the player also loses a life. The player can regain a life by eating a heart. The game ends when the player loses all three of their lives.

Press the 'esc' key to exit the program. 

## Contributors
Written by [Sam Coleman](https://github.com/sam-coleman) and [Kate Mackowiak](https://kviiim.github.io/) for Software Systems at Olin College of Engineering 
