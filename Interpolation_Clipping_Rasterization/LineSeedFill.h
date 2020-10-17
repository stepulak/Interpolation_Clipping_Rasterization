#pragma once

#include "FramebufferRunnable.h"
#include <queue>

class LineSeedFill : public FramebufferRunnable {
private:
    struct FillInfo {
        int x;
        int y;
        int xDirection;
        bool topExpanded;
        bool bottomExpanded;

        FillInfo(int x, int y, int xDirection, bool topExpanded, bool bottomExpanded)
        {
            this->x = x;
            this->y = y;
            this->xDirection = xDirection;
            this->topExpanded = topExpanded;
            this->bottomExpanded = bottomExpanded;
        }
    };

    static constexpr float NEXT_STEP_TIME = 0.01f;

    std::queue<FillInfo> m_seedQueue;
    bool m_initSeedSet;
    float m_stepTimer;

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    bool HandleMouseClick(uint8_t button, int x, int y) override;

    bool PointInGrid(int x, int y) const
    {
        return x >= 0 && y >= 0 && x < static_cast<int>(FramebufferWidth()) && y < static_cast<int>(FramebufferHeight());
    }

    bool CanContinueFill(int x, int y) const { return PointInGrid(x, y) && GetColor(x, y) != FILL_COLOR; }

    // Try to expand filling in given position (if possible), or check, if another may happen soon
    void TryToExpandFilling(bool& expanded, int x, int y, bool parentTop);

    // return false if there is no more cell to fill, otherwise true
    bool PerformOneFill();

    void Clear();
    void DrawAppInfo() const;

public:
    LineSeedFill(SDL_Window* w, SDL_Renderer* r);

    void UpdateContent() override;
    void DrawContent() const override;
};
