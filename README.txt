========================================
  INTERIOR DESIGN - MODERN HOME OFFICE
  Computer Graphics Lab Project (OpenGL)
========================================

PROJECT OVERVIEW
----------------
This project renders a stylized smart home office using modern OpenGL 2D primitives.
The scene highlights classic computer graphics algorithms (DDA, Bresenham, Midpoint
Circle) alongside delta-time animation for a smooth 120 FPS experience. Major props
include a wooden desk, dual-display computer, pendulum clock, ceiling fan, RGB lamp,
smart control panel, bookshelf, and decorative accessories.

HOW TO BUILD
------------
1. Install MinGW-w64 with freeglut (the repo assumes C:\msys64\mingw64).
2. From the `g:\CG` folder run:
   `g++ interior_design.cpp -lfreeglut -lopengl32 -lglu32 -o interior_design.exe`
3. Launch the executable: `interior_design.exe`

CONTROLS
--------
- `Esc` : Exit
- `R`   : Reset all animated angles to their starting pose
- `F`   : Give the ceiling fan a manual nudge (adds +10°)

MAIN FEATURES
-------------
- Delta-time animation timer (~8 ms) for smooth pendulum, fan, clock hands, and UI pulses.
- Layered gradients for walls/floor to mimic indirect lighting.
- Smart panel with HSV-driven music visualizer, animated Wi-Fi indicator, and status LEDs.
- Realistic cup steam, dynamic monitor content, swinging desk lamp, bookshelf accessories.
- Reusable helpers: DDA line, Bresenham line, Midpoint circle, filled circle, gradient quad.
- Explicit 2D transformations: lamp swing (rotation), ceiling fan hub translate+rotate, smart-panel touch button scaling pulse.

ALGORITHMS BY OBJECT
--------------------
| Object / Effect            | Algorithm(s) Used                                    |
|----------------------------|------------------------------------------------------|
| Lamp cord & bookshelf supports | Bresenham line drawing                            |
| Book spines, desk dividers      | DDA line drawing                                 |
| Clock bezel, pendulum weights, LEDs | Midpoint circle + triangle fan fills        |
| Coffee cup steam & dust          | Filled circles (triangle fan) with alpha       |
| Smart control panel              | GL_QUADS (drawRect) + HSV→RGB music bars       |
| Ceiling fan blades               | GL_QUADS + rotation transform                  |
| Computer monitor shader          | Gradient quads + sine-based wave animation     |
| Smart panel touch button        | Translation + Scaling (glTranslatef/glScalef)                      |

ANIMATION SUMMARY
-----------------
- `update()` computes deltaTime and advances lamp swing, fan rotation, clock hands,
  pendulum swing, dust particles, monitor waves, smart-panel pulses, and the desk
  plant's translate/scale/rotate cycle.
- Animations stay stable regardless of machine speed because everything is tied to time
  differences reported by `glutGet(GLUT_ELAPSED_TIME)`.

FILE LIST
---------
- `interior_design.cpp` : Full source for the home office scene (≈1K lines).
- `build.bat`           : Convenience script (same command as above).

CREDITS / NOTES
---------------
- Built with FreeGLUT and classic fixed-function OpenGL for teaching purposes.
- All scene geometry is authored manually with helper algorithms—no external assets.
- Feel free to remix props or swap color palettes to create your own themed workspace.

========================================
  Enjoy exploring the smart workspace!
========================================

