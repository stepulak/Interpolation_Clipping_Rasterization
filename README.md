# Interpolation, clipping and rasterization

Interpolation, clipping and rasterization algorithms in C++.

Algorithms which can be found in this application:
- DDA, Bresenham line interpolation
- Bresenham circle/ellipse interpolation
- Cohen-Sutherland line clipping (interval bisection) in rectangular window
- Cyrus-Beck line clipping in convex polygon (which is randomly generated)
- Sutherland-Hodgman polygon clipping (xmax part)
- Line seed fill of generic polygons
- Pineda's algorithm for triangle rasterization (min-max mechanism)

After you start the application, read the information and pick one algorithm using keyboard key.

Algorithms are based on user interaction (mouse), they are animated and can be stepped (E, S keys).

Using the application should be at least a little bit intuitive, if you follow the text in the top-left corner of your application.

Exit application using `escape` key.

### Build

To build this application you need to have `g++` (or similar compiler) installed with C++17 support.
Furthermore you ought to have `SDL2` and `SDL2_image` libraries in your computer.

In Ubuntu, install `g++`, `libsdl2-dev` and `libsdl2-image-dev` packages.

Then run `./build.sh` script. If compilation ends with success, run the `./icr-algo` application.