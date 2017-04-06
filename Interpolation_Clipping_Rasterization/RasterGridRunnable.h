#ifndef RASTER_GRID_RUNNABLE_H
#define RASTER_GRID_RUNNABLE_H

#include "Runnable.h"

// This class provides basic raster point operations, input processing, variable size etc...
class RasterGridRunnable : public Runnable {
protected:

	typedef Utils::Vec2f Point;
	typedef Point Vec;
	typedef std::pair<Point, Point> Line;

private:

	std::vector<Point> m_points;
	const unsigned int m_pointSize;
	const unsigned int m_maxPoints;

	// Step mode
	bool m_stepMode;
	unsigned int m_currentStep;

protected:

	static const int POINT_INVALID = -1;

	inline unsigned int GetPointSize() const { return m_pointSize; }

	inline bool IsStepMode() const { return m_stepMode; }
	inline unsigned int GetCurrentStep() const { return m_currentStep; }

	inline unsigned int NumberOfFilledPoints() const { return m_points.size() - 1; }
	inline bool AllPointsFilled() const { return NumberOfFilledPoints() == m_maxPoints; }

	inline Point& GetPoint(int index) { return m_points[index]; }
	inline const Point& GetPoint(int index) const { return m_points[index]; }

	inline void RemovePoint(int index) { m_points.erase(m_points.begin() + index); }
	inline void PushBackPoint(const Point& p) { m_points.push_back(p); }
	inline void InsertPointAt(const Point& p, int index) { m_points.insert(m_points.begin() + index, p); }
	inline void ReplacePoint(const Point& p, int index) { m_points[index] = p; }

	virtual bool HandleKeyPress(const SDL_Keycode& kc) override;
	virtual bool HandleMouseClick(Uint8 button, Sint32 x, Sint32 y) override;
	virtual bool HandleMouseMotion(Sint32 x, Sint32 y) override;

	virtual std::stringstream GetAppInfo() const override;
	
	inline Point GetInvalidPoint() const { return Point(1.f * POINT_INVALID, 1.f * POINT_INVALID); }
	inline void ClearPoint(Point& p) { p = GetInvalidPoint(); }

	// Draw point calibrated in the raster grid
	inline void DrawPoint(int x, int y) const {
		Utils::DrawPoint(GetRenderer(), x - (x % m_pointSize), y - (y % m_pointSize), m_pointSize);
	}

	void DrawPointOnMousePosition() const;
	void ClearPoints();
	void DrawGrid() const;

	// Draw connected lines
	// If connectFirstLast is true, then the first and last point will be connected with line aswell
	// If lastPointSize > m_pointSize, then the last active point (with mouse cursor on) will be drawn with given thickness
	virtual void DrawLinesFan(bool connectFirstLast = false, unsigned int lastPointSize = 0) const;

	// Draw all inserted points with given pointSize
	// if the pointSize is lower than m_pointSize, then m_pointSize will be used
	void DrawPoints(unsigned int pointSize = 0) const;

public:

	RasterGridRunnable(SDL_Window* w, SDL_Renderer* r, unsigned int pointSize, unsigned int maxPoints = UINT_MAX);
	virtual ~RasterGridRunnable() {}
};

#endif