#include "LineSeedFill.h"

LineSeedFill::LineSeedFill(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : FramebufferRunnable(font, w, r, 10)
{
    Clear();
}

void LineSeedFill::UpdateContent()
{
    if (!m_initSeedSet) {
        return;
    }
    if (IsStepMode()) {
        m_stepTimer += GetDeltaTime();
        if (m_stepTimer > NEXT_STEP_TIME) {
            m_stepTimer = 0.f;
            PerformOneFill();
        }
        return;
    }
    while (PerformOneFill()) {
        // pass
    }
}

void LineSeedFill::DrawContent() const
{
    if (PolygonLinesFilled()) {
        DrawFramebuffer();
        if (!m_initSeedSet) {
            const auto p = GetMousePosition();
            if (SDL_PointInRect(&p, &GetDrawArea()) == SDL_TRUE) {
                DrawPoint(p.x, p.y);
            }
        }
    } else {
        DrawLinesFan(true);
    }

    DrawFramebufferBorder();
    DrawAppInfo();
}

bool LineSeedFill::HandleKeyPress(const SDL_Keycode& kc)
{
    const bool res = FramebufferRunnable::HandleKeyPress(kc);

    if (kc == SDLK_w && NumberOfFilledPoints() >= 2) {
        AddPoint(GetPoint(0));
        FillPolygonLinesIntoFramebuffer();
        return true;
    }
    if (kc == SDLK_c) {
        Clear();
    }

    return res;
}

bool LineSeedFill::HandleMouseClick(uint8_t button, int x, int y)
{
    if (!PolygonLinesFilled() && FramebufferRunnable::HandleMouseClick(button, x, y)) {
        return true;
    }
    if (!m_initSeedSet) {
        const SDL_Point p = { x, y };
        const auto& area = GetDrawArea();
        if (SDL_PointInRect(&p, &area) == SDL_TRUE) {
            x = (x - area.x) / GetPointSize();
            y = (y - area.y) / GetPointSize();
            m_seedQueue.emplace(x, y, -1, false, false);
            m_seedQueue.emplace(x + 1, y, 1, true, true);
            m_initSeedSet = true;
            return true;
        }
    }
    return false;
}

void LineSeedFill::TryToExpandFilling(bool& expanded, int x, int y, bool parentTop)
{
    if (!expanded && CanContinueFilling(x, y)) {
        expanded = true;
        m_seedQueue.emplace(x, y, -1, parentTop, !parentTop);
        m_seedQueue.emplace(x + 1, y, 1, parentTop, !parentTop);
    } else if (expanded && !CanContinueFilling(x, y)) {
        expanded = false;
    }
}

bool LineSeedFill::PerformOneFill()
{
    if (m_seedQueue.empty()) {
        return false;
    }
    auto& f = m_seedQueue.front();

    if (CanContinueFilling(f.x, f.y)) {
        TryToExpandFilling(f.topExpanded, f.x, f.y - 1, false);
        TryToExpandFilling(f.bottomExpanded, f.x, f.y + 1, true);
        SetColor(f.x, f.y, FILL_COLOR);
        f.x += f.xDirection;
    } else {
        m_seedQueue.pop();
    }
    return true;
}

void LineSeedFill::Clear()
{
    m_seedQueue = {};
    m_initSeedSet = false;
    m_stepTimer = 0.f;
}

void LineSeedFill::DrawAppInfo() const
{
    std::stringstream ss;
    ss << GetAppInfo();

    if (PolygonLinesFilled()) {
        if (!m_initSeedSet) {
            ss << "SELECT INIT SEED POSITION\n";
        }
    } else {
        ss << "[W] SAVE POLY. VERTICES\n";
    }

    GetFont().DrawLine(ss.str(), 18, 0, 0);
}
