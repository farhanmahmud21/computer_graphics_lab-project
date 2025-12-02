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
| Lamp cord & bookshelf supports | Bresenham line drawing (pixel-accurate integer steps) |
| Book spines, desk dividers      | DDA line drawing (floating-point slope stepping) |
| Clock bezel & pendulum weights         | Midpoint Circle (outline) + GL_TRIANGLE_FAN fill |
| Coffee cup steam & dust          | Filled circles (GL_TRIANGLE_FAN) with alpha for softness |
| Smart control panel              | GL_QUADS (rect helper) + HSV→RGB music bars + scaling button |
| Ceiling fan blades               | GL_QUADS + translate-to-hub + rotation animation |
| Computer monitor shader          | Gradient quads + sine-based wave animation     |
| Smart panel touch button        | Translation + Scaling (glTranslatef/glScalef)   |

SUPPORTING PRIMITIVES
---------------------
- **GL_QUADS**: OpenGL primitive that draws a four-vertex polygon. We call it via `drawRect()` to build walls, desk surfaces, fan blades, and the smart-panel frame—ensuring axis-aligned rectangles with two triangles under the hood.
- **Filled circle helper**: `drawFilledCircle()` emits a GL_TRIANGLE_FAN (center vertex + arc) to paint discs. When a crisp outline is needed instead, the Midpoint Circle algorithm (`drawCircleMidpoint`) plots perimeter pixels before the fan fill.

TEAM CONTRIBUTIONS
------------------
| Contributor | Feature Ownership & Algorithm Focus                            | Transformation Highlight |
|-------------|----------------------------------------------------------------|--------------------------|
| Farhan      | Clock & pendulum bezels using Midpoint Circle helper           | Pendulum/hand rotation sweep |
| Zisan       | Hanging lamp cord/shade using Bresenham lines                  | Lamp swing rotation pivot |
| Soroar      | Desk books & dividers detailed with DDA line work              | Smart-panel touch-button scaling pulse |
| (Shared)    | Coffee steam & dust (triangle fan fills)                       | N/A – alpha-fade particles |

OBJECT-BY-OBJECT BREAKDOWN
--------------------------
- **Room walls & floor** – `drawRoom()`: GL_QUADS for gradients, DDA lines for wood grain.
- **Desk** – `drawDesk()`: stacked GL_QUADS (drawRect helper) with subtle gradients.
- **Chair** – `drawChair()`: GL_QUADS for cushions, Midpoint circles for wheels, Bresenham for legs.
- **Lamp** – `drawLamp()`: Bresenham cord, GL_QUADS + triangle fans for shade, filled circles for glow.
- **Bookshelf & wall books** – `drawBookshelf()`, `drawBooks()`: GL_QUADS bodies with DDA/Bresenham accents.
- **Smart panel** – `drawSmartPanel()`: GL_QUADS for frame/glass, HSV→RGB bars, scaling touch ring.
- **Neon-less wall art slot** – currently omitted (kept free for future props).
- **Clock & pendulum** – `drawClock()`: Midpoint circle outlines + triangle-fan fills, rotation for hands.
- **Ceiling fan** – `drawCeilingFan()`: GL_QUADS blades, translate-to-hub + rotation animation.
- **Computer & keyboard** – `drawComputer()`, `drawKeyboard()`: Gradient GL_QUADS plus DDA grid lines.
- **Printer** – `drawPrinter()`: GL_QUADS body, filled circles for buttons.
- **Desk organizer** – `drawDeskOrganizer()`: GL_QUADS base, Bresenham diagonal pens, DDA vertical pen.
- **Coffee cup & steam** – `drawCoffeeCup()`: GL_QUADS body with darker lid cap, sine-sway steam.
- **Books on desk** – `drawBooks()`: stacked GL_QUADS with DDA spines.
- **Dust particles** – `drawParticles()`: filled circles (GL_TRIANGLE_FAN) with alpha fade.
- **Smart panel indicators** – Wi-Fi arcs drawn via parametric sine/cos; status LEDs via filled circles.

SCENE ELEMENTS SNAPSHOT
-----------------------
| Element       | Description                                      |
|---------------|--------------------------------------------------|
| Office Chair  | Wheels, curved back, small headrest shadow       |
| Computer Rig  | Dual monitors, blinking shader, keyboard + mouse |
| Printer       | Tray, buttons, paper output slot                 |
| Books         | Stacked on desk and shelf with varied scaling    |
| Coffee Cup    | Lid, sleeve, animated steam wisps                |
| Wall Art Slot | Reserved space (currently blank by request)      |
| Hanging Lamp  | Red dome shade with swing animation              |
| Ceiling Fan   | Four blades rotating from hub                    |
| Wall Clock    | Moving hands, pendulum bob, layered frame        |

REQUIREMENT COVERAGE
--------------------
| Requirement          | Implementation Examples                                      |
|----------------------|--------------------------------------------------------------|
| Graphics Primitives  | Points (DDA/Bresenham), GL_QUADS, GL_TRIANGLE_FAN circles    |
| DDA Algorithm        | Desk drawer handles, desk books, steam sway, wood grain      |
| Bresenham Algorithm  | Lamp cord, chair legs, shelf brackets, organizer pens        |
| Midpoint Circle      | Clock bezel, pendulum weights, printer buttons               |
| Translation          | Fan hub placement, pendulum anchor, smart-panel widgets      |
| Rotation             | Lamp swing pivot, fan blades, clock hands, pendulum bob      |
| Scaling              | Smart-panel touch button pulse, varied book sizes            |
| Animation Loop       | Lamp swing, fan spin, clock sweep, screen blink, steam drift |

ANIMATION SUMMARY
-----------------
- `update()` computes deltaTime and advances lamp swing, fan rotation, clock hands,
  pendulum swing, dust particles, monitor waves, and the smart-panel touch-button pulse.
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

