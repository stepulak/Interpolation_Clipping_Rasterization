#pragma once

#include "RasterGridRunnable.h"

class CircleEllipseInterpolation : public RasterGridRunnable {
private:

	bool m_circleEnabled;

    bool HandleKeyPress(const SDL_Keycode& kc) override;

	void DrawAppInfo() const;
	void DrawPointCircle8(int x, int y, int dx, int dy) const;
	void DrawPointEllipse4(float x, float y, float dx, float dy) const;
	void DrawCircle(int cx, int cy, int r, bool stepMode) const;
	void DrawEllipse(float cx, float cy, float a, float b, bool stepMode) const;

public:

	CircleEllipseInterpolation(SDL_Window* w, SDL_Renderer* r);

    void DrawContent() const override;
};
