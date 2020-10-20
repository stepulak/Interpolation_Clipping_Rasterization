#include "AppSelector.h"
#include "CircleEllipseInterpolation.h"
#include "CohenSutherlandClipping.h"
#include "CyrusBeckClipping.h"
#include "DDABresenhamLineInterpolation.h"
#include "LineSeedFill.h"
#include "PinedaTriangleFill.h"
#include "SutherlandHodgmanClipping.h"

AppSelector::AppSelector(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : Runnable(font, w, r)
{
}

void AppSelector::DrawContent() const
{
    DrawAppInfo();
}

bool AppSelector::HandleKeyPress(const SDL_Keycode& kc)
{
    if (Runnable::HandleKeyPress(kc)) {
        // Pass
    } else if (kc == SDLK_a) {
        RunApp<DDABresenhamLineInterpolation>();
    } else if (kc == SDLK_s) {
        RunApp<CircleEllipseInterpolation>();
    } else if (kc == SDLK_d) {
        RunApp<CohenSutherlandClipping>();
    } else if (kc == SDLK_f) {
        RunApp<CyrusBeckClipping>();
    } else if (kc == SDLK_g) {
        RunApp<SutherlandHodgmanClipping>();
    } else if (kc == SDLK_h) {
        RunApp<LineSeedFill>();
    } else if (kc == SDLK_j) {
        RunApp<PinedaTriangleFill>();
    } else {
        return false;
    }

    return true;
}

void AppSelector::DrawAppInfo() const
{
    std::stringstream ss;

    ss << GetAppInfo();
    ss << "\nSELECT AN OPTION:\n\n";
    ss << "  [A] DDA, BRESENHAM LINE INTERPOLATION\n";
    ss << "  [S] CIRCLE ELLIPSE INTERPOLATION\n";
    ss << "  [D] COHEN SUTHERLAND LINE CLIPPING\n";
    ss << "  [F] CYRUS BECK LINE CLIPPING IN GENERIC WINDOW\n";
    ss << "  [G] SUTHERLAND HODGMAN POLYGON CLIPPING\n";
    ss << "  [H] LINE SEED POLYGON FILLING\n";
    ss << "  [J] PINEDA TRIANGLE FILLING\n";

    GetFont().DrawText(ss.str(), 50, 30);
}
