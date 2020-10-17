#pragma once

#include "RasterGridRunnable.h"

// Primitive framebuffer binded to point size
// You see, we could have used GPU framebuffer,
// but accessing directly into it very often would be expensive...
class FramebufferRunnable : public RasterGridRunnable {
public:
    enum Color {
        NONE_COLOR = 0x0,
        BLACK = 0xff,
        WHITE = 0xffffffff,
        PURPLE = 0xff00ffff,
        FILL_COLOR = PURPLE,
    };

    bool PolygonLinesFilled() const
    {
        return m_polygonLinesFilled;
    }

    size_t FramebufferWidth() const
    {
        return m_framebuffer.size();
    }

    size_t FramebufferHeight() const
    {
        return m_framebuffer[0].size();
    }

    const SDL_Rect& GetDrawArea() const
    {
        return m_drawArea;
    }

    Color GetColor(int x, int y) const
    {
        return m_framebuffer[x][y];
    }

    void SetColor(int x, int y, Color c)
    {
        m_framebuffer[x][y] = c;
    }

    // Get point with coordinates related to buffer's draw area
    Point GetPointLocal(size_t index) const
    {
        return GetPoint(index) - Point(static_cast<float>(m_drawArea.x), static_cast<float>(m_drawArea.y));
    }

    // Return point at given index as cell's coordinates related to buffer's draw area and point size
    SDL_Point GetPointAsIndex(size_t index) const
    {
        return (GetPointLocal(index) / (static_cast<float>(GetPointSize()))).ToPoint();
    }

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    bool HandleMouseClick(uint8_t button, int x, int y) override;
    bool HandleMouseMotion(int x, int y) override;
    void DrawLinesFan(bool connectFirstLast = false, uint lastPointSize = 0) const override;

    void FillPolygonLinesIntoFramebuffer();
    void ClearFramebuffer();
    void DrawFramebufferBorder() const;
    void DrawFramebuffer() const;

protected:
    FramebufferRunnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r, uint pointSize);

    FramebufferRunnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r,
        const SDL_Rect& drawArea, uint pointSize);

private:
    using FramebufferColumn = std::vector<Color>;
    using Framebuffer = std::vector<FramebufferColumn>;

    const SDL_Rect m_drawArea;
    Framebuffer m_framebuffer;
    bool m_polygonLinesFilled;
    
    uint8_t GetR(Color c) const
    {
        return c >> 24;
    }

    uint8_t GetG(Color c) const
    {
        return (c >> 16) & 0xff;
    }

    uint8_t GetB(Color c) const
    {
        return (c >> 8) & 0xff;
    }

    uint8_t GetA(Color c) const
    {
        return c & 0xff;
    }
};
