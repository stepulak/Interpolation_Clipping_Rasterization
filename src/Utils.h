#pragma once

#include "Vector2.h"

#include "limits"
#include <SDL2/SDL.h>
#include <climits>
#include <vector>

namespace Utils {

// Push color into the (global) stack
void PushColor(SDL_Renderer* r);

// Pop color from the (global) stack
// std::runtime_error may be thrown if there is more pops than pushes
void PopColor(SDL_Renderer* r);

// Initialize SDL2 window and renderer with given window proportions
// May throw a std::runtime_error if some error occurs...
std::pair<SDL_Window*, SDL_Renderer*> InitWindowRenderer(const std::string& title, int winW = 800, int winH = 600);

// Destroy both window and renderer
void DestroyWindowRenderer(SDL_Window* window, SDL_Renderer* renderer);

// Load texture from given file
// If you want to fetch texture's proportions, pass a pointer to specific proportion's argument
// May throw a std::runtime_error if some error occurs...
SDL_Texture* LoadTexture(SDL_Renderer* rend, const std::string& file, int* texW = nullptr, int* texH = nullptr);

// Find intersection of two lines represented by point and direction vector
// Note that both vectors are purposely using float (and not integers) for more precise result
Vec2f FindLineIntersetion(const Vec2f& p1, const Vec2f& v1, const Vec2f& p2, const Vec2f& v2);

// Convert from degrees to radians
static float ToRadians(float deg)
{
    return deg * static_cast<float>(M_PI) / 180.0f;
}

// Convert from radians to degrees
static float ToDegrees(float rad)
{
    return rad * 180 / static_cast<float>(M_PI);
}

// Initialize random seed with current time
void InitRandom();

// Get random number from given range
static int Random(int from, int to)
{
    return from + rand() % (to - from + 1);
}

// Return true if point p is in triangle formed from points t1, t2 and t3
bool PointInTriangle(const Vec2f& p, const Vec2f& t1, const Vec2f& t2, const Vec2f& t3);

// Draw point (dot) with given size
static void DrawPoint(SDL_Renderer* rend, int x, int y, int size = 1)
{
    SDL_Rect r = { x, y, size, size };
    SDL_RenderFillRect(rend, &r);
}

// Count signum(value)
template <typename T>
static int Signum(T value)
{
    return (value < static_cast<T>(0)) ? -1 : 1;
}

// Draw line with drawFunc into generic renderer
// Modified DDA from DDA_Bresenham.cpp, because lovely SDL2 scaling system does not apply on primitive shapes...
template <typename Rend, typename Func>
static void DrawLineGeneric(Rend* buffer, Func drawFunc, int x0, int y0, int x1, int y1,
    int lineThickness = 1, float distanceQ = .5f)
{
    float dx = 1.f * (x1 - x0);
    float dy = 1.f * (y1 - y0);
    const float fdx = fabs(dx);
    const float fdy = fabs(dy);
    const float step = (fdx >= fdy ? fdx : fdy);
    dx /= step;
    dy /= step;
    float x = 1.f * x0;
    float y = 1.f * y0;
    const float ht = lineThickness * distanceQ;
    for (float i = 0.f; i <= step; i += ht, x += dx * ht, y += dy * ht) {
        drawFunc(buffer, static_cast<int>(x - ht), static_cast<int>(y - ht), lineThickness);
    }
}

// Draw line into standard SDL2 renderer with specific line thickness
static void DrawLine(SDL_Renderer* rend, int x0, int y0, int x1, int y1, int lineThickness = 1, float distanceQ = .5f)
{
    DrawLineGeneric(rend, DrawPoint, x0, y0, x1, y1, lineThickness, distanceQ);
}

// Draw line fan (connected points) with specific line thickness
// If you do not want to draw all points, then pass your value into the limit variable
template <typename T>
static void DrawLineFan(
    SDL_Renderer* rend,
    const std::vector<Vector2<T>>& points,
    int lineThickness = 1,
    size_t limit = std::numeric_limits<size_t>::max())
{
    for (size_t i = 1u; i < points.size() && i < limit; i++) {
        const auto p = points[i].ToPoint();
        const auto q = points[i - 1u].ToPoint();
        Utils::DrawLine(rend, p.x, p.y, q.x, q.y, lineThickness);
    }
}

// Draw rectangle with given border thickness
// Set borderInside to false if you want to "expand" the rectangle and draw the border outside it's area
void DrawRectangle(SDL_Renderer* rend, const SDL_Rect& r, int borderThickness = 1, bool borderInside = true);

// Draw borders of given grid with specific cell size
// Note that if proportions of the given area are not divisible by cellSize, then
// the rest of last cell will be skipped...
void DrawGrid(SDL_Renderer* rend, const SDL_Rect& gridArea, int cellSize);

} // namespace Utils
