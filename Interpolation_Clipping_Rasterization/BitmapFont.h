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

    int GetBitmapCharWidth() const
    {
        return m_bitmapCharWidth;
    }

    SDL_Renderer* GetRenderer()
    {
        return m_renderer;
    }

    void SetRenderer(SDL_Renderer* rend)
    {
        m_renderer = rend;
    }

    int GetLineWidth(const std::string& line, int fontSize) const;
    void DrawLine(const std::string& line, int fontSize, int posX, int posY, bool isCentered = false) const;

private:
    static constexpr auto CHAR_WIDTH_OFFSET = -8;
    static constexpr auto ALPHABET_START = ' ';
    static constexpr auto ALPHABET_END = '~';
    static constexpr auto ALPHABET_SIZE = ALPHABET_END - ALPHABET_START + 1u;

    std::array<int, ALPHABET_SIZE> m_alphabet;
    int m_bitmapHeight;
    int m_bitmapCharWidth;
    SDL_Texture* m_bitmap;
    SDL_Renderer* m_renderer;

    bool InsideAlphabet(char c) const
    {
        return c >= ALPHABET_START && c <= ALPHABET_END;
    }

    int GetCharWidthFontSize(int fontSize) const
    {
        return fontSize + CHAR_WIDTH_OFFSET;
    }
};
