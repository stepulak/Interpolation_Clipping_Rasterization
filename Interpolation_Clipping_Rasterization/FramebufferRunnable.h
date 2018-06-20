#pragma once

#include "RasterGridRunnable.h"

// Primitive framebuffer binded to point size
// You see, we could have used GPU framebuffer,
// but accessing directly into it very often would be expensive...
class FramebufferRunnable : public RasterGridRunnable {
protected:

    enum Color {
        NONE_COLOR = 0x0,
        BLACK = 0xff,
        WHITE = 0xffffffff,
        PURPLE = 0xff00ffff,
        FILL_COLOR = PURPLE,
    };
    
private:

	using FramebufferColumn = std::vector<Color>;
	using Framebuffer = std::vector<FramebufferColumn>;

	Framebuffer m_framebuffer;
	bool m_polygonLinesFilled;
	SDL_Rect m_drawArea;

    Uint8 GetR(Color c) const { return c >> 24; }
    Uint8 GetG(Color c) const { return (c >> 16) & 0xff; }
    Uint8 GetB(Color c) const { return (c >> 8) & 0xff; }
    Uint8 GetA(Color c) const { return c & 0xff; }

protected:

    bool PolygonLinesFilled() const { return m_polygonLinesFilled; }
    unsigned int FramebufferWidth() const { return m_framebuffer.size(); }
    unsigned int FramebufferHeight() const { return m_framebuffer[0].size(); }
    const SDL_Rect& GetDrawArea() const { return m_drawArea; }

    Color GetColor(int x, int y) const { return m_framebuffer[x][y]; }
    void SetColor(int x, int y, Color c) { m_framebuffer[x][y] = c; }

	// Get point with coordinates related to buffer's draw area
	Point GetPointLocal(int index) const { return GetPoint(index) - Point(1.f * m_drawArea.x, 1.f * m_drawArea.y); }

	// Return point at given index as cell's coordinates related to buffer's draw area and point size
	SDL_Point GetPointAsIndex(int index) const { return (GetPointLocal(index) / (1.f * GetPointSize())).ToPoint(); }

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    bool HandleMouseClick(Uint8 button, Sint32 x, Sint32 y) override;
    bool HandleMouseMotion(Sint32 x, Sint32 y) override;
    void DrawLinesFan(bool connectFirstLast = false, unsigned int lastPointSize = 0) const override;

	void FillPolygonLinesIntoFramebuffer();
	void ClearFramebuffer();
	void DrawFramebufferBorder() const;
	void DrawFramebuffer() const;

public:

	FramebufferRunnable(SDL_Window* w, SDL_Renderer* r, unsigned int pointSize,
		unsigned int maxPoints = UINT_MAX);

	FramebufferRunnable(SDL_Window* w, SDL_Renderer* r, const SDL_Rect& drawArea,
		unsigned int pointSize, unsigned int maxPoints = UINT_MAX);
};
