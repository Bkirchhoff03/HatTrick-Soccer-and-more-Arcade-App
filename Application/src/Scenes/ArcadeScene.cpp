/*
 * ArcadeScene.cpp
 *
 *  Created on: Jul 10, 2023
 *      Author: nebk2
 */

#include "ArcadeScene.h"
#include "Screen.h"
#include "GameController.h"
#include "App.h"
#include "GameScene.h"
#include "Breakout.h"
#include "Tetris.h"
#include "SoccerGame.h"
#include "NotImplementedScene.h"
#include "PacmanStartScene.h"
#include "SoccerStartScene.h"
#include <iostream>

ArcadeScene::ArcadeScene() :
		ButtonOptionsScene( { "Tetris", "Break Out!", "Asteroids", "!Pac-Man", "Soccer" }, Color::cyan()) {

}
void ArcadeScene::init() {
	std::vector<Button::ButtonAction> actions;
	actions.push_back([this] {
		App::singleton().pushScene(getScene(TETRIS));
	});
	actions.push_back([this] {
		App::singleton().pushScene(getScene(BREAKOUT));
	});
	actions.push_back([this] {
		App::singleton().pushScene(getScene(ASTEROIDS));
	});
	actions.push_back([this] {
		App::singleton().pushScene(getScene(PACMAN));
	});
	actions.push_back([this] {
		App::singleton().pushScene(getScene(SOCCER));
	});
	setButtonActions(actions);
	ButtonOptionsScene::init();
}

void ArcadeScene::update(uint32_t dt) {
}

void ArcadeScene::draw(Screen &theScreen) {
	ButtonOptionsScene::draw(theScreen);
}
const std::string& ArcadeScene::getSceneName() const {
	static std::string sceneName = "Arcade";
	return sceneName;
}
std::unique_ptr<Scene> ArcadeScene::getScene(eGame game) {
	switch (game) {
	case TETRIS: {
		std::unique_ptr<Tetris> tetrisGame = std::make_unique<Tetris>();
		std::unique_ptr<GameScene> tetrisScene = std::make_unique<GameScene>(std::move(tetrisGame));
		return tetrisScene;
	}
		break;
	case BREAKOUT: {

		std::unique_ptr<BreakOut> breakOutGame = std::make_unique<BreakOut>();
		std::unique_ptr<GameScene> breakOutScene = std::make_unique<GameScene>(std::move(breakOutGame));
		return breakOutScene;
	}
		break;
	case ASTEROIDS: {

	}
		break;
	case PACMAN: {
		return std::make_unique<PacmanStartScene>();
	}
		break;
	case SOCCER: {
		return std::make_unique<SoccerStartScene>();
	}
		break;
	default: {

	}
		break;
	}
	std::unique_ptr<Scene> notImplementedScene = std::make_unique<NotImplementedScene>();
	return notImplementedScene;
	/*Temporary
	 {
	 std::unique_ptr<Tetris> tetrisGame = std::make_unique<Tetris>();
	 std::unique_ptr<GameScene> tetrisScene = std::make_unique<GameScene>(
	 std::move(tetrisGame));
	 pushScene(std::move(tetrisScene));
	 }*/
}
