/*
 * SoccerBall.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: nebk2
 */

#include "SoccerBall.h"
#include "Ball.h"
#include "Utils.h"
#include "SoccerGameUtils.h"
#include "Circle.h"
#include "Screen.h"
#include "SoccerPlayer.h"
#include "Vec2D.h"
#include "BoundaryEdge.h"
#include "TeamAgainst.h"
#include <math.h>

const float SoccerBall::RADIUS = 3.5f;

SoccerBall::SoccerBall() :
	SoccerBall(Vec2D::zero, SoccerBall::RADIUS) {
}
SoccerBall::SoccerBall(SoccerBall& soccerBall) {
	mBBox = soccerBall.mBBox;
	mVelocity = soccerBall.mVelocity;
	mState = soccerBall.mState;
	mSpawnPosition = soccerBall.mSpawnPosition;
}
SoccerBall::SoccerBall(const Vec2D& pos, float radius) :
	mBBox(pos - Vec2D(radius, radius), radius * 2.0f, radius * 2.0f), mVelocity(Vec2D::zero) {
	mState = SOCCER_BALL_STATE_IN_PLAY_AT_REST;
	mSpawnPosition = pos;
}

void SoccerBall::update(uint32_t dt) {
	if (mState == SOCCER_BALL_STATE_IN_PLAY_MOVING) {
		//mVelocity -= mVelocity.getUnitVec();  // Reduces by 1 unit/frame
		//This should be time-based:
		float friction = 0.95f;  // Adjust to taste
		mVelocity *= pow(friction, millisecondsToSeconds(dt) * 60.0f);
		if (mVelocity.mag() < 0.5f) {
			mVelocity = Vec2D::zero;
			mState = SOCCER_BALL_STATE_IN_PLAY_AT_REST;
		}

		//mVelocity -= mVelocity.getUnitVec();
	}
	else {
		mVelocity = Vec2D::zero;
		mState = SOCCER_BALL_STATE_IN_PLAY_AT_REST;
	}
	mBBox.moveBy(mVelocity * millisecondsToSeconds(dt));

}
void SoccerBall::draw(Screen& screen) {
	screen.draw(Circle(mBBox.getCenterPoint(), (mBBox.getWidth() / 2.0f)), Color::black(), true, Color::black());
}
void SoccerBall::bounce(const BoundaryEdge& edge) {
	Vec2D pointOnEdge;

	makeFlushWithEdge(edge, pointOnEdge, false);

	mVelocity = mVelocity.reflect(edge.normal);
}
void SoccerBall::resetToFirstPosition() {
	mBBox.moveTo(mSpawnPosition);
}

void SoccerBall::bounceOffOfSoccerPlayer(SoccerPlayer soccerPlayer) {
	mState = SOCCER_BALL_STATE_IN_PLAY_MOVING;

	float playerSpeed = static_cast<float>(soccerPlayer.getMovementSpeed());
	Vec2D playerVelocity = getMovementVector(soccerPlayer.getMovementDirection());
	// Calculate direction from player to ball
	Vec2D playerPos = soccerPlayer.getBoundingBox().getCenterPoint();
	Vec2D ballPos = mBBox.getCenterPoint();
	Vec2D playerToBall = (ballPos - playerPos).getUnitVec();

	// If player is moving, kick in that direction
	// Otherwise, kick away from player
	Vec2D kickDirection;
	if (playerVelocity.mag() > 0.1f) {
		kickDirection = playerVelocity.getUnitVec();
	} else {
		kickDirection = playerToBall;
	}

	// Add some of the ball's current momentum (makes it feel more realistic)
	Vec2D currentMomentum = mVelocity * 0.3f;  // Keep 30% of current velocity

	// New velocity = player kick + ball momentum
	mVelocity = (kickDirection * playerSpeed * 2.0f) + currentMomentum;

	// Calculate the new velocity of the ball after bouncing off the player
	//Vec2D newVelocity = playerVelocity * playerSpeed * 2;

	// Update the ball's velocity
	//mVelocity = newVelocity;
}

void SoccerBall::kick(const Vec2D &direction, float power) {
	mState = SOCCER_BALL_STATE_IN_PLAY_MOVING;
	mVelocity = direction.getUnitVec() * power;

}
