#include "AppSelector.h"

#include <iostream>

namespace {
constexpr auto FONT_BITMAP_FILENAME = "bitmapfont.png";
constexpr auto FONT_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:.[],-";
constexpr auto FONT_CHAR_WIDTH = 16u;
constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_HEIGHT = 768;
} // namespace

int main(int argc, char** argv)
{
    try {
        Utils::InitRandom();

        auto [window, renderer] = Utils::InitWindowRenderer(
            "Interpolation, clipping, rasterization...", WINDOW_WIDTH, WINDOW_HEIGHT);

        BitmapFont font(renderer, FONT_BITMAP_FILENAME, FONT_CHAR_WIDTH, FONT_ALPHABET);
        AppSelector selector(font, window, renderer);

        selector.Run();

        Utils::DestroyWindowRenderer(window, renderer);
    } catch (const std::exception& ex) {
        std::cerr << "Exeption: " << ex.what() << std::endl;
        std::cerr << "Exiting..." << std::endl;
        return -1;
    }
    return 0;
}
