#pragma once

#include "FramebufferRunnable.h"

// Triangle fill using Pineda algorithm with min-max mechanism
class PinedaTriangleFill : public FramebufferRunnable {
public:
    PinedaTriangleFill(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    void UpdateContent() override;
    void DrawContent() const override;

private:
    static constexpr auto NEXT_STEP_TIME = 0.01f;
    static constexpr auto DRAW_AREA_W = 400;
    static constexpr auto DRAW_AREA_H = 400;

    SDL_Point m_currentPoint;
    SDL_Point m_min;
    SDL_Point m_max;
    int m_xDirection;
    float m_timer;

    bool CanContinueFilling() const
    {
        return m_currentPoint.x < m_max.x || m_currentPoint.y < m_max.y;
    }

    bool HandleKeyPress(const SDL_Keycode& kc) override;

    void Clear();
    void SetupInitialFillValues();
    bool PerformOneFill();
    void DrawCurrentPointInteractive() const;
    void DrawAppInfo() const;
};
