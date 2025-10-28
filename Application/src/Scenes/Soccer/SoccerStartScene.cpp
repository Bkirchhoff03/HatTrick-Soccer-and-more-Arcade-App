/*
 * SoccerStartScene.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#include "SoccerStartScene.h"
#include "App.h"
#include "NotImplementedScene.h"
#include "GameScene.h"
#include "SoccerGame.h"
#include <vector>
#include <memory>
#include <stdlib.h>

SoccerStartScene::SoccerStartScene() :
		ButtonOptionsScene( { "Play Game", "High Scores" }, Color::green()) {

}
void SoccerStartScene::init() {
	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			App::singleton().popScene();
		}
	};

	mGameController.addInputActionForKey(backAction);

	std::vector<Button::ButtonAction> actions;
	actions.push_back([this]() {
		auto soccerGame = std::make_unique<SoccerGame>();
		App::singleton().pushScene(std::make_unique<GameScene>(std::move(soccerGame)));
	});

	actions.push_back([this]() {
		App::singleton().pushScene(std::make_unique<NotImplementedScene>());
	});
	setButtonActions(actions);
	ButtonOptionsScene::init();
}

void SoccerStartScene::update(uint32_t dt) {
}
void SoccerStartScene::draw(Screen &theScreen) {
	ButtonOptionsScene::draw(theScreen);
}
const std::string& SoccerStartScene::getSceneName() const {
	static std::string name = "Soccer start!";
	return name;
}

