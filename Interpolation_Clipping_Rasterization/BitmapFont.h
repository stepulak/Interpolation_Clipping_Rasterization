#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include <array>
#include <SDL2/SDL.h>

// Simple bitmap font with variable alphabet loaded from "one-line" image
class BitmapFont {
private:

	static const int CHAR_WIDTH_OFFSET = -8;
	static const int ALPHABET_START = ' ';
	static const int ALPHABET_END = '~';
	static constexpr int ALPHABET_SIZE = ALPHABET_END - ALPHABET_START + 1;

	std::array<int, ALPHABET_SIZE> m_alphabet;
	int m_bitmapHeight;
	int m_bitmapCharWidth;
	SDL_Texture* m_bitmap;
	SDL_Renderer* m_renderer;

	inline bool InsideAlphabet(char c) const { return c >= ALPHABET_START && c <= ALPHABET_END; }
	inline int GetCharWidthFontSize(int fontSize) const { return fontSize + CHAR_WIDTH_OFFSET; }

public:
	
	BitmapFont(const BitmapFont&) = delete;
	BitmapFont(BitmapFont&&) = delete;

	BitmapFont& operator=(const BitmapFont&) = delete;
	BitmapFont& operator=(BitmapFont&&) = delete;

	BitmapFont(SDL_Renderer* rend, const std::string& path, int charWidth, const std::string& alphabet);

	virtual ~BitmapFont();

	inline int GetBitmapCharWidth() const { return m_bitmapCharWidth; }
	inline SDL_Renderer* GetRenderer() { return m_renderer; }
	inline void SetRenderer(SDL_Renderer* rend) { m_renderer = rend; }
	
	int GetLineWidth(const std::string& line, unsigned int fontSize) const;

	void DrawLine(const std::string& line, unsigned int fontSize, int posX, int posY, bool isCentered = false) const;
};

#endif