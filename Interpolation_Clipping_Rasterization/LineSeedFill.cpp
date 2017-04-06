#include "LineSeedFill.h"

const float LineSeedFill::NEXT_STEP_TIME = 0.01f;

LineSeedFill::LineSeedFill(SDL_Window* w, SDL_Renderer* r)
	: FramebufferRunnable(w, r, 10)
{
	Clear();
}

bool LineSeedFill::HandleKeyPress(const SDL_Keycode& kc)
{
	bool res = FramebufferRunnable::HandleKeyPress(kc);

	if (kc == SDLK_w && NumberOfFilledPoints() >= 2) {
		PushBackPoint(GetPoint(0));
		FillPolygonLinesIntoFramebuffer();
		return true;
	}
	else if (kc == SDLK_c) {
		Clear();
	}

	return res;
}

bool LineSeedFill::HandleMouseClick(Uint8 button, Sint32 x, Sint32 y)
{
	if (!PolygonLinesFilled() && FramebufferRunnable::HandleMouseClick(button, x, y)) {
		return true;
	}
	else if (!m_initSeedSet) {
		SDL_Point p = { x, y };
		auto& area = GetDrawArea();
		if (SDL_PointInRect(&p, &area) == SDL_TRUE) {
			x = (x - area.x) / GetPointSize();
			y = (y - area.y) / GetPointSize();
			m_seedQueue.push(FillInfo(x, y, -1, false, false));
			m_seedQueue.push(FillInfo(x + 1, y, 1, true, true));
			m_initSeedSet = true;
			return true;
		}
	}
	return false;
}

void LineSeedFill::TryToExpandFilling(bool& expanded, int x, int y, bool parentTop)
{
	if (!expanded && CanContinueFill(x, y)) {
		expanded = true;
		m_seedQueue.push(FillInfo(x, y, -1, parentTop, !parentTop));
		m_seedQueue.push(FillInfo(x + 1, y, 1, parentTop, !parentTop));
	}
	else if (expanded && !CanContinueFill(x, y)) {
		expanded = false;
	}
}

bool LineSeedFill::PerformOneFill()
{
	if (m_seedQueue.empty()) {
		return false;
	}
	auto& f = m_seedQueue.front();

	if (CanContinueFill(f.x, f.y)) {
		TryToExpandFilling(f.topExpanded, f.x, f.y - 1, false);
		TryToExpandFilling(f.bottomExpanded, f.x, f.y + 1, true);

		SetColor(f.x, f.y, FILL_COLOR);
		f.x += f.xDirection;
	} 
	else {
		m_seedQueue.pop();
	}
	return true;
}

void LineSeedFill::Clear()
{
	m_seedQueue = std::queue<FillInfo>();
	m_initSeedSet = false;
	m_stepTimer = 0.f;
}

void LineSeedFill::DrawAppInfo() const
{
	auto&& ss = GetAppInfo();
	if (PolygonLinesFilled()) {
		if (!m_initSeedSet) {
			ss << "SELECT INIT SEED POSITION\n";
		}
	}
	else {
		ss << "[W] SAVE POLY. VERTICES\n";
	}

	DrawText(ss.str(), 18, 0, 0);
}

void LineSeedFill::UpdateContent()
{
	if (m_initSeedSet) {
		if (IsStepMode()) {
			m_stepTimer += GetDeltaTime();
			if (m_stepTimer > NEXT_STEP_TIME) {
				m_stepTimer = 0.f;
				PerformOneFill();
			}
		}
		else {
			while (PerformOneFill());
		}
	}
}

void LineSeedFill::DrawContent() const
{
	if (PolygonLinesFilled()) {
		DrawFramebuffer();
		if (!m_initSeedSet) {
			auto&& p = GetMousePosition();
			if (SDL_PointInRect(&p, &GetDrawArea()) == SDL_TRUE) {
				DrawPoint(p.x, p.y);
			}
		}
	}
	else {
		DrawLinesFan(true);
	}

	DrawFramebufferBorder();
	DrawAppInfo();
}