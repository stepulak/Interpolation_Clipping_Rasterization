#include "RasterGridRunnable.h"

RasterGridRunnable::RasterGridRunnable(SDL_Window* w, SDL_Renderer* r, unsigned int pointSize, unsigned int maxPoints)
	: Runnable(w, r),
	m_stepMode(false),
	m_pointSize(pointSize),
	m_maxPoints(maxPoints)
{
	if (pointSize == 0) {
		throw std::runtime_error("Point size cannot be 0");
	}
	if (maxPoints == 0) {
		throw std::runtime_error("Maximum number of input points cannot be 0");
	}
	ClearPoints();
}

void RasterGridRunnable::DrawPointOnMousePosition() const
{
	auto&& p = GetMousePosition();
	DrawPoint(p.x, p.y);
}

void RasterGridRunnable::ClearPoints()
{
	m_points.clear();
	m_points.push_back(GetMousePosition());
}

void RasterGridRunnable::DrawGrid() const
{
	if (m_pointSize > 1) {
		SDL_Rect area = { 0, 0, GetWindowWidth(), GetWindowHeight() };
		Utils::PushColor(GetRenderer());
		SDL_SetRenderDrawColor(GetRenderer(), 200, 200, 200, 255);
		Utils::DrawGrid(GetRenderer(), area, m_pointSize);
		Utils::PopColor(GetRenderer());
	}
}

bool RasterGridRunnable::HandleKeyPress(const SDL_Keycode & kc)
{
	if (Runnable::HandleKeyPress(kc)) {
	}
	else if (kc == SDLK_c) {
		m_currentStep = 0;
		ClearPoints();
	}
	else if (kc == SDLK_s) {
		m_currentStep++;
	}
	else if (kc == SDLK_e) {
		m_stepMode = !m_stepMode;
	}
	else {
		return false;
	}

	return true;
}

bool RasterGridRunnable::HandleMouseClick(Uint8 button, Sint32 x, Sint32 y)
{
	if (m_points.size() <= m_maxPoints) {
		m_points.push_back(Point(1.f * x, 1.f * y));
		return true;
	}
	return false;
}

bool RasterGridRunnable::HandleMouseMotion(Sint32 x, Sint32 y)
{
	if (m_points.size() <= m_maxPoints) {
		auto& p = m_points.back();
		p.x = 1.f * x;
		p.y = 1.f * y;
		return true;
	}
	return false;
}

std::stringstream RasterGridRunnable::GetAppInfo() const
{
	auto&& ss = Runnable::GetAppInfo();
	ss << "[E] STEPMODE: " << (m_stepMode ? "ACTIVE" : "INACTIVE") << '\n';
	ss << "[S] NEXT STEP\n";
	ss << "[C] CLEAR\n";

	int pointIndex = 1;
	for (const auto& p : m_points) {
		if (static_cast<unsigned int>(pointIndex) > m_maxPoints) {
			break;
		}
		auto point = (p / static_cast<float>(m_pointSize)).ToPoint();
		ss << "POINT" << pointIndex << ": [" << p.x << ", " << p.y << "]\n";
		pointIndex++;
	}

	return std::move(ss);
}

void RasterGridRunnable::DrawLinesFan(bool connectFirstLast, unsigned int lastPointSize) const
{
	Utils::DrawLineFan(GetRenderer(), m_points, m_pointSize);
	if (connectFirstLast) {
		const auto&& f = m_points.front().ToPoint();
		const auto&& l = m_points.back().ToPoint();
		Utils::DrawLine(GetRenderer(), f.x, f.y, l.x, l.y, m_pointSize);
	}
	if (lastPointSize > m_pointSize) {
		const auto&& l = m_points.back().ToPoint();
		Utils::DrawPoint(GetRenderer(), l.x - lastPointSize/2, l.y - lastPointSize / 2, lastPointSize);
	}
}

void RasterGridRunnable::DrawPoints(unsigned int pointSize) const
{
	if (pointSize < m_pointSize) {
		pointSize = m_pointSize;
	}

	for (const auto& p : m_points) {
		auto x = static_cast<int>(p.x - pointSize / 2.f);
		auto y = static_cast<int>(p.y - pointSize / 2.f);
		Utils::DrawPoint(GetRenderer(), x, y, pointSize);
	}
}
