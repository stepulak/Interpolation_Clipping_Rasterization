#include "CircleEllipseInterpolation.h"

#include <iostream>

CircleEllipseInterpolation::CircleEllipseInterpolation(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : RasterGridRunnable(font, w, r, 4)
{
}

bool CircleEllipseInterpolation::HandleKeyPress(const SDL_Keycode& kc)
{
    if (RasterGridRunnable::HandleKeyPress(kc)) {
    } else if (kc == SDLK_w) {
        m_circleInterpolation = !m_circleInterpolation;
    } else {
        return false;
    }
    return true;
}

void CircleEllipseInterpolation::DrawAppInfo() const
{
    std::stringstream ss;
    ss << GetAppInfo();
    ss << "[W] SHAPE: " << (m_circleInterpolation ? "CIRCLE" : "ELLIPSE") << '\n';
    GetFont().DrawLine(ss.str(), 18, 0, 0);
}

void CircleEllipseInterpolation::DrawPointCircle8(int x, int y, int dx, int dy) const
{
    DrawPoint(x + dx, y + dy);
    DrawPoint(x + dx, y - dy);
    DrawPoint(x - dx, y + dy);
    DrawPoint(x - dx, y - dy);
    DrawPoint(x + dy, y + dx);
    DrawPoint(x + dy, y - dx);
    DrawPoint(x - dy, y + dx);
    DrawPoint(x - dy, y - dx);
}

void CircleEllipseInterpolation::DrawPointEllipse4(float x, float y, float dx, float dy) const
{
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    int idx = static_cast<int>(dx);
    int idy = static_cast<int>(dy);

    DrawPoint(ix + idx, iy + idy);
    DrawPoint(ix + idx, iy - idy);
    DrawPoint(ix - idx, iy + idy);
    DrawPoint(ix - idx, iy - idy);
}

void CircleEllipseInterpolation::DrawCircle(int cx, int cy, int r, bool stepMode) const
{
    // Bresenham circle (1/8)
    // Drawing via DDA is much more simpler, but this should be faster...
    // I know that circle is specific type of ellipse (when a=b), but I am using another algorithm for this...
    const auto pt = GetPointSize();
    int d = r - 1 * pt;
    int d1 = 3 * pt;
    int d2 = -2 * r + 5 * pt;
    int x = 0, y = r;
    DrawPointCircle8(cx, cy, x, y);

    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    while (y > x && steps > 0) {
        if (d < 0) {
            d += d1;
            d1 += 2 * pt;
            d2 += 2 * pt;
        } else {
            d += d2;
            d1 += 2 * pt;
            d2 += 4 * pt;
            y -= pt;
        }
        x += pt;
        DrawPointCircle8(cx, cy, x, y);
        steps--;
    }
}

void CircleEllipseInterpolation::DrawEllipse(float cx, float cy, float a, float b, bool stepMode) const
{
    // Bresenham

    // The ellipse is divided into two regions
    // Uses float instead of int for better result
    auto aa = a * a;
    auto bb = b * b;
    auto x = 0.0f, y = b;
    auto d = 0.0f, dx = 0.0f;
    auto dy = 2.0f * aa * y;
    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    // A little bit obfuscated code
    // @param xinc increase of x
    // @param yinc increase of y
    auto drawRegion = [&](bool region1, float xinc, float yinc, size_t steps) {
        while (((region1 && dx < dy) || (!region1 && x > 0)) && steps > 0) {
            x += xinc;
            dx += xinc * 2 * bb;

            if ((region1 && d < 0) || (!region1 && d > 0)) {
                d += bb + dx * xinc;
            } else {
                y += yinc;
                dy += yinc * 2 * aa;
                d += bb + dx * xinc + dy * yinc;
            }
            if (region1) {
                DrawPointEllipse4(cx, cy, x, y);
            } else {
                DrawPointEllipse4(cx, cy, y, x);
            }
            steps--;
        }

        return steps;
    };

    // Region1
    DrawPointEllipse4(cx, cy, x, y);

    const float ptSize = static_cast<float>(GetPointSize());
    const auto stepsLeft = drawRegion(true, ptSize, -ptSize, steps);

    // Region 2
    if (stepsLeft > 0) {
        d = aa * y * y + bb * x * x - aa * bb;
        std::swap(aa, bb);
        std::swap(dx, dy);
        std::swap(x, y);
        drawRegion(false, -ptSize, ptSize, stepsLeft);
    }
}

void CircleEllipseInterpolation::DrawContent() const
{
    DrawGrid();
    DrawAppInfo();

    if (NumberOfFilledPoints() == 0) {
        DrawPointMousePosition();
        return;
    }

    const bool stepMode = NumberOfFilledPoints() == 2u && IsStepMode();
    const auto& p1 = GetPoint(0);
    const auto& p2 = GetPoint(1);

    if (m_circleInterpolation) {
        const auto r = SDL_abs(static_cast<int>((p1 - p2).GetLength()));
        DrawCircle(static_cast<int>(p1.x), static_cast<int>(p1.y), r, stepMode);
    } else {
        const float a = static_cast<float>(SDL_fabs(p2.x - p1.x));
        const float b = static_cast<float>(SDL_fabs(p2.y - p1.y));
        DrawEllipse(p1.x, p1.y, a, b, stepMode);
    }
}
