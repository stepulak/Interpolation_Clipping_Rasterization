#pragma once

#include "RasterGridRunnable.h"

class SutherlandHodgmanClipping : public RasterGridRunnable {
public:
    SutherlandHodgmanClipping(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    void UpdateContent() override;
    void DrawContent() const override;

private:
    static constexpr auto CLIPPING_LINE_THICKNESS = 2;

    bool m_polygonInserted;
    bool m_clippingLineInserted;
    uint m_clippingLineX;
    bool m_nextStep;
    size_t m_lastPoint;
    bool m_intersectionFound; // true if intersection was found
    bool m_pointClipped; // true if point was clipped
    bool m_drawClippingLine;

    bool HandleMouseClick(uint8_t button, int x, int y) override;
    bool HandleMouseMotion(int x, int y) override;
    bool HandleKeyPress(const SDL_Keycode& kc) override;

    void FindNextIntersection();
    void ClipNextPoint();
    void DrawAppInfo() const;
    void Clear();
};
