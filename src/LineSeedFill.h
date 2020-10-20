#pragma once

#include "FramebufferRunnable.h"
#include <queue>

class LineSeedFill : public FramebufferRunnable {
public:
    LineSeedFill(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    void UpdateContent() override;
    void DrawContent() const override;

private:
    static constexpr float NEXT_STEP_TIME = 0.01f;

    struct FillInfo {
        int x;
        int y;
        int xDirection;
        bool topExpanded;
        bool bottomExpanded;

        FillInfo(int x_, int y_, int xDirection_, bool topExpanded_, bool bottomExpanded_)
            : x(x_)
            , y(y_)
            , xDirection(xDirection_)
            , topExpanded(topExpanded_)
            , bottomExpanded(bottomExpanded_)
        {
        }
    };

    std::queue<FillInfo> m_seedQueue;
    bool m_initSeedSet;
    float m_stepTimer;

    bool PointInGrid(int x, int y) const
    {
        int fw = static_cast<int>(FramebufferWidth());
        int fh = static_cast<int>(FramebufferHeight());
        return x >= 0 && y >= 0 && x < fw && y < fh;
    }

    bool CanContinueFilling(int x, int y) const
    {
        return PointInGrid(x, y) && GetColor(x, y) != FILL_COLOR;
    }

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    bool HandleMouseClick(uint8_t button, int x, int y) override;

    // Try to expand filling in given position (if possible), or check, if another may happen soon
    void TryToExpandFilling(bool& expanded, int x, int y, bool parentTop);

    // Return false if there is no more cell to fill, otherwise true
    bool PerformOneFill();

    void Clear();
    void DrawAppInfo() const;
};
