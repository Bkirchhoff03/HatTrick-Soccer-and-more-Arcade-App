/*
 * Player.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_PLAYER_H_
#define GAMES_SOCCER_PLAYER_H_


#include "SoccerPlayer.h"
#include <stdint.h>
class Screen;
class SpriteSheet;
class SoccerBall;
class Player: public SoccerPlayer {
public:
	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement);
	virtual void update(uint32_t dt) override;
	virtual void setMovementDirection(PlayerMovement movement) override;
	inline bool isSliding() const {
		return mState == PLAYER_SLIDING;
	}
	inline bool isWithBall() const {
		return mState == PLAYER_RUNNING_WITH_BALL || mState == PLAYER_JOGGING_WITH_BALL
				|| mState == PLAYER_SPRINTING_WITH_BALL;
	}
	inline bool isWithoutBall() const {
		return !isWithBall();
	}
	void resetToFirstAnimation();
	void ballTakenByDefender();
	void resetScore();
	void kick(SoccerBall &soccerBall);
	inline uint32_t score() const {
		return mScore;
	}

	inline void giveBall() {
		mState = PLAYER_STOPPED_WITH_BALL;
	}
	void addToScore(uint32_t value);
	void setPlayerState(PlayerState state);
	inline PlayerState getPlayerState() const {
		return mState;
	}
	void setBallPossession(bool hasBall);
	inline bool hasBall() const {
		return isWithBall();
	}
private:
	uint32_t mScore;
	PlayerState mState;
	bool mIsLosingBall;
};





#endif /* GAMES_SOCCER_PLAYER_H_ */
