#pragma once

#include <SDL2/SDL.h>
#include <array>

// Simple bitmap font with alphabet loaded from "one-line" image
class BitmapFont {
public:
    BitmapFont(SDL_Renderer* rend, const std::string& path, int charWidth, const std::string& alphabet);

    BitmapFont(const BitmapFont&) = delete;
    BitmapFont(BitmapFont&&) = delete;

    BitmapFont& operator=(const BitmapFont&) = delete;
    BitmapFont& operator=(BitmapFont&&) = delete;

    virtual ~BitmapFont();

    SDL_Renderer* GetRenderer()
    {
        return m_renderer;
    }

    void DrawText(const std::string& line, int x, int y, int fontSize = 14) const;

private:
    static constexpr auto VISIBLE_ALPHABET_START = ' ';
    static constexpr auto VISIBLE_ALPHABET_END = '~';
    static constexpr auto VISIBLE_ALPHABET_SIZE = VISIBLE_ALPHABET_END - VISIBLE_ALPHABET_START + 1u;

    std::array<int, VISIBLE_ALPHABET_SIZE> m_alphabet;
    // int due to SDL2
    int m_bitmapHeight;
    int m_bitmapCharWidth;
    SDL_Texture* m_bitmap;
    SDL_Renderer* m_renderer;

    bool InsideVisibleAlphabet(char c) const
    {
        return c >= VISIBLE_ALPHABET_START && c <= VISIBLE_ALPHABET_END;
    }
};
