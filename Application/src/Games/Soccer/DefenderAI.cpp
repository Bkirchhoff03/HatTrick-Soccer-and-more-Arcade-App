/*
 * DefenderAI.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: nebk2
 */
#include "DefenderAI.h"
#include "Screen.h"
#include "TeamAgainst.h"
#include "Player.h"
#include "SoccerBall.h"
#include <cassert>
#include <algorithm>
#include <vector>
#include "Color.h"
#include "Circle.h"
DefenderAI::DefenderAI() {
}
void DefenderAI::init(Defender &defender, uint32_t lookAheadDistance, const Vec2D &scatterTarget,
		const Vec2D &defenderPositionTarget, const Vec2D &defenderAttackingPosition, DefenderName name) {
	mDifficultyMultiplier = 1.0f;
	mDefenderZoneTarget = defenderPositionTarget;
	mDefenderExitZonePosition = defenderAttackingPosition;
	mTarget = scatterTarget;
	mLookAheadDistance = lookAheadDistance;
	mnoptrDefender = &defender;
	mName = name;
	std::random_device r;
	mAIRandomGenerator.seed(r());
	mTimer = 0;
	setState(DEFENDER_AI_STATE_START);
	mLastState = DEFENDER_AI_STATE_START;
}

PlayerMovement DefenderAI::update(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
		const std::vector<Defender> &defenders, SoccerBall &soccerBall) {
	if (mnoptrDefender) {
		mTimer += dt;
		if (mTimer > 1000) {
			mTimer = 0;
		}
		if (mState == DEFENDER_AI_STATE_START) {
			setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
		}
		if (mState == DEFENDER_AI_STATE_DOUBLE_TEAM) {
			if (player.isWithoutBall()) {
				setState(DEFENDER_AI_STATE_GO_TO_ZONE);
			} else {
				Vec2D chaseTarget = getChaseTarget(dt, player, teamAgainst, defenders, soccerBall);
				changeTarget(chaseTarget);
			}
		}
		if (mState == DEFENDER_AI_STATE_GO_TO_ZONE
				&& mnoptrDefender->getBoundingBox().getCenterPoint() == mDefenderZoneTarget) {
			setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
			mnoptrDefender->setDefenderState(PLAYER_STOPPED);
		}

		if (mState == DEFENDER_AI_STATE_EXIT_ZONE && !mnoptrDefender->getBoundingBox().intersects(zone)) {
			setState(DEFENDER_AI_STATE_DOUBLE_TEAM);
			mnoptrDefender->setDefenderState(PLAYER_JOGGING);
		}

		if (mState == DEFENDER_AI_STATE_DEFENDING) {
			setState(DEFENDER_AI_STATE_DEFENDING);
			mnoptrDefender->setDefenderState(PLAYER_SPRINTING);
			Vec2D chaseTarget = getChaseTarget(dt, player, teamAgainst, defenders, soccerBall);
			changeTarget(chaseTarget);
			//changeTarget(player.getBoundingBox().getCenterPoint());
		}

		if (mnoptrDefender->getBoundingBox().containsPoint(soccerBall.getBoundingBox().getCenterPoint())) {
			if (mName == GOALKEEPER) {
				setState(DEFENDER_AI_STATE_DEFENDING);
				changeTarget(soccerBall.getBoundingBox().getCenterPoint());
			}
		}

		PlayerMovement currentDirection = mnoptrDefender->getMovementDirection();

		std::vector<PlayerMovement> tempDirections;
		std::vector<PlayerMovement> possibleDirections;

		possibleDirections = getPossibleMovementsBasedOffTarget(mTarget, mnoptrDefender->position());

		if (currentDirection != PLAYER_MOVEMENT_NONE) {
			possibleDirections.push_back(currentDirection);
		}

		for (const PlayerMovement &pDirection : possibleDirections) {
			tempDirections.push_back(pDirection);
		}

		possibleDirections.clear();

		for (const PlayerMovement &direction : tempDirections) {
			if (!teamAgainst.willCollide(*mnoptrDefender, *this, direction)) {
				possibleDirections.push_back(direction);
			}
		}

		//assert(possibleDirections.size() >= 1 && "Why can't we go anywhere?");
		if (possibleDirections.size() == 0) {
			std::cout << mName << " can't go anywhere!" << std::endl;
			std::cout << mState << " is the state" << std::endl;
			assert(false && "Why can't we go anywhere?");
		}

		std::sort(possibleDirections.begin(), possibleDirections.end(),
				[](const PlayerMovement &direction1, const PlayerMovement &direction2) {
					return direction1 < direction2;
				});
		PlayerMovement directionToGoIn = PLAYER_MOVEMENT_NONE;

		uint32_t lowestDistance = UINT32_MAX;

		for (const PlayerMovement &direction : possibleDirections) {
			Vec2D movementVec = getMovementVector(direction) * mLookAheadDistance;

			AARectangle bbox = mnoptrDefender->getBoundingBox();

			bbox.moveBy(movementVec);

			uint32_t distanceToTarget = bbox.getCenterPoint().distance(mTarget);

			if (distanceToTarget < lowestDistance) {
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}

		assert(directionToGoIn != PLAYER_MOVEMENT_NONE);

		return directionToGoIn;
	}

	return PLAYER_MOVEMENT_NONE;
}

void DefenderAI::setZone(const AARectangle rect) {
	zone = rect;
	mnoptrDefender->setZone(rect);
}
void DefenderAI::draw(Screen &screen) {
	if (mnoptrDefender) {
		Circle targetCircle = Circle(mTarget, 4);

		screen.draw(targetCircle, mnoptrDefender->getSpriteColor(), true, mnoptrDefender->getSpriteColor());

		AARectangle bbox = zone;

		/*bbox.moveBy(
		 getMovementVector(mnoptrDefender->getMovementDirection())
		 * mnoptrDefender->getBoundingBox().getWidth());*/

		Color c = Color(mnoptrDefender->getSpriteColor().getRed(), mnoptrDefender->getSpriteColor().getGreen(),
				mnoptrDefender->getSpriteColor().getBlue(), 100);
		screen.draw(bbox, mnoptrDefender->getSpriteColor(), true, c);
	}
}

void DefenderAI::defenderDelegateDefenderStateChangeTo(PlayerState lastState, PlayerState state) {
	if (mnoptrDefender && mnoptrDefender->isRealeasedFromZone()
			&& !(isInZone() || wantsToLeaveZone() || isDefending())) {
		mnoptrDefender->setMovementDirection(getOppositeDirection(mnoptrDefender->getMovementDirection()));
	}
	if (lastState == PLAYER_STOPPED && state == PLAYER_JOGGING) {
		setState(DEFENDER_AI_STATE_GO_TO_ZONE);
	} else if (lastState == PLAYER_STOPPED && state == PLAYER_SPRINTING) {
		setState(DEFENDER_AI_STATE_DEFENDING);
	} else if (lastState == PLAYER_SPRINTING && state == PLAYER_JOGGING) {
		setState(DEFENDER_AI_STATE_GO_TO_ZONE);
	} else if (lastState == PLAYER_JOGGING && state == PLAYER_STOPPED) {
		setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
	} else if (lastState == PLAYER_SPRINTING && state == PLAYER_STOPPED) {
		setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
	} else if (lastState == PLAYER_JOGGING && state == PLAYER_JOGGING) {
		setState(DEFENDER_AI_STATE_GO_TO_ZONE);
	} else if (lastState == PLAYER_JOGGING && state == PLAYER_SPRINTING) {
		setState(DEFENDER_AI_STATE_DEFENDING);
	} else if (state == PLAYER_STOPPED) {
		setState(DEFENDER_AI_STATE_DEFENDING);
	}
}

void DefenderAI::defenderWasReleasedFromZone() {
	if (mState == DEFENDER_AI_STATE_START) {
		setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
	}
}

void DefenderAI::defenderWasResetToZone() {
	setState(DEFENDER_AI_STATE_IN_ZONE_STOPPED);
}

void DefenderAI::setState(DefenderAIState state) {
	if (mState == DEFENDER_AI_STATE_IN_ZONE_STOPPED || mState == DEFENDER_AI_STATE_DEFENDING) {
		mLastState = mState;
	}
	mState = state;

	switch (state) {
	case DEFENDER_AI_STATE_IN_ZONE_STOPPED:
		changeTarget(mDefenderZoneTarget);
		mnoptrDefender->setDefenderState(PLAYER_STOPPED);
		break;
	case DEFENDER_AI_STATE_GO_TO_ZONE: {
		//Vec2D target = { mDefenderZoneTarget.GetX() + mnoptrDefender->getBoundingBox().getWidth() / 2,
		//		mDefenderZoneTarget.GetY() - mnoptrDefender->getBoundingBox().getHeight() / 2 };
		changeTarget(mDefenderZoneTarget);
	}
		break;
	case DEFENDER_AI_STATE_EXIT_ZONE:
		changeTarget(mDefenderExitZonePosition);
		break;
	case DEFENDER_AI_STATE_DEFENDING:
		changeTarget(mTarget);
		break;
	case DEFENDER_AI_STATE_DOUBLE_TEAM:
		changeTarget(mTarget);
		break;
	default:
		break;
	}

}
void DefenderAI::changeTarget(const Vec2D &target) {
	mTarget = target;
}

Vec2D DefenderAI::clampTargetToBounds(const Vec2D &target, const TeamAgainst &teamAgainst) {
	Vec2D bounds = teamAgainst.getBounds();
	Vec2D clampedTarget = target;

	// Add margin so defenders don't get stuck at exact edges
	float margin = 5.0f;

	clampedTarget.SetX(std::max(margin, std::min(clampedTarget.GetX(), bounds.GetX() - margin)));
	clampedTarget.SetY(std::max(margin, std::min(clampedTarget.GetY(), bounds.GetY() - margin)));

	return clampedTarget;
}

Vec2D DefenderAI::getChaseTarget(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
		const std::vector<Defender> &defenders, SoccerBall &soccerBall) {
	//Vec2D prediction = playerPos + (getMovementVector(player.getMovementDirection()));
	Vec2D target;
	Vec2D playerPos = player.getBoundingBox().getCenterPoint();
	Vec2D ballPos = soccerBall.getBoundingBox().getCenterPoint();
	Vec2D myPos = mnoptrDefender->getBoundingBox().getCenterPoint();
	switch (mName) {
	case GOALKEEPER: {
		// Goalkeeper defends the goal
		Vec2D goalPos = mDefenderZoneTarget;

		// If ball is close to goal, intercept it
		float ballDistanceToGoal = goalPos.distance(ballPos);
		if (ballDistanceToGoal < 50.0f) {
			// Ball is dangerous - go for it
			target = ballPos;
		} else {
			// Position between ball and goal center
			Vec2D ballToGoal = (goalPos - ballPos).getUnitVec();
			target = goalPos - (ballToGoal * 15.0f);  // 15 units in front of goal

			// Clamp to stay within the goal area
			float maxDistanceFromGoal = 30.0f;
			if (target.distance(goalPos) > maxDistanceFromGoal) {
				target = goalPos + ((target - goalPos).getUnitVec() * maxDistanceFromGoal);
			}
		}
		//target = soccerBall.getBoundingBox().getCenterPoint();
		//target = player.getBoundingBox().getCenterPoint();
	}
		break;
	case CENTER_BACK: {
		// Center back predicts where player will be
		// Lead the player by 2 tile widths in their movement direction
		Vec2D prediction = playerPos
				+ (getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth() * 2.0f);
		target = prediction;
		//target = player.getBoundingBox().getCenterPoint()
		//		+ 2 * getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth();
	}
		break;
	case LEFT_BACK: {
		// Left back uses pincer movement with CDM, but only when CDM is nearby
		Vec2D cdmPos = defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint();
		float distanceToPlayer = myPos.distance(playerPos);
		float cdmToPlayerDist = cdmPos.distance(playerPos);

		// Only use pincer logic if CDM is close to both LB and player
		float maxPincerDistance = 50.0f;  // CDM must be within this range

		if (distanceToPlayer < maxPincerDistance && cdmToPlayerDist < maxPincerDistance) {
			Vec2D leftCutoff = playerPos + Vec2D(-20.0f, 0.0f);
			target = leftCutoff;
			// CDM is nearby and helping - use pincer movement
			/*Vec2D playerMovement = getMovementVector(player.getMovementDirection())
					* player.getBoundingBox().getWidth();
			Vec2D playerOffsetPoint = playerPos + playerMovement;

			// Mirror player position across the CDM to create pincer
			Vec2D mirroredTarget = (playerOffsetPoint - cdmPos) * 2.0f + cdmPos;

			// IMPORTANT: Clamp target to stay on screen
			Vec2D bounds = teamAgainst.getBounds();
			mirroredTarget.SetX(std::max(0.0f, std::min(mirroredTarget.GetX(), bounds.GetX())));
			mirroredTarget.SetY(std::max(0.0f, std::min(mirroredTarget.GetY(), bounds.GetY())));

			 target = mirroredTarget;*/
		} else if (distanceToPlayer < 80.0f) {
			// CDM is too far away or back in zone - chase player directly
			// Predict player's movement slightly
			Vec2D prediction = playerPos
					+ (getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth() * 1.0f);
			target = prediction;
		} else {
			target = playerPos;
		}
		Vec2D bounds = teamAgainst.getBounds();
		target.SetX(std::max(5.0f, std::min(target.GetX(), bounds.GetX() - 5.0f)));
		target.SetY(std::max(5.0f, std::min(target.GetY(), bounds.GetY() - 5.0f)));
		//MAKES LB GO WAY OFF SCREEN SOMETIMES
		/*
		// Left back uses pincer movement with CDM
		// Tries to trap player between LB and CDM
		Vec2D playerMovement = getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth();
		Vec2D playerOffsetPoint = playerPos + playerMovement;

		// Mirror player position across the CDM to create pincer
		Vec2D cdmPos = defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint();
		target = (playerOffsetPoint - cdmPos) * 2.0f + cdmPos;
		 */
		//ORIGINAL
		//Vec2D pacmanOffsetPoint = player.getBoundingBox().getCenterPoint()
		//		+ (getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth());
		//target = (pacmanOffsetPoint - defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint()) * 2
		//		+ defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint();
	}
		break;
	case RIGHT_BACK: {
		// Right back is aggressive when close, strategic when far
		float distanceToPlayer = myPos.distance(playerPos);
		float aggressiveRange = player.getBoundingBox().getWidth() * 4.0f;

		if (distanceToPlayer > aggressiveRange) {
			// Too far - chase directly
			target = playerPos;
		} else {
			// Close enough - cut off escape route
			// Try to force player toward the sideline or other defenders
			Vec2D toSideline = Vec2D(teamAgainst.getBounds().GetX(), playerPos.GetY());
			Vec2D cutoffPoint = playerPos + ((toSideline - playerPos) * 0.3f);
			target = cutoffPoint;
		}
		/*auto distanceToPacmanBox = mnoptrDefender->getBoundingBox().getCenterPoint().distance(
				player.getBoundingBox().getCenterPoint());
		if (distanceToPacmanBox > player.getBoundingBox().getWidth() * 4) {
			target = player.getBoundingBox().getCenterPoint();
		} else {
			target = mAttackTarget;
		 }*/
	}
		break;
	case CENTER_DEFENSIVE_MIDFIELDER: {
		// CDM is the "sweeper" - always goes for the ball carrier
		// Most direct and aggressive

		// If player has the ball, chase them
		if (player.isWithBall()) {
			target = playerPos;
		} else {
			// Player doesn't have ball - go for the ball itself
			target = ballPos;
		}
		//target = player.getBoundingBox().getCenterPoint();
	}
		break;
	default: {
	}
		break;
	}
	target = clampTargetToBounds(target, teamAgainst);
	return target;
}

