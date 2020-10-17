#pragma once

#include "Runnable.h"

// This class provides basic raster point operations, input processing, variable size etc...
class RasterGridRunnable : public Runnable {
public:
    using Point = Utils::Vec2f;
    using Points = std::vector<Point>;
    using Vec = Point;
    using Line = std::pair<Point, Point>;

    static constexpr auto INVALID_POINT = Point(-1.f, -1.f);

    RasterGridRunnable() = delete;

    uint GetPointSize() const
    {
        return m_pointSize;
    }

    Points& GetPoints()
    {
        return m_points;
    }

    const Points& GetPoints() const
    {
        return m_points;
    }

    Point& GetPoint(size_t index)
    {
        return m_points[index];
    }

    const Point& GetPoint(size_t index) const
    {
        return m_points[index];
    }

    size_t NumberOfFilledPoints() const {
        if (m_points.size() == 0) {
            throw std::runtime_error("Point on mouse cursor is not set");
        }
        return m_points.size() - 1;
    }

    void AddPoint(const Point& p)
    {
        m_points.push_back(p);
    }

    void InsertPoint(const Point& p, size_t index)
    {
        m_points.insert(m_points.begin() + index, p);
    }

    void RemovePoint(size_t index)
    {
        m_points.erase(m_points.begin() + index);
    }

    bool IsStepMode() const
    {
        return m_stepMode;
    }

    size_t GetCurrentStep() const
    {
        return m_currentStep;
    }

    void DrawPointMousePosition() const;
    void ClearPoints();
    void DrawGrid() const;

    // Draw point with calibrated position in the raster grid
    void DrawPoint(int x, int y) const;

    // Draw all inserted points with given pointSize
    // if the pointSize > m_pointSize, then m_pointSize will be used
    void DrawPoints(uint pointSize = 0u) const;

    // Draw connected lines
    // If connectFirstLast is true, then the first and last point will be connected with line aswell
    // If lastPointSize > m_pointSize, then the last active point (with mouse cursor on) will be drawn with given thickness
    virtual void DrawLinesFan(bool connectFirstLast = false, uint lastPointSize = 0) const;

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    bool HandleMouseClick(uint8_t button, int x, int y) override;
    bool HandleMouseMotion(int x, int y) override;

    std::string GetAppInfo() const override;

protected:
    RasterGridRunnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r, uint pointSize);

private:
    Points m_points;
    const uint m_pointSize;
    bool m_stepMode = false;
    size_t m_currentStep = 0u;
};
