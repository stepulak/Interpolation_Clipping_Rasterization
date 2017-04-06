#include "BitmapFont.h"
#include "Utils.h"

BitmapFont::BitmapFont(SDL_Renderer* rend, const std::string& path, int charWidth, const std::string& alphabet)
	: m_renderer(rend),
	m_bitmapCharWidth(charWidth)
{
	m_bitmap = Utils::LoadTexture(rend, path, nullptr, &m_bitmapHeight);

	for (auto& p : m_alphabet) {
		p = -1;
	}

	int currPos = 0;
	for (const auto& c : alphabet) {
		if (InsideAlphabet(c)) {
			m_alphabet[c - ALPHABET_START] = currPos;
			currPos += charWidth;
		}
	}
}

BitmapFont::~BitmapFont()
{
	SDL_DestroyTexture(m_bitmap);
}


int BitmapFont::GetLineWidth(const std::string& line, unsigned int fontSize) const
{
	unsigned int width = 0;
	for (const auto& c : line) {
		if (InsideAlphabet(c)) {
			width += GetCharWidthFontSize(fontSize);
		}
	}
	return width;
}

void BitmapFont::DrawLine(const std::string& line, unsigned int fontSize, int posX, int posY, bool isCentered) const
{
	Utils::PushColor(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	SDL_Rect dest = { posX, posY, GetCharWidthFontSize(fontSize), static_cast<int>(fontSize) };
	SDL_Rect src = { 0, 0, m_bitmapCharWidth, m_bitmapHeight };

	for (const auto& c : line) {
		// In range and inside alphabet?
		if (InsideAlphabet(c) && m_alphabet[c - ALPHABET_START] != -1) {
			src.x = m_alphabet[c - ALPHABET_START];
			SDL_RenderCopyEx(m_renderer, m_bitmap, &src, &dest, 0, nullptr, SDL_FLIP_NONE);
			dest.x += fontSize;
		}
		else if (c == ' ') {
			dest.x += fontSize;
		}
		else if (c == '\n') {
			dest.y += fontSize;
			dest.x = posX;
		}
	}

	Utils::PopColor(m_renderer);
}