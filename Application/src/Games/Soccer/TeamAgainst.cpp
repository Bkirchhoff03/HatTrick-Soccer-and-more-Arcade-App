/*
 * TeamAgainst.cpp
 *
 *  Created on: Oct 19, 2023
 *      Author: nebk2
 */

#include "TeamAgainst.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Player.h"
#include "App.h"
#include "Circle.h"
#include "Defender.h"
#include "SoccerBall.h"
#include <cassert>

TeamAgainst& TeamAgainst::singleton() {
	static TeamAgainst theTeamAgainst;
	return theTeamAgainst;
}
bool TeamAgainst::init(const std::string &levelPath, const SpriteSheet *noptrSpriteSheet) {
	mnoptrSpriteSheet = noptrSpriteSheet;
	std::random_device r;
	mGenerator.seed(r());
	mDefendersSpawnPoints.resize(NUM_DEFENDERS);
	bool levelLoaded = loadLevel(levelPath);
	if (levelLoaded) {
		resetLevel();
	}
	bounds.SetX(bounds.GetX() * mTileWidth);
	bounds.SetY(bounds.GetY() * mTileHeight);
	return levelLoaded;
}
void TeamAgainst::update(uint32_t dt, Player &player, std::vector<Defender> &defenders,
		std::vector<DefenderAI> &defenderAIs, SoccerBall &soccerBall) {
	for (const Excluder &wall : mPlayerBoundaries) {
		BoundaryEdge edge;
		if (wall.hasCollided(player.getBoundingBox(), edge)) {
			Vec2D offset = wall.getCollisionOffset(player.getBoundingBox());
			player.moveBy(offset);
			player.stop();
		}

		for (Defender &defender : defenders) {
			if (wall.hasCollided(defender.getBoundingBox(), edge)) {
				Vec2D offset = wall.getCollisionOffset(defender.getBoundingBox());
				defender.moveBy(offset);
				defender.stop();
			}
		}
	}

	for (const Excluder &ballWall : mBallBoundaries) {
		BoundaryEdge edge;
		if (ballWall.hasCollided(soccerBall.getBoundingBox(), edge)) {
			soccerBall.bounce(edge);
		}
	}
	
	for (const Excluder &goalWall : mGoal) {
		BoundaryEdge edge;
		if (goalWall.hasCollided(soccerBall.getBoundingBox(), edge)) {
			player.addToScore(1);
			return;
		}
	}

	/*for (Tile t : mTiles) {
		if (t.isOutOfBoundsTile) {
			AARectangle teleportTileAABB(t.position, static_cast<float>(mTileWidth), static_cast<float>(mTileHeight));

			Tile *teleportToTile = getTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);
			if (teleportToTile->isOutOfBoundsTile) {
				if (teleportTileAABB.intersects(player.getBoundingBox())) {
					player.moveTo(teleportToTile->position + teleportToTile->offset);
				}
				for (Defender &defender : defenders) {
					if (teleportTileAABB.intersects(defender.getBoundingBox())) {
						defender.moveTo(teleportToTile->position + teleportToTile->offset);
					}
				}
			}
		}
	}*/
}
void TeamAgainst::draw(Screen &screen) {
	Sprite bgSprite;
	bgSprite.width = mBGImage.getWidth();
	bgSprite.height = mBGImage.getHeight();

	screen.draw(mBGImage, bgSprite, Vec2D(0, 20));
}

void TeamAgainst::makeDefenderZones(std::vector<Defender>& defenders, std::vector<DefenderAI>& defenderAIs) {
	int width = mBGImage.getWidth();
	int height = mBGImage.getHeight();
	int zoneWidth = width / 4;
	int zoneHeight = height / 3;
	defenders[RIGHT_BACK].setZone(AARectangle(Vec2D(0, 20), zoneWidth, height));
	defenders[LEFT_BACK].setZone(AARectangle(Vec2D(width-zoneWidth, 20), zoneWidth, height));
	defenders[CENTER_BACK].setZone(AARectangle(Vec2D(zoneWidth, zoneHeight+20), zoneWidth*2, zoneHeight));
	defenders[CENTER_DEFENSIVE_MIDFIELDER].setZone(AARectangle(Vec2D(zoneWidth, (zoneHeight*2)+20), zoneWidth*2, zoneHeight));
	defenders[GOALKEEPER].setZone(AARectangle(Vec2D(zoneWidth, 20), zoneWidth*2, zoneHeight));
	defenderAIs[LEFT_BACK].setZone(defenders[LEFT_BACK].zoneBoundingBox());
	defenderAIs[RIGHT_BACK].setZone(defenders[RIGHT_BACK].zoneBoundingBox());
	defenderAIs[CENTER_BACK].setZone(defenders[CENTER_BACK].zoneBoundingBox());
	defenderAIs[CENTER_DEFENSIVE_MIDFIELDER].setZone(defenders[CENTER_DEFENSIVE_MIDFIELDER].zoneBoundingBox());
	defenderAIs[GOALKEEPER].setZone(defenders[GOALKEEPER].zoneBoundingBox());
}
bool TeamAgainst::willCollide(const Defender &defender, const DefenderAI &defenderAI, PlayerMovement direction,
		const AARectangle &aBBox) const {
	AARectangle bbox = defender.getBoundingBox();
	bbox.moveBy(getMovementVector(direction));
	return bbox.intersects(aBBox);
}
bool TeamAgainst::willCollide(const SoccerBall &soccerBall, PlayerMovement direction) const {
	AARectangle bbox = soccerBall.getBoundingBox();

	bbox.moveBy(getMovementVector(direction));

	BoundaryEdge edge;

	for (const Excluder &wall : mBallBoundaries) {
		if (wall.hasCollided(bbox, edge)) {
			return true;
		}
	}
	return false;
}


bool TeamAgainst::willCollide(const AARectangle &aBBox, PlayerMovement direction) const {
	AARectangle bbox = aBBox;

	bbox.moveBy(getMovementVector(direction));

	BoundaryEdge edge;

	for (const Excluder &wall : mPlayerBoundaries) {
		if (wall.hasCollided(bbox, edge)) {
			return true;
		}
	}
	return false;
}
bool TeamAgainst::willCollide(const Defender &defender, const DefenderAI &defenderAI, PlayerMovement direction) const {
	AARectangle bbox = defender.getBoundingBox();

	bbox.moveBy(getMovementVector(direction));
	BoundaryEdge edge;
	for (const Excluder &wall : mPlayerBoundaries) {
		if (wall.hasCollided(bbox, edge)) {
			//std::cout << "defender colliding" << std::endl;
			return true;
		}
	}
	return false;
}
void TeamAgainst::resetLevel() {

}

bool TeamAgainst::isGameOver() const {
	return false;
}
void TeamAgainst::increaseLevel() {

}
void TeamAgainst::resetToFirstGame() {

}
void TeamAgainst::ballBounce(SoccerBall &soccerBall) {
	for (const Excluder& ballWall : mBallBoundaries) {
		BoundaryEdge edge;
		if (ballWall.hasCollided(soccerBall.getBoundingBox(), edge)) {
			soccerBall.bounce(edge);
		}
	}
}

bool TeamAgainst::loadLevel(const std::string &levelPath) {
	FileCommandLoader fileLoader;

	std::string bgImageName;
	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &bgImageName](ParseFuncParams params) {
		bgImageName = FileCommandLoader::readString(params);
		bool loaded = mBGImage.load(App::singleton().getBasePath() + std::string("Assets/") + bgImageName);
		assert(loaded && "Didn't load the bg image");
	};
	fileLoader.addCommand(bgImageCommand);

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params) {
		mTileWidth = FileCommandLoader::readInt(params);
		//mTiles.back().width = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params) {
		mTileHeight = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.push_back(Tile());
	};
	fileLoader.addCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().symbol = FileCommandLoader::readChar(params);
	};
	fileLoader.addCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_ball_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().collidableBall = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileCollisionCommand);

	Command tilePlayerCollisionCommand;
	tilePlayerCollisionCommand.command = "tile_player_collision";
	tilePlayerCollisionCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().collidablePlayer = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tilePlayerCollisionCommand);

	Command boundsCommand;
	boundsCommand.command = "bounds";
	boundsCommand.parseFunc = [this](ParseFuncParams params) {
		bounds = FileCommandLoader::readSize(params);
	};
	fileLoader.addCommand(boundsCommand);

	Command tileGoalieCollisionCommand;
	tileGoalieCollisionCommand.command = "tile_goalie_collision";
	tileGoalieCollisionCommand.parseFunc = [this](ParseFuncParams params) {
		//mTiles.back().collidableBall = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileGoalieCollisionCommand);

	Vec2D layoutOffset;

	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params) {
		mLayoutOffset = FileCommandLoader::readSize(params);
		layoutOffset = mLayoutOffset;
	};
	fileLoader.addCommand(layoutOffsetCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().offset = FileCommandLoader::readSize(params);
	};
	fileLoader.addCommand(tileOffsetCommand);

	Command tilePlayerSpawnPointCommand;
	tilePlayerSpawnPointCommand.command = "tile_player_spawn_point";
	tilePlayerSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().playerSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tilePlayerSpawnPointCommand);

	Command tileCDMSpawnPointCommand;
	tileCDMSpawnPointCommand.command = "tile_center_defensive_midfielder_spawn_point";
	tileCDMSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().centerDefensiveMidfielderSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileCDMSpawnPointCommand);

	Command tileLBSpawnPointCommand;
	tileLBSpawnPointCommand.command = "tile_left_back_spawn_point";
	tileLBSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().leftBackSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileLBSpawnPointCommand);

	Command tileRBSpawnPointCommand;
	tileRBSpawnPointCommand.command = "tile_right_back_spawn_point";
	tileRBSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().rightBackSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileRBSpawnPointCommand);

	Command tileCBSpawnPointCommand;
	tileCBSpawnPointCommand.command = "tile_center_back_spawn_point";
	tileCBSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().centerBackSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileCBSpawnPointCommand);

	Command tileGKSpawnPointCommand;
	tileGKSpawnPointCommand.command = "tile_keeper_spawn_point";
	tileGKSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().goalkeeperSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileGKSpawnPointCommand);


	Command tileGoalCommand;
	tileGoalCommand.command = "tile_is_goal";
	tileGoalCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().isGoal = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileGoalCommand);

	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params) {
		int startingX = layoutOffset.GetX();
		//std::cout << "line:" << params.line << std::endl;
		for (size_t c = 0; c < params.line.length(); ++c) {
			Tile *tile = getTileForSymbol(params.line[c]);
			if (tile) {
				tile->position = Vec2D(startingX, layoutOffset.GetY());
				if (tile->isGoal > 0) {
					Excluder goal;

					goal.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), static_cast<int>(mTileWidth),
									static_cast<int>(mTileHeight)));
					mGoal.push_back(goal);
				} else if (tile->collidablePlayer > 0) {
					Excluder wall;
					wall.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), static_cast<int>(mTileWidth),
									static_cast<int>(mTileHeight)));
					mPlayerBoundaries.push_back(wall);
				} else if (tile->collidableBall > 0) {
					Excluder wall;
					wall.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), static_cast<int>(mTileWidth),
									static_cast<int>(mTileHeight)));
					mBallBoundaries.push_back(wall);
				} else if (tile->collidableGoalie > 0) {
					Excluder wall;
					wall.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), static_cast<int>(mTileWidth),
									static_cast<int>(mTileHeight)));
					mGoalieBoundaries.push_back(wall);
				}

				if (tile->playerSpawnPoint > 0) {
					mPlayerSpawnLocation = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->ballSpawnPoint > 0) {
					//TO-DO implement ball spawn
					/*mBonusItem.bbox = AARectangle(
							Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY()),
					 SPRITE_WIDTH, SPRITE_HEIGHT);*/
				} else if (tile->goalkeeperSpawnPoint > 0) {
					mDefendersSpawnPoints[GOALKEEPER] = Vec2D(startingX + tile->offset.GetX() + 1,
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->centerBackSpawnPoint > 0) {
					mDefendersSpawnPoints[CENTER_BACK] = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->rightBackSpawnPoint > 0) {
					mDefendersSpawnPoints[RIGHT_BACK] = Vec2D(startingX + tile->offset.GetX() + 1,
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->leftBackSpawnPoint > 0) {
					mDefendersSpawnPoints[LEFT_BACK] = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->centerDefensiveMidfielderSpawnPoint > 0) {
					mDefendersSpawnPoints[CENTER_DEFENSIVE_MIDFIELDER] = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				}

				if (tile->isOutOfBoundsTile > 0) {
					mExclusionTiles.push_back(*tile);
				}

				startingX += static_cast<int>(mTileWidth);
			}
		}
		layoutOffset += Vec2D(0, mTileHeight);
	};

	fileLoader.addCommand(layoutCommand);

	return fileLoader.loadFile(levelPath);
}
TeamAgainst::Tile* TeamAgainst::getTileForSymbol(char symbol) {
	for (size_t i = 0; i < mTiles.size(); ++i) {
		if (mTiles[i].symbol == symbol) {
			return &mTiles[i];
		}
	}
	return nullptr;
}

