# gjk2D
Implementation of the GJK algorithm in 2D using raylib.

This simple implementation was inspired by [https://www.youtube.com/watch?v=Qupqu1xe7Io&t=1750s](this lecture) by Casey Muratori.
The lecture explained a 3D implementation of the GJK algorithm, but for the sake of ease of visualization I decided to implement only a 2D version of it.

## Building
__Windows__: execute `build.bat`

__Linux__: execute `build.sh`

The resulting executable will be in the `bin` directory.

## Usage
The black shape is still in the center, while the other one follows the cursor.

The color of the second shape is green if there's a collision, red otherwise.

Keybindings:
- `Mouse wheel`: Zoom in/out
- `F4`: Close the application

## Screenshots
![./images/collision.jpn]
![./images/no_collision.jpn]
