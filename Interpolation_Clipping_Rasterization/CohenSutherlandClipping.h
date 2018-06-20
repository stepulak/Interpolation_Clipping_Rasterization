#pragma once

#include "RasterGridRunnable.h"

class CohenSutherlandClipping : public RasterGridRunnable {
private:

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

	static constexpr int WINDOW_BORDER_THICKNESS = 2;
	static constexpr int WINDOW_WIDTH = 300;
	static constexpr int WINDOW_HEIGHT = 200;

	SDL_Rect m_clippingWindow;

	CollisionCode GetPositionCode(const Point& p) const;
	
	// Clip line from q point
	Point ClipSecondEndPoint(Point p, Point q, unsigned int& steps) const;
	Line ClipLine(const Point& p, const Point& q, bool stepMode) const;

	void DrawAppInfo() const;
	void DrawClippingWindow() const;

public:

	CohenSutherlandClipping(SDL_Window* w, SDL_Renderer* r);

    void DrawContent() const override;
};
