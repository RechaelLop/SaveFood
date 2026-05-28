// ===== SCREEN DIMENSIONS =====
const int screenWidth = 1280;
const int screenHeight = 720;

#include <windows.h>
#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>

// ===== ANIMATION VARIABLES =====
float crowX = -150.0f;
float catX = -180.0f;
float studentX = 200.0f;
float bannerAlpha = 0.0f;
float statsAlpha = 0.0f;
float fadeInAlpha = 1.0f;

// Scene control
int stage = 0;
int statLine = 0;
int frameCounter = 0;
float animationTimer = 0.0f;

// ===== POLISHED COLORS =====
struct Color {
    float r, g, b;
};

Color SKY_BLUE = {0.53f, 0.81f, 0.98f};
Color WARM_YELLOW = {1.0f, 0.85f, 0.5f};
Color DARK_RED = {0.8f, 0.1f, 0.1f};
Color SOFT_BROWN = {0.6f, 0.4f, 0.2f};
Color LIGHT_BROWN = {0.8f, 0.7f, 0.5f};
Color DARK_GREEN = {0.1f, 0.5f, 0.1f};
Color OFF_WHITE = {0.95f, 0.95f, 0.9f};
Color SHADOW_GRAY = {0.3f, 0.3f, 0.3f};

// ===== UTILITY FUNCTIONS =====
void drawRoundedRect(float x, float y, float w, float h, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * M_PI / 180.0f;
        float px = x, py = y;
        
        if (rad < M_PI/2) { px += w - radius; py += h - radius; }
        else if (rad < M_PI) { px += radius; py += h - radius; }
        else if (rad < 3*M_PI/2) { px += radius; py += radius; }
        else { px += w - radius; py += radius; }
        
        px += radius * cos(rad);
        py += radius * sin(rad);
        glVertex2f(px, py);
    }
    glEnd();
}

void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();
}

void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18, float r=1.0f, float g=1.0f, float b=1.0f) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++)
        glutBitmapCharacter(font, text[i]);
}

void drawShadow(float x, float y, float w, float h) {
    glColor4f(0, 0, 0, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x + 5, y - 5);
    glVertex2f(x + w + 5, y - 5);
    glVertex2f(x + w + 5, y + h - 5);
    glVertex2f(x + 5, y + h - 5);
    glEnd();
}

// ===== BACKGROUND =====
void drawBackground() {
    // Gradient sky
    glBegin(GL_QUADS);
    glColor3f(0.53f, 0.81f, 0.98f);
    glVertex2f(0, 400);
    glColor3f(0.85f, 0.92f, 1.0f);
    glVertex2f(screenWidth, 400);
    glColor3f(0.95f, 0.97f, 1.0f);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Wall with texture
    glColor3f(0.88f, 0.85f, 0.78f);
    glBegin(GL_QUADS);
    glVertex2f(0, 350);
    glVertex2f(screenWidth, 350);
    glVertex2f(screenWidth, 480);
    glVertex2f(0, 480);
    glEnd();
    
    // Floor
    glColor3f(0.68f, 0.63f, 0.58f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, 350);
    glVertex2f(0, 350);
    glEnd();
    
    // Floor tiles
    glColor3f(0.6f, 0.55f, 0.5f);
    for (int i = 0; i < 20; i++) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(i * 64, 0);
        glVertex2f((i+1) * 64, 0);
        glVertex2f((i+1) * 64, 350);
        glVertex2f(i * 64, 350);
        glEnd();
    }
    
    // Window
    glColor3f(0.7f, 0.85f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(50, 500);
    glVertex2f(250, 500);
    glVertex2f(250, 650);
    glVertex2f(50, 650);
    glEnd();
    
    glColor3f(0.5f, 0.4f, 0.3f);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(150, 500);
    glVertex2f(150, 650);
    glVertex2f(50, 575);
    glVertex2f(250, 575);
    glEnd();
    
    // Sun in window
    glColor3f(1.0f, 0.9f, 0.4f);
    drawCircle(100, 610, 25, 30);
}

// ===== POLISHED TABLE WITH FOOD =====
void drawTableAndFood() {
    // Shadow under table
    drawShadow(390, 180, 500, 80);
    
    // Table top
    glColor3f(SOFT_BROWN.r, SOFT_BROWN.g, SOFT_BROWN.b);
    glBegin(GL_QUADS);
    glVertex2f(390, 250);
    glVertex2f(890, 250);
    glVertex2f(890, 330);
    glVertex2f(390, 330);
    glEnd();
    
    // Table edge highlight
    glColor3f(0.7f, 0.5f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(390, 325);
    glVertex2f(890, 325);
    glVertex2f(890, 330);
    glVertex2f(390, 330);
    glEnd();
    
    // Table legs
    glColor3f(0.4f, 0.25f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(420, 180);
    glVertex2f(450, 180);
    glVertex2f(450, 250);
    glVertex2f(420, 250);
    glVertex2f(830, 180);
    glVertex2f(860, 180);
    glVertex2f(860, 250);
    glVertex2f(830, 250);
    glEnd();
    
    // Plate with shadow
    glColor4f(0, 0, 0, 0.15f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(642 + 72 * cos(rad), 273 + 37 * sin(rad));
    }
    glEnd();
    
    // Plate
    glColor3f(0.95f, 0.92f, 0.88f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(640 + 70 * cos(rad), 275 + 35 * sin(rad));
    }
    glEnd();
    
    // Plate rim
    glColor3f(0.8f, 0.75f, 0.7f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(640 + 65 * cos(rad), 275 + 33 * sin(rad));
    }
    glEnd();
    
    // Rice grains
    glColor3f(1.0f, 1.0f, 0.95f);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (int i = 0; i < 80; i++) {
        glVertex2f(610 + (i % 16) * 3.5, 270 + (i / 16) * 4);
    }
    glEnd();
    
    // Half-eaten chapati
    glColor3f(0.55f, 0.35f, 0.2f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 180; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(655 + 38 * cos(rad), 280 + 20 * sin(rad));
    }
    glEnd();
    
    // Chapati texture
    glColor3f(0.45f, 0.28f, 0.15f);
    for (int i = 0; i < 12; i++) {
        glBegin(GL_LINES);
        glVertex2f(655 + i * 3, 280);
        glVertex2f(655 + i * 3 + 15, 295);
        glEnd();
    }
    
    // "WASTED" text
    drawText(620, 270, "WASTED", GLUT_BITMAP_HELVETICA_12, 0.5f, 0.2f, 0.0f);
}

// ===== DETAILED STUDENT =====
void drawStudent() {
    // Shadow
    glColor4f(0, 0, 0, 0.2f);
    drawCircle(studentX, 165, 28, 15);
    
    // Legs with shoes
    glColor3f(0.2f, 0.2f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(studentX - 15, 170);
    glVertex2f(studentX - 5, 170);
    glVertex2f(studentX - 5, 200);
    glVertex2f(studentX - 15, 200);
    glVertex2f(studentX + 5, 170);
    glVertex2f(studentX + 15, 170);
    glVertex2f(studentX + 15, 200);
    glVertex2f(studentX + 5, 200);
    glEnd();
    
    // Shoes
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(studentX - 10, 168, 10, 12);
    drawCircle(studentX + 10, 168, 10, 12);
    
    // Body
    glColor3f(0.2f, 0.5f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(studentX - 22, 200);
    glVertex2f(studentX + 22, 200);
    glVertex2f(studentX + 22, 260);
    glVertex2f(studentX - 22, 260);
    glEnd();
    
    // Collar
    glColor3f(0.3f, 0.6f, 0.9f);
    glBegin(GL_TRIANGLES);
    glVertex2f(studentX - 15, 260);
    glVertex2f(studentX, 275);
    glVertex2f(studentX + 15, 260);
    glEnd();
    
    // Head
    glColor3f(1.0f, 0.85f, 0.7f);
    drawCircle(studentX, 278, 22, 25);
    
    // Hair
    glColor3f(0.2f, 0.15f, 0.1f);
    for (int i = -18; i <= 18; i+=6) {
        drawCircle(studentX + i, 295, 8, 10);
    }
    
    // Eyes
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(studentX - 8, 282, 3, 10);
    drawCircle(studentX + 8, 282, 3, 10);
    
    // Sad mouth
    glBegin(GL_LINE_STRIP);
    glVertex2f(studentX - 8, 272);
    glVertex2f(studentX, 268);
    glVertex2f(studentX + 8, 272);
    glEnd();
    
    // Arms with motion
    static float armAngle = 0;
    armAngle += 0.1f;
    
    glLineWidth(8);
    glColor3f(0.2f, 0.5f, 0.8f);
    glBegin(GL_LINES);
    glVertex2f(studentX - 20, 240);
    glVertex2f(studentX - 45 + sin(armAngle) * 8, 255);
    glVertex2f(studentX + 20, 240);
    glVertex2f(studentX + 45 - sin(armAngle) * 8, 255);
    glEnd();
    
    // Backpack
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(studentX + 20, 215);
    glVertex2f(studentX + 40, 215);
    glVertex2f(studentX + 40, 255);
    glVertex2f(studentX + 20, 255);
    glEnd();
}

// ===== DETAILED CROW =====
void drawCrow(float x, float y, bool bowing = false) {
    float headAngle = bowing ? -20 : 0;
    float bodyY = y;
    float headX = x + 35;
    float headY = bowing ? y + 12 : y + 28;
    
    // Shadow
    glColor4f(0, 0, 0, 0.15f);
    drawCircle(x - 10, y - 25, 35, 15);
    
    // Body
    glColor3f(0.08f, 0.08f, 0.1f);
    drawCircle(x, bodyY, 30, 25);
    
    // Wing
    glColor3f(0.15f, 0.15f, 0.18f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 20, bodyY);
    glVertex2f(x + 5, bodyY + 25);
    glVertex2f(x - 5, bodyY - 15);
    glEnd();
    
    // Wing detail
    glColor3f(0.2f, 0.2f, 0.25f);
    glBegin(GL_LINES);
    glVertex2f(x - 15, bodyY + 5);
    glVertex2f(x, bodyY - 5);
    glVertex2f(x - 10, bodyY + 10);
    glVertex2f(x + 5, bodyY - 2);
    glEnd();
    
    // Tail
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 28, bodyY - 5);
    glVertex2f(x - 45, bodyY - 15);
    glVertex2f(x - 40, bodyY + 5);
    glEnd();
    
    // Head
    glColor3f(0.08f, 0.08f, 0.1f);
    drawCircle(headX, headY, 22, 25);
    
    // Beak
    glColor3f(1.0f, 0.85f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(headX + 18, headY);
    glVertex2f(headX + 40, headY + (bowing ? -5 : 3));
    glVertex2f(headX + 32, headY - 8);
    glEnd();
    
    // Eye
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(headX + 12, headY + 5, 5, 15);
    glColor3f(0, 0, 0);
    drawCircle(headX + 13, headY + 5, 3, 15);
    
    // Eye highlight
    glColor3f(1, 1, 1);
    drawCircle(headX + 11.5f, headY + 6, 1.5f, 8);
    
    // Legs
    glColor3f(0.5f, 0.4f, 0.2f);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(x - 5, bodyY - 28);
    glVertex2f(x - 8, bodyY - 45);
    glVertex2f(x + 8, bodyY - 28);
    glVertex2f(x + 5, bodyY - 45);
    glEnd();
}

// ===== DETAILED CAT =====
void drawCat(float x, float y) {
    // Shadow
    glColor4f(0, 0, 0, 0.15f);
    drawCircle(x - 10, y - 30, 35, 15);
    
    // Body
    glColor3f(1.0f, 0.65f, 0.25f);
    drawCircle(x, y, 32, 25);
    
    // Stripes
    glColor3f(0.8f, 0.5f, 0.2f);
    for (int i = -15; i <= 15; i+=10) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(x + i - 5, y - 10);
        glVertex2f(x + i + 5, y);
        glVertex2f(x + i - 3, y + 10);
        glEnd();
    }
    
    // Head
    glColor3f(1.0f, 0.65f, 0.25f);
    drawCircle(x + 38, y + 22, 24, 25);
    
    // Ears
    glColor3f(1.0f, 0.65f, 0.25f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 26, y + 42);
    glVertex2f(x + 34, y + 62);
    glVertex2f(x + 40, y + 42);
    glVertex2f(x + 40, y + 42);
    glVertex2f(x + 52, y + 62);
    glVertex2f(x + 56, y + 42);
    glEnd();
    
    // Inner ears
    glColor3f(1.0f, 0.85f, 0.7f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 30, y + 44);
    glVertex2f(x + 34, y + 58);
    glVertex2f(x + 38, y + 44);
    glVertex2f(x + 42, y + 44);
    glVertex2f(x + 50, y + 58);
    glVertex2f(x + 52, y + 44);
    glEnd();
    
    // Eyes
    glColor3f(0.3f, 0.6f, 0.2f);
    drawCircle(x + 31, y + 28, 5, 15);
    drawCircle(x + 48, y + 28, 5, 15);
    glColor3f(0, 0, 0);
    drawCircle(x + 32, y + 28, 2.5f, 15);
    drawCircle(x + 49, y + 28, 2.5f, 15);
    
    // Nose
    glColor3f(1.0f, 0.5f, 0.5f);
    drawCircle(x + 40, y + 22, 3, 10);
    
    // Whiskers
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glVertex2f(x + 30, y + 20);
    glVertex2f(x + 15, y + 22);
    glVertex2f(x + 30, y + 18);
    glVertex2f(x + 15, y + 16);
    glVertex2f(x + 50, y + 20);
    glVertex2f(x + 65, y + 22);
    glVertex2f(x + 50, y + 18);
    glVertex2f(x + 65, y + 16);
    glEnd();
    
    // Tail
    glLineWidth(8);
    glColor3f(1.0f, 0.65f, 0.25f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 32, y - 5);
    glVertex2f(x - 50, y + 12);
    glVertex2f(x - 58, y + 5);
    glVertex2f(x - 62, y - 8);
    glEnd();
    
    // Tail tip
    glColor3f(0.7f, 0.5f, 0.2f);
    drawCircle(x - 62, y - 8, 6, 10);
    
    // Legs
    glColor3f(1.0f, 0.65f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(x - 12, y - 28);
    glVertex2f(x - 2, y - 28);
    glVertex2f(x - 2, y - 12);
    glVertex2f(x - 12, y - 12);
    glVertex2f(x + 8, y - 28);
    glVertex2f(x + 18, y - 28);
    glVertex2f(x + 18, y - 12);
    glVertex2f(x + 8, y - 12);
    glEnd();
}

// ===== ELEGANT BANNER =====
void drawBanner() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Banner shadow
    glColor4f(0, 0, 0, 0.3f * bannerAlpha);
    glBegin(GL_QUADS);
    glVertex2f(245, 475);
    glVertex2f(1045, 475);
    glVertex2f(1045, 575);
    glVertex2f(245, 575);
    glEnd();
    
    // Banner background
    glColor4f(DARK_RED.r, DARK_RED.g, DARK_RED.b, bannerAlpha);
    glBegin(GL_QUADS);
    glVertex2f(240, 470);
    glVertex2f(1040, 470);
    glVertex2f(1040, 570);
    glVertex2f(240, 570);
    glEnd();
    
    // Banner border
    glColor4f(1.0f, 0.95f, 0.7f, bannerAlpha);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(240, 470);
    glVertex2f(1040, 470);
    glVertex2f(1040, 570);
    glVertex2f(240, 570);
    glEnd();
    
    // Message
    glColor4f(1.0f, 1.0f, 1.0f, bannerAlpha);
    drawText(420, 520, "ANIMALS RESPECT FOOD", GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
    drawText(490, 490, "Why don't we?", GLUT_BITMAP_TIMES_ROMAN_24, 1, 1, 1);
    
    glDisable(GL_BLEND);
}

// ===== IMPACTFUL STATISTICS SCREEN =====
void drawStatisticsScreen() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Dark overlay with fade-in
    glColor4f(0.05f, 0.05f, 0.1f, 0.95f * statsAlpha);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight);
    glVertex2f(0, screenHeight);
    glEnd();
    
    // Decorative line
    glColor4f(0.9f, 0.3f, 0.2f, 0.7f * statsAlpha);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(200, 670);
    glVertex2f(1080, 670);
    glEnd();
    
    // Title
    drawText(380, 640, "✦ THE GLOBAL FOOD CRISIS ✦", GLUT_BITMAP_TIMES_ROMAN_24, 0.95f, 0.85f, 0.5f);
    
    // Statistics with icons
    struct StatItem {
        const char* text;
        const char* icon;
    };
    
    StatItem stats[] = {
        {"1.3 BILLION tons of food wasted globally EVERY YEAR", "🗑️"},
        {"828 MILLION people go to bed hungry every night", "😔"},
        {"9 MILLION people die from hunger annually", "💔"},
        {"That's 1 death every 3.5 seconds - mostly children under 5", "⚠️"},
        {"Food wasted in wealthy nations could feed the hungry 3x over", "💚"}
    };
    
    float yStart = 540;
    for (int i = 0; i <= statLine && i < 5; i++) {
        float alpha = statsAlpha * (i <= statLine ? 1.0f : 0.5f);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        
        // Icon
        drawText(140, yStart - i * 58, stats[i].icon, GLUT_BITMAP_HELVETICA_18, 1, 1, 1);
        // Text
        drawText(180, yStart - i * 58, stats[i].text, GLUT_BITMAP_HELVETICA_18, 1, 1, 1);
        
        // Progress bar for each stat
        float progress = (statLine >= i) ? 1.0f : 0.0f;
        glColor4f(0.3f, 0.7f, 0.3f, 0.4f * alpha);
        glBegin(GL_QUADS);
        glVertex2f(180, yStart - i * 58 - 8);
        glVertex2f(180 + 900 * progress, yStart - i * 58 - 8);
        glVertex2f(180 + 900 * progress, yStart - i * 58 - 4);
        glVertex2f(180, yStart - i * 58 - 4);
        glEnd();
    }
    
    // Final quote with animation
    if (statLine >= 5 && statsAlpha > 0.95f) {
        float quoteAlpha = statsAlpha * sin(animationTimer * 2) * 0.3f + statsAlpha * 0.7f;
        
        glColor4f(1.0f, 0.85f, 0.4f, quoteAlpha);
        drawText(260, 250, "╔══════════════════════════════════════════════════════════════╗", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        drawText(260, 225, "║  \"When we waste food, we waste the labor, water, energy,    ║", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        drawText(260, 200, "║   and love that went into producing it.                    ║", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        drawText(260, 175, "║                                                          ║", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        drawText(260, 150, "║  SAVE FOOD, SAVE LIVES!\"                                 ║", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        drawText(260, 125, "╚══════════════════════════════════════════════════════════════╝", GLUT_BITMAP_HELVETICA_18, 1, 0.85f, 0.4f);
        
        // Call to action
        glColor4f(0.5f, 0.9f, 0.5f, quoteAlpha);
        drawText(420, 80, "🌍 Be the change. Take only what you can eat. 🌍", GLUT_BITMAP_HELVETICA_18, 0.5f, 0.9f, 0.5f);
        
        // Sources
        glColor4f(0.6f, 0.6f, 0.6f, statsAlpha * 0.7f);
        drawText(860, 30, "Sources: UN FAO | World Food Programme | WHO", GLUT_BITMAP_HELVETICA_10, 0.6f, 0.6f, 0.6f);
    }
    
    glDisable(GL_BLEND);
}

// ===== MAIN DISPLAY =====
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (stage == 3) {
        drawStatisticsScreen();
    } else {
        drawBackground();
        drawTableAndFood();
        
        if (stage == 0) {
            studentX -= 1.8f;
            if (studentX < -150) stage = 1;
            drawStudent();
            drawCrow(80, 230);
            drawCat(50, 210);
            drawText(480, 690, "📱 Student leaves food on table and walks away...", GLUT_BITMAP_HELVETICA_18, 0.3f, 0.3f, 0.3f);
            drawText(560, 670, "❤️ Stage 0: Human Carelessness", GLUT_BITMAP_HELVETICA_12, 0.5f, 0.3f, 0.3f);
            
        } else if (stage == 1) {
            if (crowX < 560) crowX += 2.2f;
            if (catX < 530) catX += 2.4f;
            
            bool crowBowing = (crowX >= 555);
            drawCrow(crowX, 240, crowBowing);
            drawCat(catX, 220);
            
            if (crowX >= 555 && catX >= 525) {
                stage = 2;
            }
            drawText(480, 690, "🐦 Crow and cat arrive - they eat respectfully...", GLUT_BITMAP_HELVETICA_18, 0.3f, 0.3f, 0.3f);
            drawText(580, 670, "🙏 Stage 1: Animals Show Respect", GLUT_BITMAP_HELVETICA_12, 0.3f, 0.5f, 0.3f);
            
        } else if (stage == 2) {
            drawCrow(570, 255, true);
            drawCat(540, 235);
            
            if (bannerAlpha < 0.98f) {
                bannerAlpha += 0.01f;
            } else {
                frameCounter++;
                if (frameCounter > 180) {
                    stage = 3;
                    frameCounter = 0;
                }
            }
            drawBanner();
            drawText(440, 690, "📢 The crow bows before eating - showing respect for food", GLUT_BITMAP_HELVETICA_18, 0.3f, 0.3f, 0.3f);
            drawText(550, 670, "⭐ Stage 2: A Lesson from Nature", GLUT_BITMAP_HELVETICA_12, 0.5f, 0.5f, 0.3f);
        }
        
        // Title header
        glColor3f(0.2f, 0.3f, 0.5f);
        drawText(540, 715, "🍽️ SAVE FOOD PROJECT 🍽️", GLUT_BITMAP_TIMES_ROMAN_24, 0.2f, 0.3f, 0.5f);
    }
    
    glutSwapBuffers();
}

// ===== TIMER FUNCTIONS =====
void statsTimer(int value) {
    if (stage == 3 && statLine < 5) {
        statLine++;
        glutPostRedisplay();
        glutTimerFunc(800, statsTimer, 0);
    }
}

void fadeTimer(int value) {
    if (stage == 3 && statsAlpha < 0.98f) {
        statsAlpha += 0.02f;
        glutPostRedisplay();
        glutTimerFunc(40, fadeTimer, 0);
    }
}

void updateAnimation(int value) {
    animationTimer += 0.05f;
    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void init() {
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Save Food - A Lesson from Nature");
    
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, updateAnimation, 0);
    glutTimerFunc(1000, statsTimer, 0);
    glutTimerFunc(100, fadeTimer, 0);
    
    glutMainLoop();
    return 0;
}