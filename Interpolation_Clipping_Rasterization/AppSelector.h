#pragma once

#include "Runnable.h"

class AppSelector final : public Runnable {
private:

    template<class T>
    inline void RunApp() {
        auto app = std::make_unique<T>(GetWindow(), GetRenderer());
        app->Start();
    }

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    void DrawAppInfo() const;

public:

    AppSelector(SDL_Window* w, SDL_Renderer* r);
	
    void DrawContent() const override;
};
