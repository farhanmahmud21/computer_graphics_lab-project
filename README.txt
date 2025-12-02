========================================
   VILLAGE SCENERY - OpenGL Project
   Computer Graphics Assignment
========================================

STUDENT PROJECT INFORMATION:
----------------------------
Project Title: Village Scenery with Moving Objects
Project Number: 1 (from project list)


FEATURES IMPLEMENTED:
--------------------

1. BASIC GRAPHICS PRIMITIVES:
   - Points: Used in DDA and Midpoint algorithms
   - Lines: Fence, road markings, bird wings
   - Polygons: Houses, trees, mountains, river, sky
   - Circles: Sun, clouds, windmill hub

2. GRAPHICS ALGORITHMS:
   a) DDA (Digital Differential Analyzer) Line Algorithm:
      - Location: drawLineDDA() function (Line 33-55)
      - Used for: Sun rays, river waves, road markings, bird shapes
      
   b) Midpoint Circle Algorithm:
      - Location: drawCircleMidpoint() function (Line 62-89)
      - Used for: Sun outline, mountain snow caps

3. 2D TRANSFORMATIONS:
   a) Translation:
      - Moving clouds (cloudX variable)
      - Flying birds (birdX variable)
      - Moving boat (boatX variable)
      - Rising sun (sunY variable)
      
   b) Rotation:
      - Windmill blades using glRotatef()
      - Located in drawWindmill() function
      
   c) Scaling:
      - Houses drawn at different sizes (scale parameter)
      - Trees drawn at different sizes (scale parameter)
      - See drawHouse() and drawTree() functions

4. ANIMATION:
   - Sun rising animation
   - Clouds moving from left to right
   - Birds flying from right to left
   - Boat moving on river
   - Windmill rotating continuously


HOW TO BUILD AND RUN:
--------------------

METHOD 1: Using Visual Studio Code with MinGW
----------------------------------------------
1. Open Command Prompt in project folder
2. Run: g++ village_scenery.cpp -o village_scenery.exe -lfreeglut -lopengl32 -lglu32
3. Run: village_scenery.exe

METHOD 2: Using Code::Blocks
----------------------------
1. Create new Console Application (C++)
2. Copy village_scenery.cpp code into main.cpp
3. Go to Project > Build Options > Linker Settings
4. Add these libraries:
   - freeglut
   - opengl32
   - glu32
5. Build and Run (F9)

METHOD 3: Using Visual Studio
-----------------------------
1. Create new Empty Project (C++)
2. Add village_scenery.cpp to Source Files
3. Install freeglut using NuGet Package Manager
4. Or manually add libraries:
   - Project Properties > Linker > Input > Additional Dependencies
   - Add: freeglut.lib;opengl32.lib;glu32.lib
5. Build and Run (Ctrl+F5)


INSTALLING FREEGLUT (if not installed):
--------------------------------------
1. Download freeglut from: https://www.transmissionzero.co.uk/software/freeglut-devel/
2. Extract the zip file
3. Copy files:
   - freeglut.dll to C:\Windows\System32 (or project folder)
   - freeglut.lib to your compiler's lib folder
   - GL folder (headers) to your compiler's include folder


CONTROLS:
---------
- ESC key: Exit the program
- R key: Reset all animations


SCENE DESCRIPTION:
-----------------
The village scenery includes:
- Sky with moving clouds
- Mountains with snow caps in background
- Rising sun with rays
- Three houses of different sizes
- Trees around the houses
- A windmill with rotating blades
- A river with a moving boat
- A road with lane markings
- Flying birds
- Fence near houses


CODE STRUCTURE:
--------------
1. Global Variables (Line 18-26): Animation control variables
2. DDA Algorithm (Line 33-55): Line drawing algorithm
3. Midpoint Circle (Line 62-89): Circle drawing algorithm
4. Drawing Functions (Line 97-380): All scene elements
5. Display Function (Line 384-430): Main rendering
6. Animation Update (Line 434-468): Animation logic
7. Init & Main (Line 472-530): Setup and main loop


ALGORITHMS EXPLAINED:
--------------------

DDA LINE ALGORITHM:
- Calculates points between two coordinates
- Uses floating point increments
- Steps = max(|dx|, |dy|)
- Simple but uses floating point math

MIDPOINT CIRCLE ALGORITHM:
- Uses integer arithmetic only
- Decision parameter: d = 1 - radius
- Exploits 8-way symmetry
- More efficient than direct calculation


========================================
   Thank you!
========================================

