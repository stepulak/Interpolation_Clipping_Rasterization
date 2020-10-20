#pragma once

#include "RasterGridRunnable.h"

class CohenSutherlandClipping : public RasterGridRunnable {
public:
    CohenSutherlandClipping(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    bool HandleMouseClick(uint8_t button, int x, int y) override;
    void DrawContent() const override;

private:
    static constexpr auto WINDOW_BORDER_THICKNESS = 2;
    static constexpr auto WINDOW_WIDTH = 300;
    static constexpr auto WINDOW_HEIGHT = 200;

    enum CollisionCode {
        TOP_LEFT = 9,
        TOP_MID = 8,
        TOP_RIGHT = 10,
        MID_LEFT = 1,
        MID = 0,
        MID_RIGHT = 2,
        BOT_LEFT = 5,
        BOT_MID = 4,
        BOT_RIGHT = 6
    };

    const SDL_Rect m_clippingWindow;

    CollisionCode GetPositionCode(const Point& p) const;
    Point ClipSecondEndPoint(Point p, Point q, size_t& steps) const;
    Line ClipLine(const Point& p, const Point& q, bool stepMode) const;
    void DrawAppInfo() const;
    void DrawClippingWindow() const;
};
