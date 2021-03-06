#include "FramebufferRunnable.h"

FramebufferRunnable::FramebufferRunnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r, uint pointSize)
    : RasterGridRunnable(font, w, r, pointSize)
    , m_drawArea({ 0, 0, GetWindowWidth(), GetWindowHeight() })
{
    ClearFramebuffer();
}

FramebufferRunnable::FramebufferRunnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r,
    const SDL_Rect& drawArea, uint pointSize)
    : RasterGridRunnable(font, w, r, pointSize)
    , m_drawArea(drawArea)
{
    const auto ww = GetWindowWidth();
    const auto wh = GetWindowHeight();

    if (drawArea.x >= ww || drawArea.y >= wh) {
        throw std::runtime_error("Draw area invalid position");
    }
    if (drawArea.x + drawArea.w >= ww || drawArea.y + drawArea.h >= wh) {
        throw std::runtime_error("Draw area invalid proportions");
    }

    ClearFramebuffer();
}

bool FramebufferRunnable::HandleKeyPress(const SDL_Keycode& kc)
{
    const bool res = RasterGridRunnable::HandleKeyPress(kc);
    if (kc == SDLK_c) {
        ClearFramebuffer();
        return true;
    }
    return res;
}

bool FramebufferRunnable::HandleMouseClick(uint8_t button, int x, int y)
{
    SDL_Point p = { x, y };
    if (SDL_PointInRect(&p, &m_drawArea) == SDL_TRUE) {
        return RasterGridRunnable::HandleMouseClick(button, x, y);
    }
    return false;
}

bool FramebufferRunnable::HandleMouseMotion(int x, int y)
{
    SDL_Point p = { x, y };
    if (SDL_PointInRect(&p, &m_drawArea) == SDL_TRUE) {
        return RasterGridRunnable::HandleMouseMotion(x, y);
    }
    return false;
}

void FramebufferRunnable::DrawLinesFan(bool connectFirstLast, uint lastPointSize) const
{
    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), GetR(FILL_COLOR), GetG(FILL_COLOR), GetB(FILL_COLOR), GetA(FILL_COLOR));

    auto drawPoint = [](SDL_Renderer* rend, int x, int y, int size) {
        Utils::DrawPoint(rend, x + size - (x % size), y + size - (y % size), size);
    };

    for (uint i = 1; i <= NumberOfFilledPoints(); i++) {
        const auto p0 = GetPoint(i - 1).ToPoint();
        const auto p1 = GetPoint(i).ToPoint();
        Utils::DrawLineGeneric(GetRenderer(), drawPoint, p0.x, p0.y, p1.x, p1.y, GetPointSize(), 1.f);
    }

    const auto l = GetPoint(NumberOfFilledPoints()).ToPoint();

    if (connectFirstLast) {
        const auto f = GetPoint(0).ToPoint();
        Utils::DrawLineGeneric(GetRenderer(), drawPoint, f.x, f.y, l.x, l.y, GetPointSize(), 1.f);
    }

    if (lastPointSize > GetPointSize()) {
        drawPoint(GetRenderer(), l.x, l.y, lastPointSize);
    }

    Utils::PopColor(GetRenderer());
}

void FramebufferRunnable::FillPolygonLinesIntoFramebuffer()
{
    ClearRenderer();
    DrawLinesFan(true);

    const int ptSize = GetPointSize();
    const auto pixels = std::unique_ptr<Color[]>(new Color[m_drawArea.w * m_drawArea.h]);

    SDL_RenderReadPixels(GetRenderer(), &m_drawArea, SDL_PIXELFORMAT_RGBA8888,
        reinterpret_cast<void*>(&pixels[0]), m_drawArea.w * sizeof(Color));

    // Take one pixel from raster cell
    for (int y = 0; y < m_drawArea.h; y += ptSize) {
        for (int x = 0; x < m_drawArea.w; x += ptSize) {
            m_framebuffer[x / ptSize][y / ptSize] = pixels[y * m_drawArea.w + x];
        }
    }

    m_polygonLinesFilled = true;
}

void FramebufferRunnable::ClearFramebuffer()
{
    const auto w = m_drawArea.w / GetPointSize();
    const auto h = m_drawArea.h / GetPointSize();
    m_framebuffer = Framebuffer(w, FramebufferColumn(h, NONE_COLOR));
    m_polygonLinesFilled = false;
}

void FramebufferRunnable::DrawFramebufferBorder() const
{
    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), 0, 0, 0, 255);
    SDL_Rect border = { m_drawArea.x - 1, m_drawArea.y - 1, m_drawArea.w + 2, m_drawArea.h + 2 };
    SDL_RenderDrawRect(GetRenderer(), &border);
    Utils::PopColor(GetRenderer());
}

void FramebufferRunnable::DrawFramebuffer() const
{
    Utils::PushColor(GetRenderer());

    const auto ptSize = GetPointSize();
    Color currentColor = NONE_COLOR;
    size_t columnIndex = 0u;

    for (const auto& columns : m_framebuffer) {
        size_t rowIndex = 0u;
        for (const auto& c : columns) {
            if (c != currentColor) {
                SDL_SetRenderDrawColor(GetRenderer(), GetR(c), GetG(c), GetB(c), GetA(c));
                currentColor = c;
            }
            DrawPoint(columnIndex * ptSize + m_drawArea.x, rowIndex * ptSize + m_drawArea.y);
            rowIndex++;
        }
        columnIndex++;
    }

    Utils::PopColor(GetRenderer());
}
