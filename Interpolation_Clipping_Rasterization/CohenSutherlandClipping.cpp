#include "CohenSutherlandClipping.h"

CohenSutherlandClipping::CohenSutherlandClipping(SDL_Window* w, SDL_Renderer* r)
	: RasterGridRunnable(w, r, 2, 2)
{
	m_clippingWindow.x = GetWindowWidth() / 2 - WINDOW_WIDTH / 2;
	m_clippingWindow.y = GetWindowHeight() / 2 - WINDOW_HEIGHT / 2;
	m_clippingWindow.w = WINDOW_WIDTH;
	m_clippingWindow.h = WINDOW_HEIGHT;
}

CohenSutherlandClipping::CollisionCode CohenSutherlandClipping::GetPositionCode(const Point& p) const
{
	int wx = m_clippingWindow.x;
	int wy = m_clippingWindow.y;
	int ww = m_clippingWindow.w;
	int wh = m_clippingWindow.h;

	// Maybe a little bit overcomplicated...
	if (p.x < wx && p.y < wy) {
		return TOP_LEFT;
	}
	else if (p.x > wx + ww && p.y < wy) {
		return TOP_RIGHT;
	}
	else if (p.x > wx && p.x < wx + ww && p.y < wy) {
		return TOP_MID;
	}
	else if (p.x < wx && p.y > wy + wh) {
		return BOT_LEFT;
	}
	else if (p.x > wx + ww && p.y > wy + wh) {
		return BOT_RIGHT;
	}
	else if (p.x > wx && p.x < wx + ww && p.y > wy + wh) {
		return BOT_MID;
	}
	else if (p.x < wx && p.y > wy && p.y < wy + wh) {
		return MID_LEFT;
	}
	else if (p.x > wx + ww && p.y > wy && p.y < wy + wh) {
		return MID_RIGHT;
	}
	else {
		return MID;
	}

	/*
	// This code not work properly...
	unsigned char c = 0;
	if (p.x < wx) {
	c |= MID_LEFT;
	}
	else if (p.x > wx + ww) {
	c |= MID_RIGHT;
	}
	else if (p.y < wy) {
	c |= BOT_MID;
	}
	else if (p.y > wy + wh) {
	c |= TOP_MID;
	}
	return static_cast<Code>(c);
	*/
}

CohenSutherlandClipping::Point CohenSutherlandClipping::ClipSecondEndPoint(Point p, Point q, unsigned int& steps) const
{
	Point p1 = p;
	if (GetPositionCode(q) == MID) {
		return q; // Q is inside
	}
	else if ((GetPositionCode(q) & GetPositionCode(p)) != MID) {
		return p; // both are outside
	}
	else {
		while (p.GetPointDistance(q) > 2.f && steps > 0) {
			auto&& m = p.CountPointMiddle(q);
			if ((GetPositionCode(m) & GetPositionCode(q)) != MID) {
				if ((GetPositionCode(m) & GetPositionCode(p)) != MID) {
					q = p1;
					break;
				}
				else {
					q = m;
					steps--;
				}
			}
			else {
				p = m;
				steps--;
			}
		}
	}
	return q;
}

CohenSutherlandClipping::Line CohenSutherlandClipping::ClipLine(const Point & p, const Point & q, bool stepMode) const
{
	auto steps = stepMode ? GetCurrentStep() : INT_MAX;

	// Clip from the left
	auto&& qm = ClipSecondEndPoint(p, q, steps);

	if (p == q) {
		// Whole line was clipped (rejected) 
		return { GetInvalidPoint(), GetInvalidPoint() };
	}
	else {
		// Clip from the right
		auto&& pm = ClipSecondEndPoint(q, p, steps);
		return { pm, qm };
	}
}

void CohenSutherlandClipping::DrawAppInfo() const
{
	DrawText(GetAppInfo().str(), 18, 0, 0);
}

void CohenSutherlandClipping::DrawClippingWindow() const
{
	Utils::PushColor(GetRenderer());
	SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);
	Utils::DrawRectangle(GetRenderer(), m_clippingWindow, WINDOW_BORDER_THICKNESS, false);
	Utils::PopColor(GetRenderer());
}

void CohenSutherlandClipping::DrawContent() const
{
	DrawAppInfo();

	if (AllPointsFilled()) {
		auto&& line = ClipLine(GetPoint(0), GetPoint(1), IsStepMode());
		auto&& f = line.first.ToPoint();
		auto&& s = line.second.ToPoint();
		auto ptSize = GetPointSize();
		Utils::DrawLine(GetRenderer(), f.x, f.y, s.x, s.y, ptSize);
		Utils::DrawPoint(GetRenderer(), f.x - ptSize, f.y - ptSize, ptSize * 2);
		Utils::DrawPoint(GetRenderer(), s.x - ptSize, s.y - ptSize, ptSize * 2);
	}
	else {
		DrawLinesFan(false, GetPointSize() * 2);
	}

	DrawClippingWindow();
}