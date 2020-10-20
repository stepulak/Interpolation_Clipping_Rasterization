#include "CohenSutherlandClipping.h"

CohenSutherlandClipping::CohenSutherlandClipping(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : RasterGridRunnable(font, w, r, 2u)
    , m_clippingWindow({
          GetWindowWidth() / 2 - WINDOW_WIDTH / 2,
          GetWindowHeight() / 2 - WINDOW_HEIGHT / 2,
          WINDOW_WIDTH,
          WINDOW_HEIGHT,
      })
{
}

bool CohenSutherlandClipping::HandleMouseClick(uint8_t button, int x, int y)
{
    if (NumberOfFilledPoints() < 2u) {
        return RasterGridRunnable::HandleMouseClick(button, x, y);
    }
    return false;
}

void CohenSutherlandClipping::DrawContent() const
{
    DrawAppInfo();

    if (NumberOfFilledPoints() != 2u) {
        DrawLinesFan(false, GetPointSize() * 2);
    } else {
        const auto line = ClipLine(GetPoint(0), GetPoint(1), IsStepMode());
        const auto f = line.first.ToPoint();
        const auto s = line.second.ToPoint();
        const auto ptSize = GetPointSize();
        Utils::DrawLine(GetRenderer(), f.x, f.y, s.x, s.y, ptSize);
        Utils::DrawPoint(GetRenderer(), f.x - ptSize, f.y - ptSize, ptSize * 2);
        Utils::DrawPoint(GetRenderer(), s.x - ptSize, s.y - ptSize, ptSize * 2);
    }

    DrawClippingWindow();
}

CohenSutherlandClipping::CollisionCode CohenSutherlandClipping::GetPositionCode(const Point& p) const
{
    const auto wx = m_clippingWindow.x;
    const auto wy = m_clippingWindow.y;
    const auto ww = m_clippingWindow.w;
    const auto wh = m_clippingWindow.h;

    // Maybe a little bit overcomplicated...
    if (p.x < wx && p.y < wy) {
        return TOP_LEFT;
    }
    if (p.x > wx + ww && p.y < wy) {
        return TOP_RIGHT;
    }
    if (p.x > wx && p.x < wx + ww && p.y < wy) {
        return TOP_MID;
    }
    if (p.x < wx && p.y > wy + wh) {
        return BOT_LEFT;
    }
    if (p.x > wx + ww && p.y > wy + wh) {
        return BOT_RIGHT;
    }
    if (p.x > wx && p.x < wx + ww && p.y > wy + wh) {
        return BOT_MID;
    }
    if (p.x < wx && p.y > wy && p.y < wy + wh) {
        return MID_LEFT;
    }
    if (p.x > wx + ww && p.y > wy && p.y < wy + wh) {
        return MID_RIGHT;
    }
    return MID;
}

CohenSutherlandClipping::Point CohenSutherlandClipping::ClipSecondEndPoint(Point p, Point q, size_t& steps) const
{
    if (GetPositionCode(q) == MID) {
        return q; // Q is inside
    }
    if ((GetPositionCode(q) & GetPositionCode(p)) != MID) {
        return p; // both are outside
    }

    while (p.GetPointDistance(q) > 2.f && steps > 0) {
        const auto m = p.CountPointMiddle(q);
        if ((GetPositionCode(m) & GetPositionCode(q)) != MID) {
            if ((GetPositionCode(m) & GetPositionCode(p)) != MID) {
                q = p;
                break;
            }
            q = m;
            steps--;
        } else {
            p = m;
            steps--;
        }
    }
    return q;
}

CohenSutherlandClipping::Line CohenSutherlandClipping::ClipLine(const Point& p, const Point& q, bool stepMode) const
{
    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    // Clip from the left
    const auto qm = ClipSecondEndPoint(p, q, steps);

    if (p == q) {
        // Whole line was clipped (rejected)
        return { INVALID_POINT, INVALID_POINT };
    }

	// Clip from the right
	const auto pm = ClipSecondEndPoint(q, p, steps);
	return { pm, qm };
}

void CohenSutherlandClipping::DrawAppInfo() const
{
    GetFont().DrawText(GetAppInfo(), 0, 0);
}

void CohenSutherlandClipping::DrawClippingWindow() const
{
    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);
    Utils::DrawRectangle(GetRenderer(), m_clippingWindow, WINDOW_BORDER_THICKNESS, false);
    Utils::PopColor(GetRenderer());
}
