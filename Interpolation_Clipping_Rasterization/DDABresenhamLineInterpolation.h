#pragma once

#include "RasterGridRunnable.h"

class DDABresenhamLineInterpolation : public RasterGridRunnable {
private:

	bool m_ddaEnabled; // false for Bresenham

    bool HandleKeyPress(const SDL_Keycode& kc) override;
	void DrawAppInfo() const;
	void DrawDDA(int x0, int y0, int x1, int y1, bool stepMode) const;
	void DrawBresenham(int x0, int y0, int x1, int y1, bool stepMode) const;

public:

	DDABresenhamLineInterpolation(SDL_Window* w, SDL_Renderer* r);

    void DrawContent() const override;
};
