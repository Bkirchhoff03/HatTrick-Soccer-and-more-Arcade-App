/*
 * Player.cpp
 *
 *  Created on: Sep 25, 2023
 *      Author: nebk2
 */

#include "Player.h"
#include "SoccerBall.h"
#include "Line2D.h"

void Player::init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
		uint32_t movementSpeed, bool updateSpriteOnMovement) {
	SoccerPlayer::init(spriteSheet, animationsPath, intialPos, movementSpeed, false);
	resetToFirstAnimation();
	resetScore();
	mState = PLAYER_STOPPED;
}
void Player::update(uint32_t dt) {
	SoccerPlayer::update(dt);
}
void Player::setMovementDirection(PlayerMovement movement) {
	PlayerMovement currentDirection = getMovementDirection();

	if (movement == PLAYER_MOVEMENT_LEFT && currentDirection != PLAYER_MOVEMENT_LEFT) {
		setAnimation("move_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT && currentDirection != PLAYER_MOVEMENT_RIGHT) {
		setAnimation("move_right", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_DOWN && currentDirection != PLAYER_MOVEMENT_DOWN) {
		setAnimation("move_down", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_UP && currentDirection != PLAYER_MOVEMENT_UP) {
		setAnimation("move_up", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_LEFT_UP && currentDirection != PLAYER_MOVEMENT_LEFT_UP) {
		setAnimation("move_up_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_LEFT_DOWN && currentDirection != PLAYER_MOVEMENT_LEFT_DOWN) {
		setAnimation("move_down_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT_UP && currentDirection != PLAYER_MOVEMENT_RIGHT_UP) {
		setAnimation("move_up_right", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT_DOWN && currentDirection != PLAYER_MOVEMENT_RIGHT_DOWN) {
		setAnimation("move_down_right", true);
		resetDelta();
	}
	// Update state based on whether we're moving and if we have the ball
	// This is a simple version - you can make it more sophisticated
	if (movement == PLAYER_MOVEMENT_NONE) {
		if (isWithBall()) {
			setPlayerState(PLAYER_STOPPED_WITH_BALL);
		} else {
			setPlayerState(PLAYER_STOPPED);
		}
	} else {
		// Default to running when moving (you can add sprint button later)
		if (isWithBall()) {
			setPlayerState(PLAYER_RUNNING_WITH_BALL);
		} else {
			setPlayerState(PLAYER_RUNNING);
		}
	}
	//setMovementSpeed(50);
	SoccerPlayer::setMovementDirection(movement);
}

void Player::resetToFirstAnimation() {
}
void Player::ballTakenByDefender() {
}
void Player::resetScore() {
	mScore = 0;
}
void Player::kick(SoccerBall &soccerBall) {
	// Check if ball is close enough to kick
	float kickDistance = getBoundingBox().getWidth() * 1.5f;  // Can kick within 1.5x player width
	Vec2D playerPos = getBoundingBox().getCenterPoint();
	Vec2D ballPos = soccerBall.getBoundingBox().getCenterPoint();

	float distanceToBall = playerPos.distance(ballPos);

	if (distanceToBall <= kickDistance) {
		// Ball is close enough - kick it!

		// Kick direction based on player's facing direction
		PlayerMovement dir = getMovementDirection();
		Vec2D kickDirection = getMovementVector(dir);

		// If player isn't moving, kick based on player-to-ball direction
		if (dir == PLAYER_MOVEMENT_NONE) {
			kickDirection = (ballPos - playerPos).getUnitVec();
		}

		// Set kick power (you can vary this based on player state later)
		float kickPower = 150.0f;  // Adjust this to tune kick strength

		// Apply the kick to the ball
		soccerBall.kick(kickDirection, kickPower);
	}
	/*Vec2D dribblePos = (getMovementVector(getMovementDirection()) *= 3) += position();
	if (soccerBall.getBoundingBox().getCenterPoint().distance(position()) < 3
			&& soccerBall.getBoundingBox().getCenterPoint().distance(dribblePos) < 1) {
		BoundaryEdge edge;
		PlayerMovement dir = getMovementDirection();
		AARectangle bbox = getBoundingBox();
		if (dir == PLAYER_MOVEMENT_LEFT) {
			edge.edge = Line2D(Vec2D(bbox.getBottomRightPoint().GetY(), bbox.getTopLeftPoint().GetX()),
					Vec2D(bbox.getTopLeftPoint().GetX(), bbox.getTopLeftPoint().GetY()));
	}
	 edge.edge = Line2D(getBoundingBox()., p1)
	 soccerBall.bounce()
	 }*/
}

void Player::addToScore(uint32_t value) {
	mScore += value;
}

void Player::setPlayerState(PlayerState state) {
	mState = state;

	switch (mState) {
	case PLAYER_STOPPED:
		setMovementSpeed(0);
		std::cout << "Player Stopped" << std::endl;
		break;

	case PLAYER_RUNNING:
		setMovementSpeed(PLAYER_MOVEMENT_SPEED);
		std::cout << "Player Running" << std::endl;
		break;

	case PLAYER_JOGGING:
		setMovementSpeed(PLAYER_MOVEMENT_SPEED / 2);  // Slower than running
		std::cout << "Player Jogging" << std::endl;
		break;

	case PLAYER_SPRINTING:
		setMovementSpeed(PLAYER_SPRINT_SPEED);
		std::cout << "Player Sprinting" << std::endl;
		break;

	case PLAYER_TACKLING:
		// Tackling locks movement temporarily
		setMovementSpeed(0);
		std::cout << "Player Tackling" << std::endl;
		// Could trigger tackle animation here
		break;

	case PLAYER_SLIDING:
		// Sliding continues in one direction
		setMovementSpeed(PLAYER_SPRINT_SPEED * 1.2f);  // Faster but uncontrollable
		break;

	case PLAYER_STOPPED_WITH_BALL:
		setMovementSpeed(0);
		std::cout << "Player Stopped With Ball" << std::endl;
		break;

	case PLAYER_RUNNING_WITH_BALL:
		setMovementSpeed(PLAYER_WITH_BALL_MOVEMENT_SPEED);
		std::cout << "Player Running With Ball" << std::endl;
		break;

	case PLAYER_JOGGING_WITH_BALL:
		setMovementSpeed(PLAYER_WITH_BALL_MOVEMENT_SPEED / 2);
		std::cout << "Player Jogging With Ball" << std::endl;
		break;

	case PLAYER_SPRINTING_WITH_BALL:
		setMovementSpeed(PLAYER_WITH_BALL_SPRINT_SPEED);
		std::cout << "Player Sprinting With Ball" << std::endl;
		break;

	default:
		break;
	}
}

void Player::setBallPossession(bool hasBall) {
	if (hasBall && !isWithBall()) {
		std::cout << "Player Got Ball" << std::endl;
		// Just got the ball - switch to with-ball state
		if (mState == PLAYER_SPRINTING) {
			setPlayerState(PLAYER_SPRINTING_WITH_BALL);
		} else if (mState == PLAYER_RUNNING) {
			setPlayerState(PLAYER_RUNNING_WITH_BALL);
		} else {
			setPlayerState(PLAYER_STOPPED_WITH_BALL);
		}
	} else if (!hasBall && isWithBall()) {
		std::cout << "Player Lost Ball" << std::endl;
		// Lost the ball - switch to without-ball state
		if (mState == PLAYER_SPRINTING_WITH_BALL) {
			setPlayerState(PLAYER_SPRINTING);
		} else if (mState == PLAYER_RUNNING_WITH_BALL) {
			setPlayerState(PLAYER_RUNNING);
		} else {
			setPlayerState(PLAYER_STOPPED);
		}
	}
}
