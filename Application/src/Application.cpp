//============================================================================
// Name        : ArcadeApp.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "App/App.h"
const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 2;
const int DELAY = 50;
using namespace std;

int main(int argc, char *argv[]) {
	if (App::singleton().init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION)) {
		App::singleton().run();
	}
	return 0;
}

