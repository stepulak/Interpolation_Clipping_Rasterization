#include "PinedaTriangleFill.h"

const float PinedaTriangleFill::NEXT_STEP_TIME = 0.01f;

PinedaTriangleFill::PinedaTriangleFill(SDL_Window* w, SDL_Renderer* r)
	: FramebufferRunnable(w, r, { 400, 100, DRAW_AREA_W, DRAW_AREA_H }, 5, 3)
{
	Clear();
}

bool PinedaTriangleFill::HandleKeyPress(const SDL_Keycode& kc)
{
	bool res = FramebufferRunnable::HandleKeyPress(kc);

	if (kc == SDLK_c) {
		Clear();
		return true;
	}

	return res;
}

void PinedaTriangleFill::Clear()
{
	m_currentPoint = m_min = m_max = { POINT_INVALID, POINT_INVALID };
	m_xDirection = 1;
	m_timer = 0.f;
}

void PinedaTriangleFill::SetupInitialFillValues()
{
	m_min = { static_cast<int>(FramebufferWidth()), static_cast<int>(FramebufferHeight()) };
	m_max = { 0, 0 };

	for (unsigned int i = 0; i <= NumberOfFilledPoints(); i++) {
		auto&& p = GetPointAsIndex(i);
		
		if (m_min.x > p.x) {
			m_min.x = p.x;
		}
		if (m_max.x < p.x) {
			m_max.x = p.x;
		}
		if (m_min.y > p.y) {
			m_min.y = p.y;
		}
		if (m_max.y < p.y) {
			m_max.y = p.y;
		}
	}

	m_currentPoint = m_min;
}

bool PinedaTriangleFill::PerformOneFill()
{
	if (!CanContinueFilling()) {
		return false;
	}
	
	auto ptSize = 1.f * GetPointSize();
	auto ptReal = Point(m_currentPoint) * ptSize + Vec(ptSize / 2.f, ptSize / 2.f);

	if (Utils::PointInTriangle(ptReal, GetPointLocal(0), GetPointLocal(1), GetPointLocal(2))) {
		SetColor(m_currentPoint.x, m_currentPoint.y, FILL_COLOR);
	}

	if ((m_xDirection > 0 && m_currentPoint.x >= m_max.x) || (m_xDirection < 0 && m_currentPoint.x <= m_min.x)) {
		m_currentPoint.y++;
		m_xDirection *= -1;		
	}
	else {
		m_currentPoint.x += m_xDirection;
	}

	return true;
}

void PinedaTriangleFill::DrawCurrentPointInteractive() const
{
	auto& da = GetDrawArea();
	auto ptSize = GetPointSize();
	DrawPoint(da.x + m_currentPoint.x * ptSize, da.y + m_currentPoint.y * ptSize);
}

void PinedaTriangleFill::DrawAppInfo() const
{
	DrawText(GetAppInfo().str(), 18, 0, 0);
}

void PinedaTriangleFill::UpdateContent()
{
	if (AllPointsFilled()) {
		if (!PolygonLinesFilled()) {
			FillPolygonLinesIntoFramebuffer();
			SetupInitialFillValues();
		}
		else {
			if (IsStepMode()) {
				m_timer += GetDeltaTime();
				if (m_timer >= NEXT_STEP_TIME) {
					m_timer = 0;
					PerformOneFill();
				}
			}
			else {
				while (PerformOneFill());
			}
		}
	}
}

void PinedaTriangleFill::DrawContent() const
{
	if (AllPointsFilled()) {
		DrawFramebuffer();
		if (IsStepMode() && CanContinueFilling()) {
			DrawCurrentPointInteractive();
		}
	}
	else {
		DrawLinesFan(true);
	}

	DrawFramebufferBorder();
	DrawAppInfo();
}
