/*
 * SoccerBall.h
 *
 *  Created on: Nov 8, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_SOCCERBALL_H_
#define GAMES_SOCCER_SOCCERBALL_H_
#include "Ball.h"
//class TeamAgainst;
class SoccerPlayer;
class Screen;
struct BoundaryEdge;
enum SoccerBallState {
	SOCCER_BALL_STATE_IN_PLAY_MOVING,
	SOCCER_BALL_STATE_OUT_OF_PLAY,
	SOCCER_BALL_STATE_IN_PLAY_AT_REST
};
class SoccerBall: Ball {
public:
	SoccerBall();
	SoccerBall(SoccerBall &soccerBall);
	SoccerBall(const Vec2D &pos, float radius);

	void update(uint32_t dt);
	void draw(Screen &screen);
	inline AARectangle getBoundingBox() const {
		return mBBox;
	}
	void resetToFirstPosition();
	void bounce(const BoundaryEdge &edge);
	void bounceOffOfSoccerPlayer(SoccerPlayer soccerPlayer);
	void kick(const Vec2D &direction, float power);
private:
	AARectangle mBBox;
	Vec2D mVelocity;
	SoccerBallState	mState;
	static const float RADIUS;
	static const float FRICTION;
	Vec2D mSpawnPosition;
};



#endif /* GAMES_SOCCER_SOCCERBALL_H_ */
