#include "DDABresenhamLineInterpolation.h"

DDABresenhamLineInterpolation::DDABresenhamLineInterpolation(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : RasterGridRunnable(font, w, r, 5)
{
}

void DDABresenhamLineInterpolation::DrawContent() const
{
    DrawGrid();
    DrawAppInfo();

    if (NumberOfFilledPoints() == 0) {
        DrawPointMousePosition();
        return;
    }

    const bool stepMode = NumberOfFilledPoints() == 2u && IsStepMode();
    const auto p1 = GetPoint(0).ToPoint();
    const auto p2 = GetPoint(1).ToPoint();

    if (m_ddaInterpolation) {
        DrawDDA(p1.x, p1.y, p2.x, p2.y, stepMode);
    } else {
        DrawBresenham(p1.x, p1.y, p2.x, p2.y, stepMode);
    }
}

bool DDABresenhamLineInterpolation::HandleKeyPress(const SDL_Keycode& kc)
{
    if (RasterGridRunnable::HandleKeyPress(kc)) {
        // pass
    } else if (kc == SDLK_w) {
        m_ddaInterpolation = !m_ddaInterpolation;
    } else {
        return false;
    }
    return true;
}

void DDABresenhamLineInterpolation::DrawAppInfo() const
{
    std::stringstream ss;
    ss << GetAppInfo();
    ss << "[W] LINE ALGORITHM: " << (m_ddaInterpolation ? "DDA" : "BRESENHAM") << '\n';
    GetFont().DrawText(ss.str(), 0, 0);
}

void DDABresenhamLineInterpolation::DrawDDA(int x0, int y0, int x1, int y1, bool stepMode) const
{
    float dx = static_cast<float>(x1 - x0);
    float dy = static_cast<float>(y1 - y0);
    const float fdx = fabs(dx);
    const float fdy = fabs(dy);
    float step = (fdx >= fdy ? fdx : fdy);

    // normalize
    dx /= step;
    dy /= step;

    // draw line
    float x = static_cast<float>(x0);
    float y = static_cast<float>(y0);

    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    for (int i = 0; i <= step && steps > 0; i += GetPointSize(), steps--) {
        DrawPoint(static_cast<int>(x), static_cast<int>(y));
        x += dx * GetPointSize();
        y += dy * GetPointSize();
    }
}

void DDABresenhamLineInterpolation::DrawBresenham(int x0, int y0, int x1, int y1, bool stepMode) const
{
    bool swapcoord = false;

    // Determine K, we cannot go under K < 1
    if (abs(y0 - y1) > abs(x0 - x1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        swapcoord = true;
    }

    int startx = x0, endx = x1, starty = y0;
    float dx = static_cast<float>(x1 - x0);
    float dy = static_cast<float>(y1 - y0);

    if (x0 > x1) {
        startx = x1;
        endx = x0;
        starty = y1;
        dy = -dy;
    }

    float de = fabs(dy / dx);
    float e = 0;
    int addy = (dy < 0 ? -1 : 1) * GetPointSize();

    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    for (int x = startx, y = starty; x <= endx && steps > 0; x += GetPointSize(), steps--) {
        if (!swapcoord) {
            DrawPoint(x, y);
        } else {
            DrawPoint(y, x);
        }
        e += de;

        if (e > 0.5f) {
            y += addy;
            e--;
        }
    }
}
