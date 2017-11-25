#include "DDABresenhamLineInterpolation.h"

DDABresenhamLineInterpolation::DDABresenhamLineInterpolation(SDL_Window* w, SDL_Renderer* r)
	: RasterGridRunnable(w, r, 5, 2),
	m_ddaEnabled(true)
{
}

bool DDABresenhamLineInterpolation::HandleKeyPress(const SDL_Keycode& kc)
{
	if (RasterGridRunnable::HandleKeyPress(kc)) {
	}
	else if (kc == SDLK_w) {
		m_ddaEnabled = !m_ddaEnabled;
	}
	else {
		return false;
	}
	return true;
}

void DDABresenhamLineInterpolation::DrawAppInfo() const
{
	auto ss = GetAppInfo();
	ss << "[W] LINE ALGORITHM: " << (m_ddaEnabled ? "DDA\n" : "BRESENHAM\n");
	DrawText(ss.str(), 18, 0, 0);
}

void DDABresenhamLineInterpolation::DrawDDA(int x0, int y0, int x1, int y1, bool stepMode) const
{
	float dx = static_cast<float>(x1 - x0);
	float dy = static_cast<float>(y1 - y0);
	float fdx = fabs(dx);
	float fdy = fabs(dy);
	float step = (fdx >= fdy ? fdx : fdy);

	// normalize
	dx /= step;
	dy /= step;

	// draw line
	float x = static_cast<float>(x0);
	float y = static_cast<float>(y0);

	auto steps = stepMode ? GetCurrentStep() : INT_MAX;

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

	auto steps = stepMode ? GetCurrentStep() : INT_MAX;

	for (int x = startx, y = starty; x <= endx && steps > 0; x += GetPointSize(), steps--) {
		if (!swapcoord) {
			DrawPoint(x, y);
		}
		else {
			DrawPoint(y, x);
		}
		e += de;

		if (e > 0.5f) {
			y += addy;
			e--;
		}
	}
}

void DDABresenhamLineInterpolation::DrawContent() const
{
	DrawGrid();
	DrawAppInfo();

	if (NumberOfFilledPoints() > 0) {
		bool stepMode = NumberOfFilledPoints() == 2 && IsStepMode();
		auto&& p1 = GetPoint(0).ToPoint();
		auto&& p2 = GetPoint(1).ToPoint();

		if (m_ddaEnabled) {
			DrawDDA(p1.x, p1.y, p2.x, p2.y, stepMode);
		}
		else {
			DrawBresenham(p1.x, p1.y, p2.x, p2.y, stepMode);
		}
	}
	else {
		DrawPointOnMousePosition();
	}
}