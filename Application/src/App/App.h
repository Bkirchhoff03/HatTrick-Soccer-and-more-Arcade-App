/*
 * App.h
 *
 *  Created on: Jul 10, 2023
 *      Author: nebk2
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "../Graphics/Screen.h"
#include <vector>
#include <stdint.h>
#include <memory>
#include "../Scenes/Scene.h"
#include "../Input/InputController.h"
#include "../Graphics/BitmapFont.h"
struct SDL_Window;

class App{
public:
	static App& singleton();
	bool init(uint32_t width, uint32_t height, uint32_t mag);
	void run();


	inline const uint32_t width() const {return mScreen.width();}
	inline const uint32_t height() const {return mScreen.height();}

	void pushScene(std::unique_ptr<Scene> scene);
	void popScene();
	Scene* topScene();

	static const std::string& getBasePath();
	inline const BitmapFont& getFont() const {
		return mFont;
	}
private:
	Screen mScreen;
	SDL_Window * mnoptrWindow;

	std::vector<std::unique_ptr<Scene>> mSceneStack;
	InputController mInputController;

	BitmapFont mFont;

};



#endif /* APP_APP_H_ */
