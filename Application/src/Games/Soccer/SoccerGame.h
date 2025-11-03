/*
 * SoccerGame.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_SOCCERGAME_H_
#define GAMES_SOCCER_SOCCERGAME_H_


#include "Game.h"
#include "TeamAgainst.h"
#include "SoccerGameUtils.h"
#include "Player.h"
#include "SpriteSheet.h"
#include "InputAction.h"
#include "Defender.h"
#include "DefenderAI.h"
#include "AARectangle.h"
#include "SoccerBall.h"

enum SoccerGameState {
	GAME_STARTING = 0, IN_GAME, SCORED_ON, GAME_STOPPED, GAME_OVER
};

class SoccerGame: public Game {
public:
	void init(GameController &controller) override;
	void update(uint32_t dt) override;
	void draw(Screen &screen) override;
	const std::string& getName() const override;
	void callFoul(SoccerPlayer player);
private:
	void resetGameAfterScore();
	void updatePlayerMovement();
	bool isDirectionComponentOf(PlayerMovement component, PlayerMovement diagonal);
	void handleGameControllerState(uint32_t dt, InputState state, PlayerMovement direction);
	void resetGame();
	void drawScore(Screen &screen);
	void setupDefenders();


	SoccerBall mSoccerBall;
	PlayerMovement mPressedDirection;
	bool mIsSprintHeld;
	SpriteSheet mPlayerSpriteSheet;
	Player mPlayer;
	//TeamAgainst mTeamAgainst;
	int mNumGoalsFor;
	int mNumGoalsAgainst;
	std::vector<Defender> mDefenders;
	std::vector<DefenderAI> mDefenderAI;
	uint32_t mTimer;
	std::string mAnnouncement;
	SoccerGameState mGameState;
	uint32_t mLevelStartingTimer;
	AARectangle mStringRect;
};



#endif /* GAMES_SOCCER_SOCCERGAME_H_ */
