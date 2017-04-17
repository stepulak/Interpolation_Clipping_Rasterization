#include "CyrusBeckClipping.h"

CyrusBeckClipping::CyrusBeckClipping(SDL_Window* w, SDL_Renderer* r)
	: RasterGridRunnable(w, r, 2, 2)
{
	CreateClippingPolygon();
	CreateClippingLines();
}

void CyrusBeckClipping::CreateClippingPolygon()
{
	int vertices = Utils::Random(POLYGON_MIN_VERTICES, POLYGON_MAX_VERTICES);
	float anglePart = 360.f / static_cast<float>(vertices);
	float angleMax = anglePart * 1.5f;
	float angleMin = anglePart * 0.5f;
	float angleCurrent = 0;

	m_clippingPolygon.reserve(vertices + 1);

	for (int i = 0; i < vertices; i++) {
		if (360.f - angleCurrent < angleMax) {
			angleCurrent = 360.f;
		}
		else {
			angleCurrent += 1.f * Utils::Random(static_cast<int>(angleMin), static_cast<int>(angleMax));
		}
		auto rad = Utils::ToRadians(angleCurrent);
		auto x = GetWindowWidth() / 2.f + POLYGON_RADIUS * cos(rad);
		auto y = GetWindowHeight() / 2.f + POLYGON_RADIUS * sin(rad);

		m_clippingPolygon.push_back(Point(x, y));
	}
	m_clippingPolygon.push_back(m_clippingPolygon.front());
}

void CyrusBeckClipping::CreateClippingLines()
{
	m_clippingLines.reserve(m_clippingPolygon.size() - 1);

	for (unsigned int i = 0; i < m_clippingPolygon.size() - 1; i++) {
		const auto& p = m_clippingPolygon[i];
		const auto& q = m_clippingPolygon[i + 1];
		auto&& mid = p.CountPointMiddle(q);
		auto dirVec = Vec(p, q);

		m_clippingLines.push_back({ mid + (dirVec * 10.f), mid + (dirVec * -10.f) });
	}
}

CyrusBeckClipping::Line CyrusBeckClipping::ClipLine(const Point & p, const Point & q, bool stepMode) const
{
	float tmin = 0.f;
	float tmax = 1.f;
	const auto pqVec = Vec(p, q);
	auto steps = stepMode ? GetCurrentStep() : INT_MAX;

	for (unsigned int i = 0; i < m_clippingPolygon.size() - 1 && tmin < tmax && steps > 0; i++) {
		const auto clipLineNormal = Vec(m_clippingPolygon[i + 1], m_clippingPolygon[i]).CountNormal();
		const auto normalsResult = clipLineNormal.Dot(pqVec);

		if (normalsResult != 0) {
			auto ape = Vec(m_clippingPolygon[i], p);
			auto t0 = -1.f * clipLineNormal.Dot(ape) / normalsResult;

			if (normalsResult < 0) {
				tmin = std::max(t0, tmin);
			}
			else { // > 0
				tmax = std::min(t0, tmax);
			}
			steps--;
		}
	}
	if (tmin < tmax) {
		auto lineParam = [&](float t) {
			return p + (pqVec * t);
		};

		return { lineParam(tmin), lineParam(tmax) };
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
	DrawText(GetAppInfo().str(), 18, 0, 0);
}

void CyrusBeckClipping::DrawContent() const
{
	DrawAppInfo();
	DrawClippingLines();
	DrawClippingPolygon();

	if (AllPointsFilled()) {
		auto&& line = ClipLine(GetPoint(0), GetPoint(1), IsStepMode());
		auto&& p1 = line.first.ToPoint();
		auto&& p2 = line.second.ToPoint();

		Utils::PushColor(GetRenderer());
		SDL_SetRenderDrawColor(GetRenderer(), 255, 0, 0, 255);

		Utils::DrawLine(GetRenderer(), p1.x, p1.y, p2.x, p2.y, GetPointSize());

		auto pts = GetPointSize() * 4;
		Utils::DrawPoint(GetRenderer(), p1.x - pts / 2, p1.y - pts / 2, pts);
		Utils::DrawPoint(GetRenderer(), p2.x - pts / 2, p2.y - pts / 2, pts);

		Utils::PopColor(GetRenderer());
	}
	else {
		DrawLinesFan(false, GetPointSize() * 2);
	}
}
