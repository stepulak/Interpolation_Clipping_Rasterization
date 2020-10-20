#include "BitmapFont.h"
#include "Utils.h"

#include <cstring>

BitmapFont::BitmapFont(SDL_Renderer* rend, const std::string& path, int charWidth, const std::string& alphabet)
    : m_renderer(rend)
    , m_bitmapCharWidth(charWidth)
    , m_bitmap(Utils::LoadTexture(rend, path, nullptr, &m_bitmapHeight))
{
    if (charWidth <= 0) {
        throw std::runtime_error("charWidth must be positive!");
    }
    std::memset(m_alphabet.data(), -1, m_alphabet.size() * sizeof(m_alphabet.front()));

    int pos = 0;

    for (const auto& c : alphabet) {
        if (InsideVisibleAlphabet(c)) {
            m_alphabet[c - VISIBLE_ALPHABET_START] = pos;
            pos += charWidth;
        }
    }
}

BitmapFont::~BitmapFont()
{
    SDL_DestroyTexture(m_bitmap);
}

void BitmapFont::DrawText(const std::string& line, int x, int y, int fontSize) const
{
    Utils::PushColor(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

    SDL_Rect dest = { x, y, static_cast<int>(fontSize * 0.66f), fontSize };
    SDL_Rect src = { 0, 0, m_bitmapCharWidth, m_bitmapHeight };

    for (const auto& c : line) {
        // In range and inside alphabet?
        if (InsideVisibleAlphabet(c) && m_alphabet[c - VISIBLE_ALPHABET_START] != -1) {
            src.x = m_alphabet[c - VISIBLE_ALPHABET_START];
            SDL_RenderCopyEx(m_renderer, m_bitmap, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
            dest.x += fontSize;
        } else if (c == ' ') {
            dest.x += fontSize;
        } else if (c == '\n') {
            dest.y += fontSize;
            dest.x = x;
        }
    }

    Utils::PopColor(m_renderer);
}