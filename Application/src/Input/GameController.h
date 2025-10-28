/*
 * GameController.h
 *
 *  Created on: Jul 11, 2023
 *      Author: nebk2
 */

#ifndef INPUT_GAMECONTROLLER_H_
#define INPUT_GAMECONTROLLER_H_

#include "InputAction.h"
#include <vector>

class GameController {
public:
	GameController();
	InputAction getActionForKey(InputKey key);
	InputAction getActionForMultipleKeys(Uint8 *keys);
	void addInputActionForKey(const ButtonAction &buttonAction);
	void clearAll();

	static bool isPressed(InputState state);
	static bool isReleased(InputState state);
	static InputKey actionKey();
	static InputKey cancelKey();
	static InputKey leftKey();
	static InputKey rightKey();
	static InputKey upKey();
	static InputKey downKey();
	static InputKey enterKey();
	static InputKey shiftKey();
	static InputKeys leftUpKey();
	static InputKeys rightUpKey();
	static InputKeys leftDownKey();
	static InputKeys rightDownKey();

	inline const MouseMovedAction getMouseMovedAction() {
		return mMouseMovedAction;
	}
	inline void setMouseMovedAction(const MouseMovedAction &mouseMovedAction) {
		mMouseMovedAction = mouseMovedAction;
	}

	MouseInputAction getMouseButtonActionForMouseButton(MouseButton button);
	void addMouseButtonAction(const MouseButtonAction& mouseButtonAction);

	static MouseButton leftMouseButton();
	static MouseButton rightMouseButton();
private:
	std::vector<ButtonAction> mButtonActions;
	std::vector<MouseButtonAction> mMouseButtonActions;
	MouseMovedAction mMouseMovedAction;
};

#endif /* INPUT_GAMECONTROLLER_H_ */
