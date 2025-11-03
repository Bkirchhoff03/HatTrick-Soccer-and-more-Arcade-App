/*
 * GameController.cpp
 *
 *  Created on: Jul 11, 2023
 *      Author: nebk2
 */

#include "GameController.h"
#include <SDL2/SDL.h>

GameController::GameController():mMouseMovedAction(nullptr) {

}
InputAction GameController::getActionForKey(InputKey key) {
	for (const ButtonAction &buttonAction : mButtonActions) {
		if (key == buttonAction.key) {
			return buttonAction.action;
		}
	}
	return [](uint32_t, InputState) {
	};
}
InputAction GameController::getActionForMultipleKeys(Uint8 *keys) {
	for (const ButtonAction &buttonAction : mButtonActions) {
		if (keys[buttonAction.keys.first] == 1 && keys[buttonAction.keys.second] == 1) {
			return buttonAction.action;
		}
	}
	return NULL;
}

void GameController::addInputActionForKey(const ButtonAction &buttonAction) {
	mButtonActions.push_back(buttonAction);
}
void GameController::clearAll() {
	mButtonActions.clear();
}

bool GameController::isPressed(InputState state) {
	return state == SDL_PRESSED;
}
bool GameController::isReleased(InputState state) {
	return state == SDL_RELEASED;
}
InputKey GameController::actionKey() {
	return static_cast<InputKey>(SDLK_RETURN);
}
InputKey GameController::cancelKey() {
	return static_cast<InputKey>(SDLK_ESCAPE);
}
InputKey GameController::leftKey() {
	return static_cast<InputKey>(SDLK_a);
}
InputKey GameController::rightKey() {
	return static_cast<InputKey>(SDLK_d);
}
InputKey GameController::upKey() {
	return static_cast<InputKey>(SDLK_w);
}
InputKey GameController::downKey() {
	return static_cast<InputKey>(SDLK_s);
}
InputKey GameController::spaceKey() {
	return static_cast<InputKey>(SDLK_SPACE);
}
InputKey GameController::shiftKey() {
	return static_cast<InputKey>(SDLK_LSHIFT);
}
InputKeys GameController::leftUpKey() {
	return static_cast<InputKeys>(std::pair<uint8_t, uint8_t>(SDL_SCANCODE_A, SDL_SCANCODE_W));
}
InputKeys GameController::rightUpKey() {
	return static_cast<InputKeys>(std::pair<uint8_t, uint8_t>(SDL_SCANCODE_D, SDL_SCANCODE_W));
}
InputKeys GameController::leftDownKey() {
	return static_cast<InputKeys>(std::pair<uint8_t, uint8_t>(SDL_SCANCODE_A, SDL_SCANCODE_S));
}
InputKeys GameController::rightDownKey() {
	return static_cast<InputKeys>(std::pair<uint8_t, uint8_t>(SDL_SCANCODE_D, SDL_SCANCODE_S));
}
MouseInputAction GameController::getMouseButtonActionForMouseButton(MouseButton button) {
	for (MouseButtonAction buttonAction : mMouseButtonActions) {
		if(button == buttonAction.mouseButton){
			return buttonAction.mouseInputAction;
		}
	}
	return [](InputState state, const MousePosition& pos){};
}
void GameController::addMouseButtonAction(
		const MouseButtonAction &mouseButtonAction) {
	mMouseButtonActions.push_back(mouseButtonAction);
}

MouseButton GameController::leftMouseButton() {
	return static_cast<MouseButton>(SDL_BUTTON_LEFT);
}
MouseButton GameController::rightMouseButton() {
	return static_cast<MouseButton>(SDL_BUTTON_RIGHT);
}
