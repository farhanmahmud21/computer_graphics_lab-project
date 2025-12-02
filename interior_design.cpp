/*
=================================================================
    INTERIOR DESIGN - Home Office Room
    OpenGL Computer Graphics Project
    
    Features:
    - Basic Graphics Primitives (points, lines, polygons, circles)
    - Graphics Algorithms (DDA Line, Bresenham Line, Midpoint Circle)
    - 2D Transformations (Translation, Rotation, Scaling, Shear)
    - Animations (Swinging Lamp, Clock hands, Fan rotation)
=================================================================
*/

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

// ==================== GLOBAL VARIABLES ====================
// Animation variables
float lampAngle = 0;           // Lamp swinging angle
float lampDirection = 1;       // Lamp swing direction
float fanAngle = 0;            // Ceiling fan rotation
float clockSecond = 0;         // Clock second hand angle
float clockMinute = 0;         // Clock minute hand angle
bool computerOn = true;        // Computer screen state
int blinkCounter = 0;          // For screen blinking effect
float glowPhase = 0;           // For pulsing glow effects
float particleY[5] = {0};      // Floating dust particles Y positions
float particleX[5] = {150, 300, 450, 550, 700}; // Particle X positions
float screenWave = 0;          // Monitor screen animation
float pendulumAngle = 0;       // Clock pendulum
float pendulumDir = 1;         // Pendulum direction

// Smart home & modern effects
float smartPanelGlow = 0;      // Smart panel indicator
float musicBar[5] = {0};       // Music visualizer bars

// Constants
const float PI = 3.14159265f;

// ==================== GRAPHICS ALGORITHMS ====================

/*
    DDA Line Drawing Algorithm
    - Digital Differential Analyzer
    - Uses floating point arithmetic
*/
void drawLineDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps;
    
    if (abs(dx) > abs(dy)) {
        steps = abs(dx);
    } else {
        steps = abs(dy);
    }
    
    if (steps == 0) steps = 1;
    
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    
    float x = x1;
    float y = y1;
    
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

/*
    Bresenham Line Drawing Algorithm
    - Uses only integer arithmetic
    - More efficient than DDA
*/
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x1, y1);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
}

/*
    Midpoint Circle Drawing Algorithm
    - Uses integer arithmetic
    - Exploits 8-way symmetry
*/
void drawCircleMidpoint(int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(cx + x, cy + y);
        glVertex2i(cx - x, cy + y);
        glVertex2i(cx + x, cy - y);
        glVertex2i(cx - x, cy - y);
        glVertex2i(cx + y, cy + x);
        glVertex2i(cx - y, cy + x);
        glVertex2i(cx + y, cy - x);
        glVertex2i(cx - y, cy - x);
        
        if (d < 0) {
            d = d + 2 * x + 3;
        } else {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

// Draw filled circle (triangle fan fill; Midpoint Circle is only used for outlines)
void drawFilledCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);  // Center point
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Draw filled rectangle helper
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

// HSV to RGB conversion for rainbow effects
void hsvToRgb(float h, float s, float v, float &r, float &g, float &b) {
    int i = (int)(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
}

// Draw gradient rectangle
void drawGradientRect(float x, float y, float w, float h, 
                      float r1, float g1, float b1,
                      float r2, float g2, float b2) {
    glBegin(GL_QUADS);
        glColor3f(r1, g1, b1);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glColor3f(r2, g2, b2);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

// ==================== ROOM ELEMENTS ====================

// Draw smart home control panel (GL_QUADS for panels + Midpoint circles for LEDs)
void drawSmartPanel() {
    const float panelX = 55.0f;
    const float panelY = 300.0f;
    const float panelW = 95.0f;
    const float panelH = 130.0f;
    
    // Panel frame (uses drawRect helper built on GL_QUADS)
    glColor3f(0.18f, 0.18f, 0.2f);
    drawRect(panelX - 6, panelY - 6, panelW + 12, panelH + 12);
    
    // Panel background
    glColor3f(0.1f, 0.1f, 0.14f);
    drawRect(panelX, panelY, panelW, panelH);
    
    // Animated glass gradient
    float pulse = 0.5f + 0.5f * sin(smartPanelGlow);
    float wave = sin(smartPanelGlow * 0.5f);
    glBegin(GL_QUADS);
        glColor3f(0.08f + 0.04f * wave, 0.12f + 0.08f * pulse, 0.28f + 0.12f * pulse);
        glVertex2f(panelX + 5, panelY + 5);
        glVertex2f(panelX + panelW - 5, panelY + 5);
        glColor3f(0.18f + 0.12f * pulse, 0.2f + 0.1f * wave, 0.4f + 0.12f * pulse);
        glVertex2f(panelX + panelW - 5, panelY + panelH - 5);
        glVertex2f(panelX + 5, panelY + panelH - 5);
    glEnd();
    
    // Music visualizer bars (HSV -> RGB for rainbow effect)
    float barBase = panelY + 12;
    for (int i = 0; i < 5; i++) {
        float h = 12 + musicBar[i] * (panelH - 50);
        float hue = 0.02f + i * 0.18f;
        float r, g, b;
        hsvToRgb(hue, 0.95f, 1.0f, r, g, b);
        glColor3f(r, g, b);
        drawRect(panelX + 8 + i * 16, barBase, 10, h);
    }
    
    // Temperature widget (stacked rectangles)
    glColor3f(1.0f, 0.55f, 0.15f);
    drawRect(panelX + 10, panelY + panelH - 55, 45, 20);
    glColor3f(0.25f, 0.1f, 0.05f);
    drawRect(panelX + 12, panelY + panelH - 53, 41, 16);
    glColor3f(1.0f, 0.85f, 0.4f);
    drawRect(panelX + 15, panelY + panelH - 49, 18, 8);
    drawRect(panelX + 37, panelY + panelH - 49, 8, 8);
    
    // WiFi icon kept well inside the frame
    float wifiCx = panelX + panelW - 20;
    float wifiCy = panelY + panelH - 20;
    for (int i = 0; i < 3; i++) {
        float radius = 4 + i * 4;
        float waveDelay = sin(smartPanelGlow * 3 - i * 0.6f);
        float brightness = 0.6f + 0.4f * waveDelay;
        glColor3f(0.2f * brightness, 0.95f * brightness, 0.5f * brightness);
        glLineWidth(2);
        glBegin(GL_LINE_STRIP);
        for (int a = 45; a <= 135; a += 10) {
            float ang = a * PI / 180;
            glVertex2f(wifiCx + radius * cos(ang), wifiCy + radius * sin(ang));
        }
        glEnd();
    }
    glLineWidth(1);
    glColor3f(0.3f, 1.0f, 0.6f);
    drawFilledCircle(wifiCx, wifiCy - 4, 2, 10);
    
    // Status LED
    glColor3f(0.25f + 0.75f * pulse, 0.2f, 0.4f);
    drawFilledCircle(panelX + 18, panelY + panelH - 18, 4, 12);
    
    // Contributor: Soroar – Touch target / power button with SCALING transform
    float touchScale = 1.0f + 0.18f * sin(smartPanelGlow * 1.2f);
    glPushMatrix();
    glTranslatef(panelX + panelW - 35, panelY + 22, 0);
    glScalef(touchScale, touchScale, 1.0f);
    glColor3f(0.2f * pulse, 0.45f * pulse, 0.85f * pulse);
    drawFilledCircle(0, 0, 8, 18);
    glColor3f(0.4f + 0.2f * pulse, 0.75f + 0.2f * pulse, 1.0f);
    drawFilledCircle(0, 0, 4, 12);
    glPopMatrix();
}

// Draw the room walls and floor
void drawRoom() {
    // Back wall with subtle gradient
    glBegin(GL_QUADS);
        glColor3f(0.85f, 0.65f, 0.45f);
        glVertex2f(0, 100);
        glVertex2f(800, 100);
        glColor3f(0.95f, 0.78f, 0.58f);
        glVertex2f(800, 500);
        glVertex2f(0, 500);
    glEnd();
    
    // Floor (reflective wood with gradient)
    glBegin(GL_QUADS);
        glColor3f(0.55f, 0.40f, 0.28f);
        glVertex2f(0, 0);
        glVertex2f(800, 0);
        glColor3f(0.72f, 0.56f, 0.40f);
        glVertex2f(800, 100);
        glVertex2f(0, 100);
    glEnd();
    
    // Floor wood grain lines
    glColor3f(0.5f, 0.38f, 0.25f);
    for (int i = 0; i < 8; i++) {
        drawLineDDA(0, 12 + i * 12, 800, 10 + i * 12);
    }
    
    // Baseboard
    glColor3f(0.7f, 0.7f, 0.7f);
    drawRect(0, 93, 800, 2);
    glColor3f(0.92f, 0.92f, 0.92f);
    drawRect(0, 95, 800, 8);
}

// Contributor: Zisan – Lamp cord via Bresenham; lamp swing uses rotation transform
// hanging lamp
void drawLamp() {
    glPushMatrix();
    
    // Pivot point at ceiling
    glTranslatef(400, 480, 0);
    glRotatef(lampAngle, 0, 0, 1);  // ROTATION transformation
    glTranslatef(-400, -480, 0);
    
    // Lamp cord using Bresenham
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(2);
    drawLineBresenham(400, 500, 400, 430);
    
    // Lamp shade (red dome)
    glColor3f(0.85f, 0.2f, 0.15f);
    glBegin(GL_QUADS);
        glVertex2f(360, 430);
        glVertex2f(440, 430);
        glVertex2f(420, 400);
        glVertex2f(380, 400);
    glEnd();
    
    // Lamp top curve
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(400, 430);  // Center
    for (int i = 0; i <= 180; i += 10) {
        float angle = i * PI / 180;
        glVertex2f(400 + 40 * cos(angle), 430 + 15 * sin(angle));
    }
    glEnd();
    
    // Light glow effect (pulsing)
    float glow = 0.6f + 0.4f * sin(glowPhase * 1.5f);
    glColor3f(1.0f * glow, 0.9f * glow, 0.5f * glow);
    drawFilledCircle(400, 390, 18, 25);
    
    // Light bulb (bright yellow center)
    glColor3f(1.0f, 0.98f, 0.8f);
    drawFilledCircle(400, 395, 8, 20);
    
    // Light rays
    glColor3f(1.0f * glow * 0.5f, 0.95f * glow * 0.5f, 0.6f * glow * 0.3f);
    for (int i = 0; i < 8; i++) {
        float rayAngle = i * 45 * PI / 180 + glowPhase * 0.2f;
        float x1 = 400 + 12 * cos(rayAngle);
        float y1 = 390 + 12 * sin(rayAngle);
        float x2 = 400 + (25 + 5 * sin(glowPhase + i)) * cos(rayAngle);
        float y2 = 390 + (25 + 5 * sin(glowPhase + i)) * sin(rayAngle);
        drawLineDDA(x1, y1, x2, y2);
    }
    
    glPopMatrix();
}

// Draw the desk with drawers
void drawDesk() {
    // Desk top (gray)
    glColor3f(0.5f, 0.5f, 0.5f);
    drawRect(80, 180, 640, 15);
    
    // Left drawer unit
    glColor3f(0.6f, 0.6f, 0.6f);
    drawRect(80, 50, 120, 130);
    
    // Left drawer fronts
    glColor3f(0.55f, 0.55f, 0.55f);
    drawRect(85, 120, 110, 35);
    drawRect(85, 80, 110, 35);
    drawRect(85, 40, 110, 35);
    
    // Drawer handles using DDA lines
    glColor3f(0.3f, 0.3f, 0.3f);
    glPointSize(2);
    drawLineDDA(120, 137, 160, 137);
    drawLineDDA(120, 97, 160, 97);
    drawLineDDA(120, 57, 160, 57);
    
    // Right drawer unit
    glColor3f(0.6f, 0.6f, 0.6f);
    drawRect(600, 50, 120, 130);
    
    // Right drawer fronts
    glColor3f(0.55f, 0.55f, 0.55f);
    drawRect(605, 120, 110, 35);
    drawRect(605, 80, 110, 35);
    drawRect(605, 40, 110, 35);
    
    // Drawer handles
    drawLineDDA(640, 137, 680, 137);
    drawLineDDA(640, 97, 680, 97);
    drawLineDDA(640, 57, 160, 57);
    
    // Drawer unit wheels using Midpoint Circle
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(2);
    drawCircleMidpoint(100, 45, 8);
    drawCircleMidpoint(180, 45, 8);
    drawCircleMidpoint(620, 45, 8);
    drawCircleMidpoint(700, 45, 8);
    
    // Fill wheels
    glColor3f(0.15f, 0.15f, 0.15f);
    drawFilledCircle(100, 45, 7, 15);
    drawFilledCircle(180, 45, 7, 15);
    drawFilledCircle(620, 45, 7, 15);
    drawFilledCircle(700, 45, 7, 15);
}

// Draw the computer monitor
void drawComputer() {
    // Monitor stand
    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(270, 195, 60, 10);
    drawRect(290, 205, 20, 30);
    
    // Monitor frame (black with subtle shadow)
    glColor3f(0.1f, 0.1f, 0.1f);
    drawRect(208, 233, 184, 134);
    glColor3f(0.18f, 0.18f, 0.18f);
    drawRect(210, 235, 180, 130);
    
    // Monitor screen with animated gradient
    float wave = sin(screenWave) * 0.1f;
    glBegin(GL_QUADS);
        glColor3f(0.15f + wave, 0.5f + wave, 0.55f);
        glVertex2f(220, 245);
        glVertex2f(380, 245);
        glColor3f(0.25f, 0.75f + wave * 0.5f, 0.7f + wave * 0.3f);
        glVertex2f(380, 355);
        glVertex2f(220, 355);
    glEnd();
    
    // Animated scan lines
    glColor3f(0.35f, 0.85f, 0.8f);
    for (int i = 0; i < 4; i++) {
        float lineY = 250 + fmod(screenWave * 20 + i * 28, 100);
        drawLineDDA(222, lineY, 378, lineY);
    }
    
    // Power LED (pulsing green)
    float glow = 0.5f + 0.5f * sin(glowPhase * 2);
    glColor3f(0.1f, 0.4f + 0.5f * glow, 0.1f);
    drawFilledCircle(385, 240, 3, 10);
}

// Draw the keyboard
void drawKeyboard() {
    // Keyboard base
    glColor3f(0.25f, 0.25f, 0.25f);
    drawRect(230, 195, 140, 8);
    
    // Keyboard keys (small rectangles) using lines
    glColor3f(0.35f, 0.35f, 0.35f);
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 12; col++) {
            drawRect(235 + col * 11, 196 + row * 3, 9, 2);
        }
    }
}

// Draw the office chair
void drawChair() {
    // Chair base wheels using Midpoint Circle
    glColor3f(0.2f, 0.2f, 0.2f);
    glPointSize(2);
    drawCircleMidpoint(400, 35, 6);
    drawCircleMidpoint(370, 45, 6);
    drawCircleMidpoint(430, 45, 6);
    
    // Fill wheels
    glColor3f(0.15f, 0.15f, 0.15f);
    drawFilledCircle(400, 35, 5, 12);
    drawFilledCircle(370, 45, 5, 12);
    drawFilledCircle(430, 45, 5, 12);
    
    // Chair legs using Bresenham
    glColor3f(0.25f, 0.25f, 0.25f);
    glPointSize(2);
    drawLineBresenham(400, 35, 400, 90);
    drawLineBresenham(400, 45, 370, 45);
    drawLineBresenham(400, 45, 430, 45);
    
    // Chair seat
    glColor3f(0.15f, 0.15f, 0.15f);
    drawRect(360, 90, 80, 20);
    
    // Chair back
    glColor3f(0.12f, 0.12f, 0.12f);
    drawRect(365, 110, 70, 90);
    
    // Chair back curve (rounded top)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(400, 200);  // Center
    for (int i = 0; i <= 180; i += 10) {
        float angle = i * PI / 180;
        glVertex2f(400 + 35 * cos(angle), 200 + 15 * sin(angle));
    }
    glEnd();
}

// Draw the printer
void drawPrinter() {
    // Printer body
    glColor3f(0.85f, 0.85f, 0.85f);
    drawRect(580, 195, 100, 50);
    
    // Printer top
    glColor3f(0.75f, 0.75f, 0.75f);
    drawRect(580, 245, 100, 15);
    
    // Paper tray
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(590, 200, 80, 8);
    
    // Paper output slot
    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(595, 230, 70, 5);
    
    // Printer buttons using Midpoint Circle
    glColor3f(0.2f, 0.6f, 0.2f);
    drawFilledCircle(665, 255, 4, 12);
    glColor3f(0.6f, 0.2f, 0.2f);
    drawFilledCircle(650, 255, 4, 12);
}

// Contributor: Soroar – Desk books use DDA line accents and precise scaling
// Draw books stacked on desk
void drawBooks() {
    // Book 1 (pink/magenta)
    glColor3f(0.85f, 0.3f, 0.5f);
    drawRect(580, 260, 90, 15);
    
    // Book 2 (teal)
    glColor3f(0.2f, 0.6f, 0.6f);
    drawRect(585, 275, 85, 12);
    
    // Book 3 (yellow)
    glColor3f(0.9f, 0.85f, 0.3f);
    drawRect(583, 287, 87, 14);
    
    // Book spines using DDA
    glColor3f(0.7f, 0.2f, 0.4f);
    drawLineDDA(580, 260, 580, 275);
    glColor3f(0.15f, 0.5f, 0.5f);
    drawLineDDA(585, 275, 585, 287);
}

// Draw wall picture/painting
void drawPicture() {
    // Frame (brown)
    glColor3f(0.4f, 0.25f, 0.1f);
    drawRect(70, 320, 120, 100);
    
    // Picture background (sky)
    glColor3f(0.53f, 0.81f, 0.98f);
    drawRect(80, 330, 100, 80);
    
    // Mountains
    glColor3f(0.3f, 0.5f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(80, 360);
        glVertex2f(130, 400);
        glVertex2f(180, 360);
    glEnd();
    
    glColor3f(0.25f, 0.45f, 0.25f);
    glBegin(GL_TRIANGLES);
        glVertex2f(100, 360);
        glVertex2f(150, 390);
        glVertex2f(180, 360);
    glEnd();
    
    // Snow cap
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(120, 395);
        glVertex2f(130, 400);
        glVertex2f(140, 395);
    glEnd();
    
    // Sun in picture
    glColor3f(1.0f, 0.9f, 0.3f);
    drawFilledCircle(95, 395, 10, 15);
    
    // Ground
    glColor3f(0.2f, 0.6f, 0.2f);
    drawRect(80, 330, 100, 30);
}

// Draw bookshelf on wall
void drawBookshelf() {
    // Shelf bracket
    glColor3f(0.55f, 0.35f, 0.2f);
    drawRect(550, 380, 180, 8);
    
    // Books on shelf (using SCALING - different sizes)
    // Book 1 (red)
    glColor3f(0.8f, 0.2f, 0.2f);
    drawRect(560, 388, 25, 45);
    
    // Book 2 (blue)
    glColor3f(0.2f, 0.3f, 0.7f);
    drawRect(590, 388, 20, 40);
    
    // Book 3 (orange)
    glColor3f(0.9f, 0.5f, 0.1f);
    drawRect(615, 388, 22, 50);
    
    // Book 4 (green - lying flat)
    glColor3f(0.3f, 0.7f, 0.3f);
    drawRect(645, 388, 35, 12);
    
    // Book 5 (yellow - on top)
    glColor3f(0.9f, 0.9f, 0.3f);
    drawRect(648, 400, 30, 10);
    
    // Small decorative item (green box)
    glColor3f(0.4f, 0.75f, 0.4f);
    drawRect(700, 388, 25, 35);
    
    // Shelf support brackets using Bresenham
    glColor3f(0.4f, 0.25f, 0.15f);
    glPointSize(2);
    drawLineBresenham(560, 380, 560, 370);
    drawLineBresenham(560, 370, 575, 380);
    drawLineBresenham(710, 380, 710, 370);
    drawLineBresenham(710, 370, 725, 380);
}

// Helper to draw a single animated coffee-steam curl using a line strip
void drawSteamCurl(float baseX, float baseY, float height, float phase, float sway) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 24; ++i) {
        float t = i / 24.0f;
        float y = baseY + t * height;
        float taper = 1.0f - t * 0.35f; // fade lateral motion near the top
        float x = baseX + sin(phase + t * 3.14159f * 1.2f) * sway * taper;
        glVertex2f(x, y);
    }
    glEnd();
}

// Draw coffee cup on desk
void drawCoffeeCup() {
    // Cup body
    glColor3f(0.85f, 0.85f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(100, 195);
        glVertex2f(130, 195);
        glVertex2f(127, 235);
        glVertex2f(103, 235);
    glEnd();
    
    // Cup lid (brown)
    glColor3f(0.4f, 0.25f, 0.15f);
    drawRect(98, 235, 35, 8);
    
    // Cup sleeve
    glColor3f(0.5f, 0.35f, 0.2f);
    drawRect(102, 205, 27, 18);
    
    // Steam wisps (line strips for smoother curls)
    glColor3f(0.85f, 0.85f, 0.9f);
    float steamPhase = clockSecond * 0.05f;
    glLineWidth(2.0f);
    glEnable(GL_LINE_SMOOTH);
    drawSteamCurl(108.0f, 245.0f, 22.0f, steamPhase, 3.5f);
    drawSteamCurl(115.0f, 245.0f, 26.0f, steamPhase + 0.6f, 4.0f);
    drawSteamCurl(122.0f, 245.0f, 22.0f, steamPhase + 1.1f, 3.2f);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
}

// Draw a minimal desk organizer (replaces plant)
void drawDeskOrganizer() {
    // Base pad
    glColor3f(0.35f, 0.2f, 0.1f);
    drawRect(165, 195, 45, 6);
    glColor3f(0.8f, 0.75f, 0.65f);
    drawRect(168, 201, 39, 10);

    // Pen holder
    glColor3f(0.25f, 0.25f, 0.3f);
    drawRect(172, 211, 20, 18);
    glColor3f(0.18f, 0.18f, 0.22f);
    drawRect(174, 213, 16, 14);

    // Pens using Bresenham (diagonal) + DDA vertical
    glColor3f(0.8f, 0.15f, 0.15f);
    drawLineBresenham(178, 227, 180, 250);
    glColor3f(0.1f, 0.6f, 0.8f);
    drawLineBresenham(186, 227, 188, 252);
    glColor3f(0.95f, 0.85f, 0.2f);
    drawLineDDA(182, 227, 182, 247);
}


// Contributor: Farhan – Clock & pendulum highlight Midpoint Circle usage + rotation
// Draw wall clock with animated hands
void drawClock() {
    // Clock shadow
    glColor3f(0.7f, 0.5f, 0.35f);
    drawFilledCircle(733, 417, 34, 40);
    
    // Solid outer ring (dark brown wooden frame)
    glColor3f(0.35f, 0.22f, 0.12f);
    drawFilledCircle(730, 420, 36, 40);
    
    // Inner ring (lighter wood)
    glColor3f(0.55f, 0.38f, 0.22f);
    drawFilledCircle(730, 420, 32, 40);
    
    // Clock face (cream white)
    glColor3f(0.98f, 0.96f, 0.92f);
    drawFilledCircle(730, 420, 28, 40);
    
    // Subtle inner shadow on face
    glColor3f(0.92f, 0.90f, 0.86f);
    drawFilledCircle(731, 419, 26, 40);
    
    // Clock face center
    glColor3f(0.98f, 0.96f, 0.92f);
    drawFilledCircle(730, 420, 24, 40);
    
    // Clock hour markers (thicker at 12, 3, 6, 9)
    for (int i = 0; i < 12; i++) {
        float angle = i * 30 * PI / 180;
        float x1, y1, x2, y2;
        if (i % 3 == 0) {
            // Major markers
            glColor3f(0.15f, 0.15f, 0.15f);
            x1 = 730 + 20 * sin(angle);
            y1 = 420 + 20 * cos(angle);
            x2 = 730 + 26 * sin(angle);
            y2 = 420 + 26 * cos(angle);
            glPointSize(3);
        } else {
            // Minor markers
            glColor3f(0.3f, 0.3f, 0.3f);
            x1 = 730 + 22 * sin(angle);
            y1 = 420 + 22 * cos(angle);
            x2 = 730 + 26 * sin(angle);
            y2 = 420 + 26 * cos(angle);
            glPointSize(2);
        }
        drawLineDDA(x1, y1, x2, y2);
    }
    
    // Hour hand (thick, black)
    glColor3f(0.1f, 0.1f, 0.1f);
    float hourAngle = clockMinute * PI / 180;
    glBegin(GL_QUADS);
        glVertex2f(730 - 2 * cos(hourAngle), 420 + 2 * sin(hourAngle));
        glVertex2f(730 + 2 * cos(hourAngle), 420 - 2 * sin(hourAngle));
        glVertex2f(730 + 14 * sin(hourAngle) + 1 * cos(hourAngle), 420 + 14 * cos(hourAngle) - 1 * sin(hourAngle));
        glVertex2f(730 + 14 * sin(hourAngle) - 1 * cos(hourAngle), 420 + 14 * cos(hourAngle) + 1 * sin(hourAngle));
    glEnd();
    
    // Minute hand (thinner)
    glColor3f(0.15f, 0.15f, 0.15f);
    float minAngle = clockSecond * 0.5f * PI / 180;
    glBegin(GL_TRIANGLES);
        glVertex2f(730 - 1.5f * cos(minAngle), 420 + 1.5f * sin(minAngle));
        glVertex2f(730 + 1.5f * cos(minAngle), 420 - 1.5f * sin(minAngle));
        glVertex2f(730 + 20 * sin(minAngle), 420 + 20 * cos(minAngle));
    glEnd();
    
    // Second hand (red, thin, with counterweight)
    glColor3f(0.85f, 0.15f, 0.1f);
    float secAngle = clockSecond * PI / 180;
    glBegin(GL_LINES);
        glVertex2f(730 - 6 * sin(secAngle), 420 - 6 * cos(secAngle));
        glVertex2f(730 + 24 * sin(secAngle), 420 + 24 * cos(secAngle));
    glEnd();
    // Counterweight circle
    glColor3f(0.85f, 0.15f, 0.1f);
    drawFilledCircle(730 - 5 * sin(secAngle), 420 - 5 * cos(secAngle), 2, 10);
    
    // Center cap (gold/brass)
    glColor3f(0.85f, 0.7f, 0.3f);
    drawFilledCircle(730, 420, 4, 15);
    glColor3f(0.95f, 0.85f, 0.5f);
    drawFilledCircle(730, 420, 2, 12);
    
    // Pendulum below clock
    glColor3f(0.3f, 0.2f, 0.1f);
    float pendX = 730 + 15 * sin(pendulumAngle * PI / 180);
    float pendY = 375;
    glBegin(GL_LINES);
        glVertex2f(730, 384);
        glVertex2f(pendX, pendY);
    glEnd();
    // Pendulum bob (gold)
    glColor3f(0.85f, 0.7f, 0.3f);
    drawFilledCircle(pendX, pendY - 5, 8, 20);
    glColor3f(0.95f, 0.85f, 0.5f);
    drawFilledCircle(pendX, pendY - 5, 5, 15);
}

// Contributor: Zisan – Ceiling fan blades via GL_QUADS + translate/rotate animation
// Draw ceiling fan (with rotation animation)
void drawCeilingFan() {
    // Fan mount
    glColor3f(0.4f, 0.4f, 0.4f);
    drawRect(195, 480, 20, 20);
    
    // Fan motor housing
    glColor3f(0.3f, 0.3f, 0.3f);
    drawFilledCircle(205, 475, 15, 20);
    
    // Fan blades with ROTATION transformation
    glPushMatrix();
    glTranslatef(205, 475, 0);
    glRotatef(fanAngle, 0, 0, 1);
    
    glColor3f(0.35f, 0.25f, 0.15f);
    
    // 4 blades
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 90, 0, 0, 1);
        glBegin(GL_QUADS);
            glVertex2f(-5, 0);
            glVertex2f(5, 0);
            glVertex2f(8, 50);
            glVertex2f(-8, 50);
        glEnd();
        glPopMatrix();
    }
    
    glPopMatrix();
    
    // Center cap
    glColor3f(0.5f, 0.5f, 0.5f);
    drawFilledCircle(205, 475, 8, 15);
}

// ==================== MAIN DISPLAY FUNCTION ====================

// Draw floating dust particles in sunlight
void drawParticles() {
    for (int i = 0; i < 5; i++) {
        float brightness = 0.7f + 0.3f * sin(glowPhase + i);
        glColor3f(1.0f * brightness, 0.95f * brightness, 0.8f * brightness);
        float px = particleX[i] + sin(particleY[i] * 0.05f + i) * 10;
        drawFilledCircle(px, particleY[i], 2, 8);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw all room elements (back to front for proper layering)
    drawRoom();
    drawParticles();      // Floating dust in light
    drawCeilingFan();
    drawLamp();
    drawBookshelf();
    drawClock();
    drawSmartPanel();     // Smart home panel
    drawDesk();
    drawComputer();
    drawKeyboard();
    drawBooks();
    drawPrinter();
    drawDeskOrganizer();
    drawCoffeeCup();
    drawChair();
    
    glutSwapBuffers();
}

// ==================== ANIMATION UPDATE ====================

void update(int value) {
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    if (deltaTime <= 0 || deltaTime > 0.1f) deltaTime = 0.016f;
    
    // Lamp swinging animation (Translation + Rotation)
    lampAngle += lampDirection * 18.0f * deltaTime;
    if (lampAngle > 8) {
        lampAngle = 8;
        lampDirection = -1;
    } else if (lampAngle < -8) {
        lampAngle = -8;
        lampDirection = 1;
    }
    
    // Ceiling fan rotation
    fanAngle += 240.0f * deltaTime;
    if (fanAngle > 360) fanAngle -= 360;
    
    // Clock animation - SMOOTH continuous sweep motion like real clock
    // Second hand: 6 degrees per real second (360 degrees / 60 seconds)
    clockSecond += 6.0f * deltaTime;
    if (clockSecond >= 360.0f) clockSecond -= 360.0f;
    // Minute hand: 0.1 degrees per real second (6 degrees per minute)
    clockMinute += 0.1f * deltaTime;
    if (clockMinute >= 360.0f) clockMinute -= 360.0f;
    
    // Pendulum swing animation
    pendulumAngle += pendulumDir * 45.0f * deltaTime;
    if (pendulumAngle > 15) {
        pendulumAngle = 15;
        pendulumDir = -1;
    } else if (pendulumAngle < -15) {
        pendulumAngle = -15;
        pendulumDir = 1;
    }
    
    // Glow and screen wave effects
    glowPhase += deltaTime * 3.0f;
    if (glowPhase > 100) glowPhase -= 100;
    screenWave += deltaTime * 2.5f;
    if (screenWave > 100) screenWave -= 100;
    
    // Floating dust particles
    for (int i = 0; i < 5; i++) {
        particleY[i] += deltaTime * (15 + i * 5);
        if (particleY[i] > 400) particleY[i] = 120;
    }
    
    // Smart panel effects
    smartPanelGlow += deltaTime * 4.0f;
    for (int i = 0; i < 5; i++) {
        musicBar[i] = 0.3f + 0.7f * fabs(sin(glowPhase * 3 + i * 1.2f));
    }

    glutPostRedisplay();
    glutTimerFunc(8, update, 0);  
}



void init() {
    glClearColor(0.15f, 0.12f, 0.1f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 500);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



// ==================== MAIN FUNCTION ====================

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Interior Design - Home Office (OpenGL Project)");
    
    init();
    
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
  
    printf("   MODERN SMART HOME OFFICE\n");
   
    glutMainLoop();
    
    return 0;
}

