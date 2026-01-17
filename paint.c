#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <math.h>
#include <string.h>

#define WIDTH 1120
#define HEIGHT 840
#define FPS 160
#define RADIUS 20
#define MAX_CIRCLES 50000
#define MAX_COLORS MAX_CIRCLES
#define NUMBER_COLORS 7
// #define INITIAL_SIZE 20

#define POS_X_RECT 15
#define POS_Y_RECT 15
#define WIDTH_RECT 40
#define HEIGHT_RECT 40

//	int size = 0;
//	int capacity = INITIAL_SIZE;


/*
void handleMemoryAlloc(int initialSize) {
	Vector2* pmousePositions = mousePositions;
	pmousePositions = (Vector2*) malloc(capacity * sizeof(Vector2));
	if (pmousePositions == NULL) exit(0);
	
	for (size = 0; size < initialSize; size++) {
		if (size >= capacity - 1) {
			capacity *= 2; // 40 de capacidade quando i == 20 e size == 19
			
			Vector2* temp = (Vector2*) realloc(pmousePositions, capacity * sizeof(pmousePositions)); // realoca memoria para capacidade 40
			if (temp == NULL) exit(0);
			
			pmousePositions = temp;
			printf("[DEBUG: The capacity has increased to: %d]\n", capacity);
			}
			pmousePositions[i] = mousePositions[i];
			}
			}
			*/
		
Vector2 mousePositions[MAX_CIRCLES];

void createColorButtons(Color* colors) {
	int j = 0;
	for (j = 0; j < NUMBER_COLORS; j++) {
		DrawRectangle(POS_X_RECT + 40*j, POS_Y_RECT, WIDTH_RECT, HEIGHT_RECT, colors[j]);
	}
	DrawRectangleLines(POS_X_RECT + 40*j - 40, POS_Y_RECT, WIDTH_RECT, HEIGHT_RECT, WHITE);
}

Color color[MAX_COLORS];
Color actualColor;
int canDraw = 1;

Color getColor(Vector2 mousePos, Color* colors) {
	if (mousePos.y >= POS_Y_RECT && mousePos.y <= POS_Y_RECT + HEIGHT_RECT) {
		for (int k = 0; k < NUMBER_COLORS; k++) {
			if (mousePos.x >= POS_X_RECT + 40*k && mousePos.x <= POS_X_RECT + 40*k + 40) {
				if (IsMouseButtonPressed(0)) {
					actualColor = colors[k];
				}
				canDraw = 0;
			}
		}
	}
	return actualColor;
}

int i = 0;

void drawCircleOnMouse(float radius, Vector2* mousePositions, Color* colors) {
	Vector2 mousePos = GetMousePosition();
	if (IsMouseButtonDown(0)) {
		// handleMemoryAlloc(i);
		canDraw = 1;
		color[i] = getColor(mousePos, colors);
		for (int j = 0; j < i; j++) {
			if (mousePos.x == mousePositions[j].x && mousePos.y == mousePositions[j].y) {
				canDraw = 0;
			}
		}
		if (canDraw) {
			mousePositions[i].x = mousePos.x;
			mousePositions[i].y = mousePos.y;
			i++;
		}
	};

	for (int j = 0; j < i; j++) {
		DrawCircle(mousePositions[j].x, mousePositions[j].y, radius, color[j]);
	}

}

int main() {
	Color colors[NUMBER_COLORS] = {WHITE, RED, GREEN, BLUE, PURPLE, ORANGE, BLACK};
	InitWindow(WIDTH, HEIGHT, "Paint v1.0");
	SetTargetFPS(FPS);
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			drawCircleOnMouse(RADIUS, mousePositions, colors);
			createColorButtons(colors);
			DrawText("Paint v1.0", WIDTH/2 - 70, 15, 30, WHITE);
			DrawFPS(WIDTH - 100, 5);
		EndDrawing();
		printf("%d\n", i);
	}

	CloseWindow();

	return 0;
}