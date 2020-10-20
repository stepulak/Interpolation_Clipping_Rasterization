#include "CyrusBeckClipping.h"

CyrusBeckClipping::CyrusBeckClipping(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : RasterGridRunnable(font, w, r, 2u)
{
    CreateClippingPolygon();
    CreateClippingLines();
}

bool CyrusBeckClipping::HandleMouseClick(uint8_t button, int x, int y)
{
    if (NumberOfFilledPoints() < 2u) {
        return RasterGridRunnable::HandleMouseClick(button, x, y);
    }
    return false;
}

void CyrusBeckClipping::DrawContent() const
{
    DrawAppInfo();
    DrawClippingLines();
    DrawClippingPolygon();

    if (NumberOfFilledPoints() != 2u) {
        DrawLinesFan(false, GetPointSize() * 2);
        return;
    }
    const auto line = ClipLine(GetPoint(0), GetPoint(1), IsStepMode());
    const auto p1 = line.first.ToPoint();
    const auto p2 = line.second.ToPoint();
    const auto pts = GetPointSize() * 4u;

    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);
    Utils::DrawLine(GetRenderer(), p1.x, p1.y, p2.x, p2.y, GetPointSize());
    Utils::DrawPoint(GetRenderer(), p1.x - pts / 2, p1.y - pts / 2, pts);
    Utils::DrawPoint(GetRenderer(), p2.x - pts / 2, p2.y - pts / 2, pts);
    Utils::PopColor(GetRenderer());
}

void CyrusBeckClipping::CreateClippingPolygon()
{
    const size_t vertices = Utils::Random(POLYGON_MIN_VERTICES, POLYGON_MAX_VERTICES);
    const auto anglePart = 360.f / static_cast<float>(vertices);
    const auto angleMax = anglePart * 1.5f;
    const auto angleMin = anglePart * 0.5f;
    float angleCurrent = 0.f;

    m_clippingPolygon.reserve(vertices + 1);

    for (size_t i = 0u; i < vertices; i++) {
        if (360.f - angleCurrent < angleMax) {
            angleCurrent = 360.f;
        } else {
            angleCurrent += Utils::Random(static_cast<int>(angleMin), static_cast<int>(angleMax));
        }
        const auto rad = Utils::ToRadians(angleCurrent);
        const auto x = GetWindowWidth() / 2.f + POLYGON_RADIUS * cos(rad);
        const auto y = GetWindowHeight() / 2.f + POLYGON_RADIUS * sin(rad);

        m_clippingPolygon.emplace_back(x, y);
    }

    m_clippingPolygon.push_back(m_clippingPolygon.front());
}

void CyrusBeckClipping::CreateClippingLines()
{
    m_clippingLines.reserve(m_clippingPolygon.size() - 1);

    for (uint i = 0; i < m_clippingPolygon.size() - 1; i++) {
        const auto& p = m_clippingPolygon[i];
        const auto& q = m_clippingPolygon[i + 1];
        const auto mid = p.CountPointMiddle(q);
        const auto dirVec = Vec(p, q);

        m_clippingLines.emplace_back(mid + (dirVec * 10.f), mid + (dirVec * -10.f));
    }
}

CyrusBeckClipping::Line CyrusBeckClipping::ClipLine(const Point& p, const Point& q, bool stepMode) const
{
    const auto pqVec = Vec(p, q);
    float tmin = 0.f;
    float tmax = 1.f;
    auto steps = stepMode ? GetCurrentStep() : std::numeric_limits<size_t>::max();

    for (uint i = 0; i < m_clippingPolygon.size() - 1 && tmin < tmax && steps > 0; i++) {
        const auto clipLineNormal = Vec(m_clippingPolygon[i + 1], m_clippingPolygon[i]).CountNormal();
        const auto normalsResult = clipLineNormal.Dot(pqVec);

        if (normalsResult != 0) {
            const auto ape = Vec(m_clippingPolygon[i], p);
            const auto t0 = -1.f * clipLineNormal.Dot(ape) / normalsResult;

            if (normalsResult < 0) {
                tmin = std::max(t0, tmin);
            } else { // > 0
                tmax = std::min(t0, tmax);
            }
            steps--;
        }
    }
    if (tmin < tmax) {
        return { p + (pqVec * tmin), p + (pqVec * tmax) };
    }
    return {};
}

void CyrusBeckClipping::DrawClippingPolygon() const
{
    Utils::DrawLineFan(GetRenderer(), m_clippingPolygon, POLYGON_THICKNESS);
}

void CyrusBeckClipping::DrawClippingLines() const
{
    Utils::PushColor(GetRenderer());
    SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 255, 255);

    for (const auto& l : m_clippingLines) {
        auto&& p1 = l.first.ToPoint();
        auto&& p2 = l.second.ToPoint();
        SDL_RenderDrawLine(GetRenderer(), p1.x, p1.y, p2.x, p2.y);
    }

    Utils::PopColor(GetRenderer());
}

void CyrusBeckClipping::DrawAppInfo() const
{
    GetFont().DrawText(GetAppInfo(), 0, 0);
}
