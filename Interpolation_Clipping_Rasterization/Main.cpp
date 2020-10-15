/*
	(C) 2017 Stepan Trcka

	This application provides basic algorithms and examples of interpolation, clipping and rasterization.

	Algorithms which can be found in this application:
		- DDA, Bresenham line interpolation
		- Bresenham circle/ellipse interpolation
		- Cohen-Sutherland line clipping (interval bisection) in rectangular window
		- Cyrus-Beck line clipping in convex polygon (which is randomly generated)
		- Sutherland-Hodgman polygon clipping (xmax part)
		- Line seed fill of generic polygons
		- Pineda's algorithm for triangle rasterization (min-max mechanism)
*/

#include <iostream>

namespace {
	const std::string BitmapFontFilename = "bitmapfont.png";
	const std::string DefaultAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:.[],-";
}

int main(int argc, char** argv) {
    try {
        Utils::InitRandom();
        SDL_Window* window;
        SDL_Renderer* renderer;
        Utils::InitWindowRenderer("Interpolation, clipping, rasterization", window, renderer, 1000, 600);
        Runnable::SetupFont(std::make_unique<BitmapFont>(renderer, BitmapFontFilename, 16, DefaultAlphabet));

        AppSelector d(window, renderer);
        d.Start();

        Utils::DestroyWindowRenderer(window, renderer); 
    } 
    catch (std::exception& ex) {
        std::cout << "Exeption: " << ex.what() << std::endl;
    }
    return 0;
}
