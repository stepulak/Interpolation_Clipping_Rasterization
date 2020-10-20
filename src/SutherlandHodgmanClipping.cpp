#include "SutherlandHodgmanClipping.h"

SutherlandHodgmanClipping::SutherlandHodgmanClipping(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : RasterGridRunnable(font, w, r, 2)
{
    Clear();
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
            } else if (m_pointClipped) {
                ClipNextPoint();
            } else {
                m_drawClippingLine = false;
            }
        }
        return;
    }
    while (m_intersectionFound) {
        FindNextIntersection();
    }
    while (m_pointClipped) {
        ClipNextPoint();
    }
    m_drawClippingLine = false;
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

bool SutherlandHodgmanClipping::HandleMouseClick(uint8_t button, int x, int y)
{
    if (!m_polygonInserted) {
        return RasterGridRunnable::HandleMouseClick(button, x, y);
    }
    if (!m_clippingLineInserted) {
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
    if (!m_clippingLineInserted) {
        m_clippingLineX = x;
        return true;
    }
    return false;
}

bool SutherlandHodgmanClipping::HandleKeyPress(const SDL_Keycode& kc)
{
    const bool res = RasterGridRunnable::HandleKeyPress(kc);

    if (kc == SDLK_c) { // clear
        Clear();
    } else if (kc == SDLK_w) { // write down polygon
        if (!m_polygonInserted && NumberOfFilledPoints() > 1) {
            m_polygonInserted = true;
            AddPoint(GetPoint(0));
        }
    } else if (kc == SDLK_s) {
        m_nextStep = true;
    } else {
        return res;
    }

    return true;
}

void SutherlandHodgmanClipping::FindNextIntersection()
{
    for (size_t i = m_lastPoint; i < NumberOfFilledPoints(); i++) {
        const auto& p0 = GetPoint(i);
        const auto& p1 = GetPoint(i + 1u);

        if ((p1.x > m_clippingLineX && p0.x < m_clippingLineX) || (p0.x > m_clippingLineX && p1.x < m_clippingLineX)) {
            // find intersection
            const auto pIntersec = Utils::FindLineIntersetion(p0, p1 - p0, Point(1.f * m_clippingLineX, 0.f), Vec(0.f, -1.f));
            InsertPoint(pIntersec, i + 1);
            m_intersectionFound = true;
            m_lastPoint = i + 1u;
            return;
        }
    }
    // no more intersections found
    m_intersectionFound = false;
    m_lastPoint = 0u;
}

void SutherlandHodgmanClipping::ClipNextPoint()
{
    for (size_t i = m_lastPoint; i < NumberOfFilledPoints(); i++) {
        if (GetPoint(i).x > m_clippingLineX) {
            // clip the point
            RemovePoint(i);
            m_pointClipped = true;
            m_lastPoint = i;
            return;
        }
    }
    m_pointClipped = false;
    m_lastPoint = 0u;
}

void SutherlandHodgmanClipping::DrawAppInfo() const
{
    std::stringstream ss;
    ss << GetAppInfo();
    ss << "[W] SAVE POLY. VERTICES\n";
    GetFont().DrawText(ss.str(), 0, 0);
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