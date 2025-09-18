# simple-las-viewer

A simple LAS point cloud renderer in C and OpenGL
![2025-01-29_01-03](https://github.com/user-attachments/assets/4d251d47-8576-4bde-bb75-08c6bda55fdc)
## Features
Flying camera movement with mouse lookaround (WASD, Space, Shift)  
Point color modes (1) Height scaled RGB, (2) Intensity of point, (3) Mix between 1 and 2  
Model scaling (+e, -q)  
Render Distance (+c, -z)  

## Dependencies
Available on the AUR:
```
sudo pacman -S glfw cglm
```

## Installation
Clone this repository
```
make
./simple-las-viewer [path] [--reduce-noise]
```

## Notes
Untested on Windows. You may need to manually link libraries  
Only Point Data Record Format 6 supported
