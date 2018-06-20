#pragma once

#include "RasterGridRunnable.h"

class CyrusBeckClipping : public RasterGridRunnable {
private:

	static constexpr int POLYGON_RADIUS = 100;
	static constexpr int POLYGON_MIN_VERTICES = 5;
	static constexpr int POLYGON_MAX_VERTICES = 10;
	static constexpr int POLYGON_THICKNESS = 3;

	std::vector<Point> m_clippingPolygon;
	std::vector<Line> m_clippingLines;

	void CreateClippingPolygon();

	// Clipping lines are extended polygon borders
	// Make sure this function is called after CreateClippingPolygon()
	void CreateClippingLines();

	Line ClipLine(const Point& p, const Point& q, bool stepMode) const;

	void DrawClippingPolygon() const;
	void DrawClippingLines() const;
	void DrawAppInfo() const;

public:

	CyrusBeckClipping(SDL_Window* w, SDL_Renderer* r);

	virtual void DrawContent() const;
};
