# Arcade Game Collection

A collection of classic arcade games built in C++ using SDL2, with retro graphics and authentic arcade gameplay.

## Table of Contents
- [Overview](#overview)
- [Games Included](#games-included)
- [Getting Started](#getting-started)
- [How to Play](#how-to-play)
- [Project Architecture](#project-architecture)
- [Technical Details](#technical-details)
- [Development](#development)

## Overview

This is a arcade game collection that recreates the classic arcade experience. The project features multiple playable arcade games with authentic feeling controls and gameplay.

**Key Features:**
- Multiple classic arcade games in one application
- Retro pixel art graphics and animations
- Smooth gameplay with fixed timestep physics
- Keyboard controls optimized for arcade style gameplay
- Sprite based animation system
- Modular game architecture for easy expansion

## Games Included

### 1. Soccer (Active Development)
A fast paced arcade soccer game where you control a player trying to score goals against AI defenders.

**Objective:** Score 3 goals within 3 minutes to win!

**Features:**
- 5 AI defenders with intelligent positioning and behavior
- Multiple player states (running, sprinting, ball possession)
- Ball physics with bouncing and collision detection
- Zone-based defender AI
- Time-based gameplay

### 2. Tetris (Active Development)
Classic block dropping puzzle game with standard Tetris mechanics.

**Objective:** Clear lines by completing horizontal rows with falling tetrominoes.

**Features:**
- Traditional 7 tetromino shapes
- Line clearing mechanics
- Progressive difficulty
- Score tracking

### 3. Break Out! 
Paddle and ball brick breaker game.

**Objective:** Break all blocks using the ball while keeping it from falling off the bottom.

**Features:**
- Multiple levels with different block layouts
- 3 lives per game
- Ball physics with angle-based bouncing
- Paddle control

### 4. Pac-Man 
Classic maze chase game with ghost AI.

**Objective:** Collect all pellets while avoiding ghosts, or eat power pellets to chase them!

**Features:**
- Authentic maze navigation
- Multiple ghost types with different AI behaviors
- Power-up system
- Score and life tracking

### 5. Asteroids
*(Placeholder - Not yet implemented)*

## Getting Started

### Prerequisites

- **C++ Compiler** (C++11 or later)
- **SDL2 Library** (Simple DirectMedia Layer 2)

### Building the Project (For now)

This project uses Eclipse CDT as its build system, at the moment but I plan to change this soon to an executable or something better.

#### Using Eclipse CDT:
1. Open Eclipse CDT
2. Import the project: `File > Import > Existing Projects into Workspace`
3. Select the `Application` directory
4. Build the project: `Project > Build Project` or `Ctrl+B`
5. Run the executable from the `Debug` folder

#### Assets Required:
The game requires asset files located in `Debug/Assets/`:
- `ArcadeFont.txt` - Bitmap font definitions
- `*_animations.txt` - Animation definitions for each game
- `*.bmp` - Sprite sheets and level backgrounds
- `*_level.txt` - Level data files

Ensure all assets are present in the `Debug/Assets` directory before running.

### Running the Application

After building, run the executable:
```
Application/Debug/Application.exe
```

The arcade menu will appear, allowing you to select a game using keyboard controls.

## How to Play

### Universal Controls

All games use WASD-based controls for consistency:

| Key    	| Action                      	|
|--------	|-----------------------------	|
| W      	| Move Up / Navigate Up       	|
| A      	| Move Left / Navigate Left   	|
| S      	| Move Down / Navigate Down   	|
| D      	| Move Right / Navigate Right 	|
| SPACE  	| Action / Kick / Rotate      	|
| LSHIFT 	| Sprint / Special            	|
| RETURN 	| Start / Select / Confirm    	|
| ESCAPE 	| Back / Cancel / Quit        	|

### Game-Specific Controls

#### Soccer
- **Movement:** `W/A/S/D` - Move player in 8 directions
- **Kick:** `SPACE` - Kick the ball when nearby
- **Sprint:** `LSHIFT` (hold) - Run faster (drains stamina)
- **Start/Restart:** `RETURN` - Begin game or restart after game over

**How to Play:**
1. Select Soccer from the arcade menu
2. Press `RETURN` to start
3. Use `W/A/S/D` to move your player around the field
4. Get close to the ball and press `SPACE` to kick it toward the goal
5. Hold `LSHIFT` to sprint past defenders (works with or without ball)
6. Score 3 goals before the 3-minute timer runs out to win!

**Tips:**
- Sprinting makes you faster but is harder to control
- Kick the ball past defenders instead of trying to dribble through them
- Watch out for the goalkeeper - they're tough to beat!
- Ball bounces off players
- If the goalkeeper gets the ball or it goes out of bounds, play resets

**Player States:**
- Normal running: 50 pixels/frame
- Sprinting: 75 pixels/frame
- With ball: Slightly slower for control
- Defender speeds vary by position (GK, CB, LB, RB, CDM)

#### Tetris
- **Move Left/Right:** `A/D`
- **Rotate:** `W` or `SPACE`
- **Soft Drop:** `S` - Increase falling speed
- **Start:** `RETURN`

**How to Play:**
1. Falling blocks (tetrominoes) appear at the top
2. Position and rotate them to form complete horizontal lines
3. Complete lines disappear and award points
4. Game ends when blocks reach the top

#### Break Out
- **Move Paddle:** `A/D` - Move left/right
- **Launch Ball:** `SPACE`
- **Start:** `RETURN`

**How to Play:**
1. Control the paddle at the bottom of the screen
2. Bounce the ball upward to break blocks
3. Don't let the ball fall past your paddle!
4. Clear all blocks to complete the level
5. You have 3 lives

#### Pac-Man
- **Movement:** `W/A/S/D` - Navigate maze
- **Start:** `RETURN`

**How to Play:**
1. Navigate the maze collecting all pellets
2. Avoid ghosts - contact means losing a life
3. Eat power pellets (large dots) to temporarily chase ghosts
4. Clear all pellets to advance to the next level

## Project Architecture

### Directory Structure

```
Application/
├── src/
│   ├── Application.cpp          # Main entry point
│   ├── App/                     # Core application framework
│   │   ├── App.h/cpp            # Singleton app manager, game loop
│   │   ├── Button.h/cpp         # UI button component
│   │   └── ArcadeScene.cpp      # Main menu selection
│   │
│   ├── Scenes/                  # Scene/screen management
│   │   ├── Scene.h              # Scene interface
│   │   ├── GameScene.h/cpp      # Generic game wrapper
│   │   ├── ArcadeScene.h/cpp    # Arcade menu
│   │   └── ButtonOptionsScene.h # Menu base class
│   │
│   ├── Games/                   # Game implementations
│   │   ├── Game.h               # Game interface
│   │   ├── Soccer/              # Soccer game (active development)
│   │   ├── Tetris/              # Tetris implementation
│   │   ├── BreakOut/            # Breakout implementation
│   │   └── Pacman/              # Pac-Man implementation
│   │
│   ├── Graphics/                # Rendering system
│   │   ├── Screen.h/cpp         # Main SDL2 drawing interface
│   │   ├── ScreenBuffer.h/cpp   # Pixel buffer
│   │   ├── Color.h/cpp          # Color definitions
│   │   ├── SpriteSheet.h/cpp    # Sprite management
│   │   ├── AnimatedSprite.h/cpp # Animation system
│   │   └── BitmapFont.h/cpp     # Text rendering
│   │
│   ├── Input/                   # Input handling
│   │   ├── GameController.h/cpp # Keyboard to action mapping
│   │   ├── InputController.h/cpp# SDL2 input handler
│   │   └── InputAction.h        # Action callbacks
│   │
│   ├── Shapes/                  # 2D geometry & collision
│   │   ├── AARectangle.h/cpp    # Axis-aligned rectangles
│   │   ├── Circle.h/cpp         # Circle collision
│   │   ├── Line2D.h/cpp         # Line segments
│   │   └── Triangle.h/cpp       # Triangle shapes
│   │
│   └── utils/                   # Utilities
│       ├── Vec2D.h/cpp          # 2D vector math
│       ├── Utils.h/cpp          # General utilities
│       └── FileCommandLoader.h  # Asset loading
│
└── Debug/
    ├── Application.exe          # Compiled executable
    └── Assets/                  # Game assets (runtime)
        ├── ArcadeFont.txt
        ├── Soccer_animations.txt
        ├── Soccer_level.txt
        ├── SoccerSprites.bmp
        └── ...
```

### Core Systems

#### 1. Application Loop
The application uses a fixed timestep game loop with accumulator pattern:
- **Fixed timestep:** 10ms per update
- **Variable render rate:** As fast as possible

```cpp
while (running) {
    accumulator += deltaTime;

    // Process input
    inputController.update(dt);

    // Fixed update loop
    while (accumulator >= FIXED_DT) {
        currentScene->update(FIXED_DT);
        accumulator -= FIXED_DT;
    }

    // Render
    currentScene->draw(screen);
    screen.swapScreens();
}
```

#### 2. Scene Management
- Scene stack-based navigation
- Each scene implements `Scene` interface with `init()`, `update()`, `draw()`
- Push/pop scenes for menu navigation
- `GameScene` wraps `Game` implementations

#### 3. Input System
Three-layer input architecture:
1. **InputController** - Raw SDL2 event handling
2. **GameController** - Maps keys to action callbacks
3. **Game/Scene** - Registers button actions with lambdas

```cpp
ButtonAction kickAction;
kickAction.key = GameController::spaceKey();
kickAction.action = [this](uint32_t dt, InputState state) {
    if (GameController::isPressed(state)) {
        player.kick(ball);
    }
};
controller.addInputActionForKey(kickAction);
```

#### 4. Graphics Pipeline
- **Screen** - Main SDL2 rendering interface (224x288 pixels, 2x magnification)
- **ScreenBuffer** - Pixel-level drawing buffer for direct manipulation
- **SpriteSheet** - Loads sprites from BMP images and text definitions
- **AnimatedSprite** - Frame-based animation playback (typically 15 FPS)
- **BitmapFont** - Bitmap-based text rendering with arcade font

#### 5. Physics & Collision
- **Vec2D** - 2D vector math (add, subtract, dot product, magnitude, normalize)
- **Shape classes** - Collision detection (rectangles, circles, lines)
- Collision functions: `intersects()`, `containsPoint()`, `bounceOff()`

## Technical Details

### Graphics Technology
- **Library:** SDL2 (Simple DirectMedia Layer 2)
- **Rendering:** Hardware-accelerated using SDL_Renderer and SDL_Texture
- **Resolution:** 224x288 pixels 
- **Scaling:** 2x magnification for modern displays
- **Sprite Format:** BMP images with text-based sprite definitions
- **Animation:** Frame-based with configurable FPS 

### Physics System
- Fixed timestep (10ms) for deterministic physics
- Velocity-based movement with friction
- Collision detection using geometric shapes
- Bounce physics with normal reflection
- Ball-player interactions with momentum transfer

### AI Systems

#### Soccer Defender AI
- **Zone-based defense:** Each defender guards a specific area
- **Position types:** GK (Goalkeeper), LB (Left Back), CB (Center Back), RB (Right Back), CDM (Center Defensive Midfielder)
- **Behavior states:** Idle, Chase, Defend Zone, Intercept
- **Decision making:** Distance-based threat assessment
- **Speed variation:** Different speeds for different actions (normal, jogging, sprinting)

#### Pac-Man Ghost AI
- Multiple personality types (Blinky, Pinky, Inky, Clyde)
- Chase and scatter modes
- Pathfinding through maze
- Frightened state when power pellet active

### Animation System
Animation definitions are text-based for easy editing:

```
:animation move_left
:sprite_sheet SoccerSprites
:size 12 12
:fps 15
:frame_keys 4
player_moving_left_1
player_moving_left_2
player_moving_left_1
player_moving_left_3
```

Each frame references a sprite in the associated sprite sheet BMP file.

### Level Format (Soccer Example)
Levels use ASCII-based tile maps:

```
w = Wall (blocks player and ball)
o = Ball collision only
s = Goal scoring area
b = Goalie zone
- = Open space
c/r/l/k = Defender spawn points
```

Level files pair a text based collision map with a BMP background image.

## Development

### Current Development Status

**Active Branch:** `SoccerStartScene`

**Recent Changes:**
- New player states (sprinting, ball possession variants) - `SoccerGame.cpp`
- Improved defender AI logic - `Defender.h/cpp`, `DefenderAI.h`
- Kicking mechanics refinement - `Player.cpp`
- Enhanced game controller input handling - `GameController.cpp/h`

**Known Issues/Bugs:**
- Some bugs fixed by resetting the game frequently (noted in commit messages)
- Wall bounce physics recently implemented
- Defender movement AI improvements ongoing

### Adding a New Game

To add a new game to the arcade:

1. **Create game class** inheriting from `Game` interface:
   ```cpp
   class MyGame : public Game {
   public:
       void init(GameController& controller) override;
       void update(uint32_t dt) override;
       void draw(Screen& screen) override;
       std::string getName() const override;
   };
   ```

2. **Implement required methods:**
   - `init()` - Set up game state and register input actions
   - `update()` - Game logic, physics, collision detection
   - `draw()` - Render sprites, shapes, and text
   - `getName()` - Return game name for menu

3. **Register in ArcadeScene:**
   Add button in `ArcadeScene.cpp` to launch your game:
   ```cpp
   Button myGameButton("My Game", ...);
   buttons.push_back(myGameButton);
   ```

4. **Create assets:**
   - Sprite sheet BMP file
   - Animation definitions text file
   - Level data (if applicable)
   - Place in `Debug/Assets/`

### Asset Creation Guidelines

**Sprite Sheets:**
- Use BMP format (Windows Bitmap)
- Define sprite locations in accompanying `.txt` file
- Use consistent pixel dimensions (e.g., 12x12 for characters)

**Animations:**
- Define in `{GameName}_animations.txt`
- Specify FPS 
- List frame keys in sequence
- Support looping and one shot animations

**Levels:**
- Use ASCII based tile maps for collision
- Pair with BMP background image
- Define tile meanings in game's level loader

### Code Style
- Modern C++11 features (lambdas, smart pointers, auto)
- Virtual inheritance for interfaces
- Clear naming conventions (mMemberVariables, PascalCase for classes)
