/*
 * InputAction.h
 *
 *  Created on: Jul 11, 2023
 *      Author: nebk2
 */

#ifndef INPUT_INPUTACTION_H_
#define INPUT_INPUTACTION_H_

#include <functional>
#include <utility>
#include <stdint.h>
#include "C:\msys64\mingw64\include\SDL2\SDL_keycode.h"

using InputKeys = std::pair<uint8_t,uint8_t>;
using InputKey = uint8_t;
using InputState = uint8_t;
using MouseButton = uint8_t;

using InputAction = std::function<void(uint32_t dt, InputState)>;

struct ButtonAction{
	InputKeys keys = std::pair<uint8_t, uint8_t>(SDLK_ASTERISK, SDLK_BACKSPACE);
	InputKey key = SDLK_BACKSPACE;
	InputAction action;
};

struct MousePosition{
	int32_t xPos, yPos;
};

using MouseMovedAction = std::function<void(const MousePosition& mousePosition)>;
using MouseInputAction = std::function<void(InputState state, const MousePosition& position)>;

struct MouseButtonAction{
	MouseButton mouseButton;
	MouseInputAction mouseInputAction;
};


#endif /* INPUT_INPUTACTION_H_ */
