# 3D light simulation with point lights in OpenGL using textures.

This work is a part of a university project implemented for the preliminary thesis project.

## Prerequisites

In order to run the application you'll need the following libraries and compiler:<br>
For **DEBIAN**
```bash 
sudo apt-get install libglfw3 libglfw3-dev g++
```
For **MANJARO**
```bash
sudo pacman -S g++ glfw-x11
```
And to compile and run the program
```bash
g++ main.cpp -std=c++17 -lstdc++fs -lGL -lglfw && ./a.out
```


## Controls
* **WSAD** moving around.
* **LShift** sprint.
* **MouseWheel** zooming and unzooming.
* **PageUP/PageDown** change the height of the light sources.
* **Home** disco on/off.
* **L** turns on/off PolygonMode.

## Showcase
![gif](gif/room.gif)