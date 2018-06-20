#pragma once

#include "FramebufferRunnable.h"

// Triangle fill using Pineda algorithm with min-max mechanism
class PinedaTriangleFill : public FramebufferRunnable {
private:

    static constexpr float NEXT_STEP_TIME = 0.01f;
    static constexpr int DRAW_AREA_W = 400;
    static constexpr int DRAW_AREA_H = 400;

    SDL_Point m_currentPoint;
    SDL_Point m_min;
    SDL_Point m_max;
    int m_xDirection;
    float m_timer;

    bool HandleKeyPress(const SDL_Keycode& kc) override;

    bool CanContinueFilling() const { return m_currentPoint.x < m_max.x || m_currentPoint.y < m_max.y; }

    void Clear();
    void SetupInitialFillValues();
    bool PerformOneFill();
    void DrawCurrentPointInteractive() const;
    void DrawAppInfo() const;
	
public:
	
    PinedaTriangleFill(SDL_Window* w, SDL_Renderer* r);

    void UpdateContent() override;
    void DrawContent() const override;
};
