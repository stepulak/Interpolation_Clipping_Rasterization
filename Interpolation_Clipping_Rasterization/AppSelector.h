#ifndef APP_SELECTOR_H
#define APP_SELECTOR_H

#include "Runnable.h"

class AppSelector final : public Runnable {
private:

	template<class T>
	inline void RunApp() {
		auto app = std::make_unique<T>(GetWindow(), GetRenderer());
		app->Start();
	}

	virtual bool HandleKeyPress(const SDL_Keycode& kc) override;
	void DrawAppInfo() const;

public:

	AppSelector(SDL_Window* w, SDL_Renderer* r);
	
	virtual void DrawContent() const override;
};

#endif