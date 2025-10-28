/*
 * Scene.h
 *
 *  Created on: Jul 10, 2023
 *      Author: nebk2
 */

#ifndef SCENES_SCENE_H_
#define SCENES_SCENE_H_

#include <string>
#include <stdint.h>
#include "../Input/GameController.h"

class Screen;

//Interface
class Scene{
public:
	virtual ~Scene() {}
	virtual void init() = 0;
	virtual void update(uint32_t dt) = 0;
	virtual void draw(Screen& theScreen) = 0;
	virtual const std::string& getSceneName() const = 0;

	GameController* getGameController() {return &mGameController;}
protected:
	GameController mGameController;
};



#endif /* SCENES_SCENE_H_ */
