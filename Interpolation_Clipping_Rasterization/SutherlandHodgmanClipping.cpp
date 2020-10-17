#include "SutherlandHodgmanClipping.h"

SutherlandHodgmanClipping::SutherlandHodgmanClipping(SDL_Window* w, SDL_Renderer* r)
	: RasterGridRunnable(w, r, 2)
{
	Clear();
}

bool SutherlandHodgmanClipping::HandleMouseClick(uint8_t button, int x, int y)
{
	if (!m_polygonInserted) {
		return RasterGridRunnable::HandleMouseClick(button, x, y);
	}
	else if (!m_clippingLineInserted) {
		m_clippingLineInserted = true;
		m_clippingLineX = x;
		return true;
	}
	return false;
}

bool SutherlandHodgmanClipping::HandleMouseMotion(int x, int y)
{
	if (!m_polygonInserted) {
		return RasterGridRunnable::HandleMouseMotion(x, y);
	}
	else if (!m_clippingLineInserted) {
		m_clippingLineX = x;
		return true;
	}
	return false;
}

bool SutherlandHodgmanClipping::HandleKeyPress(const SDL_Keycode& kc)
{
	bool res = RasterGridRunnable::HandleKeyPress(kc);

	if (kc == SDLK_c) { // clear
		Clear();
	}
	else if (kc == SDLK_w) { // write down polygon
		if (!m_polygonInserted && NumberOfFilledPoints() > 1) {
			m_polygonInserted = true;
			PushBackPoint(GetPoint(0));
		}
	}
	else if (kc == SDLK_s) {
		m_nextStep = true;
	}
	else {
		return res;
	}

	return true;
}

void SutherlandHodgmanClipping::FindNextIntersection()
{
	for (uint i = m_lastPoint; i < NumberOfFilledPoints(); i++) {
		auto& p0 = GetPoint(i);
		auto& p1 = GetPoint(i + 1);

		if ((p1.x > m_clippingLineX && p0.x < m_clippingLineX) || (p0.x > m_clippingLineX && p1.x < m_clippingLineX)) {
			// find intersection
			auto&& pIntersec = Utils::FindLineIntersetion(p0, p1 - p0, Point(1.f * m_clippingLineX, 0.f), Vec(0.f, -1.f));
			InsertPointAt(pIntersec, i + 1);
			m_intersectionFound = true;
			m_lastPoint = i + 1;
			return;
		}
	}
	// no more intersections found
	m_intersectionFound = false;
	m_lastPoint = 0;
}

void SutherlandHodgmanClipping::ClipNextPoint()
{
	for (uint i = m_lastPoint; i < NumberOfFilledPoints(); i++) {
		if (GetPoint(i).x > m_clippingLineX) {
			// clip the point
			RemovePoint(i);
			m_pointClipped = true;
			m_lastPoint = i;
			return;
		}
	}
	m_pointClipped = false;
	m_lastPoint = 0;
}

void SutherlandHodgmanClipping::DrawAppInfo() const
{
	auto ss = GetAppInfo();
	ss << "[W] SAVE POLY. VERTICES\n";
	DrawText(ss.str(), 18, 0, 0);
}

void SutherlandHodgmanClipping::Clear()
{
	m_polygonInserted = false;
	m_clippingLineInserted = false;
	m_clippingLineX = 0;
	m_nextStep = false;
	m_intersectionFound = true;
	m_pointClipped = true;
	m_lastPoint = 0;
	m_drawClippingLine = true;
}

void SutherlandHodgmanClipping::UpdateContent()
{
	if (!m_polygonInserted || !m_clippingLineInserted) {
		return;
	}

	if (IsStepMode()) {
		if (m_nextStep) {
			m_nextStep = false;

			if (m_intersectionFound) {
				FindNextIntersection();
			}
			else if (m_pointClipped) {
				ClipNextPoint();
			}
			else {
				m_drawClippingLine = false;
			}
		}
	}
	else {
		while (m_intersectionFound) {
			FindNextIntersection();
		}
		while (m_pointClipped) {
			ClipNextPoint();
		}
		m_drawClippingLine = false;
	}
}

void SutherlandHodgmanClipping::DrawContent() const
{
	DrawAppInfo();

	Utils::PushColor(GetRenderer());
	SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);

	DrawLinesFan(!m_polygonInserted);
	DrawPoints(GetPointSize() * 4);

	if (m_polygonInserted && m_drawClippingLine) {
		SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 255, 255);
		Utils::DrawLine(GetRenderer(), m_clippingLineX, 0, m_clippingLineX, GetWindowHeight(), CLIPPING_LINE_THICKNESS);
	}

	Utils::PopColor(GetRenderer());
}
