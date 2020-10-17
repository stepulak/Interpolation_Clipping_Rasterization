#include "RasterGridRunnable.h"

RasterGridRunnable::RasterGridRunnable(
    const BitmapFont& font,
    SDL_Window* w,
    SDL_Renderer* r,
    uint pointSize)
    : Runnable(font, w, r)
    , m_pointSize(pointSize)
{
    if (pointSize == 0) {
        throw std::runtime_error("Point size cannot be 0");
    }
    ClearPoints();
}

void RasterGridRunnable::DrawPointMousePosition() const
{
    const auto p = GetMousePosition();
    DrawPoint(p.x, p.y);
}

void RasterGridRunnable::ClearPoints()
{
    m_points.clear();
    m_points.push_back(GetMousePosition());
}

void RasterGridRunnable::DrawGrid() const
{
    if (m_pointSize <= 1) {
        return;
    }
    SDL_Rect area = { 0, 0, GetWindowWidth(), GetWindowHeight() };
    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), 200, 200, 200, 255);
    Utils::DrawGrid(GetRenderer(), area, m_pointSize);
    Utils::PopColor(GetRenderer());
}

bool RasterGridRunnable::HandleKeyPress(const SDL_Keycode& kc)
{
    if (Runnable::HandleKeyPress(kc)) {
        // pass
    } else if (kc == SDLK_c) {
        m_currentStep = 0;
        ClearPoints();
    } else if (kc == SDLK_s) {
        m_currentStep++;
    } else if (kc == SDLK_e) {
        m_stepMode = !m_stepMode;
    } else {
        return false;
    }

    return true;
}

bool RasterGridRunnable::HandleMouseClick(uint8_t button, int x, int y)
{
    m_points.emplace_back(x, y);
    return true;
}

bool RasterGridRunnable::HandleMouseMotion(int x, int y)
{
    auto& p = m_points.back();
    p.x = static_cast<float>(x);
    p.y = static_cast<float>(y);
    return true;
}

std::string RasterGridRunnable::GetAppInfo() const
{
    std::stringstream ss;

    ss << Runnable::GetAppInfo();
    ss << "[E] STEPMODE: " << (m_stepMode ? "ACTIVE" : "INACTIVE") << '\n';
    ss << "[S] NEXT STEP\n";
    ss << "[C] CLEAR\n";

    size_t pointIndex = 1u;
    for (const auto& p : m_points) {
        const auto point = (p / static_cast<float>(m_pointSize)).ToPoint();
        ss << "POINT" << pointIndex << ": [" << p.x << ", " << p.y << "]\n";
        pointIndex++;
    }

    return ss.str();
}

void RasterGridRunnable::DrawPoint(int x, int y) const
{
    Utils::DrawPoint(GetRenderer(), x - (x % m_pointSize), y - (y % m_pointSize), m_pointSize);
}

void RasterGridRunnable::DrawPoints(uint pointSize) const
{
    if (pointSize < m_pointSize) {
        pointSize = m_pointSize;
    }

    for (const auto& p : m_points) {
        const auto x = static_cast<int>(p.x - pointSize / 2.f);
        const auto y = static_cast<int>(p.y - pointSize / 2.f);
        Utils::DrawPoint(GetRenderer(), x, y, pointSize);
    }
}

void RasterGridRunnable::DrawLinesFan(bool connectFirstLast, uint lastPointSize) const
{
    Utils::DrawLineFan(GetRenderer(), m_points, m_pointSize);
    if (connectFirstLast) {
        const auto f = m_points.front().ToPoint();
        const auto l = m_points.back().ToPoint();
        Utils::DrawLine(GetRenderer(), f.x, f.y, l.x, l.y, m_pointSize);
    }
    if (lastPointSize > m_pointSize) {
        const auto l = m_points.back().ToPoint();
        Utils::DrawPoint(GetRenderer(), l.x - lastPointSize / 2, l.y - lastPointSize / 2, lastPointSize);
    }
}
