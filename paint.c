#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <string.h>

#define WIDTH 1280
#define HEIGHT 720
#define FPS 160
#define MAX_CIRCLES 50000
#define MAX_COLORS MAX_CIRCLES
#define MAX_THICKNESS MAX_CIRCLES
#define NUMBER_COLORS 7
#define NUMBER_THICKNESSES 3

#define SMALL 5
#define MEDIUM 10
#define LARGE 20

#define POS_X_COLOR_RECT 20
#define POS_Y_COLOR_RECT 20
#define POS_X_THICKNESS_RECT (WIDTH-60)
#define POS_Y_THICKNESS_RECT POS_Y_COLOR_RECT
#define WIDTH_RECT 40
#define HEIGHT_RECT 40
// #define INITIAL_SIZE 20

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

typedef struct Circle{
	Color color;
	int thickness;
	Vector2 positions;
} Circle;
		
// Vector2 mousePositions[MAX_CIRCLES];

void createColorButtons(Color* colors) {
	int j = 0;
	for (j = 0; j < NUMBER_COLORS; j++) {
		DrawRectangle(POS_X_COLOR_RECT + WIDTH_RECT*j, POS_Y_COLOR_RECT, WIDTH_RECT, HEIGHT_RECT, colors[j]);
	}
	DrawRectangleLines(POS_X_COLOR_RECT + WIDTH_RECT*j - WIDTH_RECT, POS_Y_COLOR_RECT, WIDTH_RECT, HEIGHT_RECT, WHITE);
}

// Color colorArray[MAX_COLORS];
Color actualColor = {255, 255, 255, 255}; // WHITE
int canDraw = 1;

Color getColor(Vector2 mousePos, Color* colors) {
	int k = 0;
	if (mousePos.y >= POS_Y_COLOR_RECT && mousePos.y <= POS_Y_COLOR_RECT + HEIGHT_RECT) {
		for (k = 0; k < NUMBER_COLORS; k++) {
			if (mousePos.x >= POS_X_COLOR_RECT + WIDTH_RECT*k && mousePos.x <= POS_X_COLOR_RECT + WIDTH_RECT*k + WIDTH_RECT) {
				ShowCursor();
				if (IsMouseButtonPressed(0)) {
					actualColor = colors[k];
				}
				canDraw = 0;
			}
		}
	}
	return actualColor;
}

// int thicknessArray[MAX_THICKNESS];
int actualThickness = SMALL;


void createThicknessButtons(int* thickness) {
	for (int j = 0; j < NUMBER_THICKNESSES; j++) {
		DrawRectangle(POS_X_THICKNESS_RECT - WIDTH_RECT*j, POS_Y_THICKNESS_RECT, -WIDTH_RECT, HEIGHT_RECT, BLACK);
		DrawRectangleLines(POS_X_THICKNESS_RECT - WIDTH_RECT*j, POS_Y_THICKNESS_RECT, -WIDTH_RECT, HEIGHT_RECT, WHITE);
		DrawCircle((POS_X_THICKNESS_RECT - WIDTH_RECT*j - WIDTH_RECT) + WIDTH_RECT / 2, (POS_Y_THICKNESS_RECT) + HEIGHT_RECT / 2, thickness[j], WHITE);
	}
}

int getThickness(Vector2 mousePos, int* thickness) {
	if (mousePos.y >= POS_Y_THICKNESS_RECT && mousePos.y <= POS_Y_THICKNESS_RECT + HEIGHT_RECT) {
		for (int j = 0; j < NUMBER_THICKNESSES; j++) {
			if ((mousePos.x >= POS_X_THICKNESS_RECT - WIDTH_RECT*j - WIDTH_RECT) && (mousePos.x <= POS_X_THICKNESS_RECT - WIDTH_RECT*j)) {
				ShowCursor();
				if (IsMouseButtonPressed(0)) {
					actualThickness = thickness[j];
				}
				canDraw = 0;
			}
		}
	}
	return actualThickness;
}

int i = 0;

//Clear background
Vector2 buttonPos = {(WIDTH-50)/2, HEIGHT - 50};

void createClearBackgroundButton(Circle *circlesArray, int size){
	Vector2 mousePos = GetMousePosition();
	if(mousePos.x < (buttonPos.x + 50) && mousePos.y < (buttonPos.y + 50) &&
	mousePos.x > (buttonPos.x) && mousePos.y > (buttonPos.y) 
	){
		DrawRectangle(buttonPos.x, buttonPos.y , 50,30, RED);
		DrawText("CLEAR", buttonPos.x, buttonPos.y, 10, WHITE);
		if(IsMouseButtonPressed(0)){
			memset(circlesArray, 0, size*sizeof(Circle)); //para dar clear na memoria por meio da lib string.h
			i = 0;
		}
	}else{
		DrawRectangle(buttonPos.x, buttonPos.y , 50,30, WHITE);
		DrawText("CLEAR", buttonPos.x, buttonPos.y, 10, RED);
	}
}



void drawCircleOnMouse(int* thickness, Circle* circlesA, Color* colors) {
	Vector2 mousePos = GetMousePosition();
	if (IsMouseButtonDown(0)) {
		canDraw = 1;
		circlesA[i].color = getColor(mousePos, colors);
		circlesA[i].thickness = getThickness(mousePos, thickness);
		for (int j = 0; j < i; j++) {
			if (mousePos.x == circlesA[j].positions.x && mousePos.y == circlesA[j].positions.y) {
				canDraw = 0;
			}
			if(mousePos.x < (buttonPos.x + 50) && mousePos.y < (buttonPos.y + 50) &&
	mousePos.x > (buttonPos.x) && mousePos.y > (buttonPos.y)){
		canDraw = 0;
	}
		}
		if (canDraw) {
			HideCursor();
			circlesA[i].positions.x = mousePos.x;
			circlesA[i].positions.y = mousePos.y;
			i++;
		}
	};

	for (int j = 0; j < i; j++) {
		DrawCircle(circlesA[j].positions.x, circlesA[j].positions.y, circlesA[j].thickness, circlesA[j].color);
		
	}

	Color outlineColor = ColorIsEqual(getColor(mousePos, colors), BLACK) ? WHITE : getColor(mousePos, colors);
	DrawCircleLines(mousePos.x, mousePos.y, getThickness(mousePos, thickness), outlineColor);

}

int main() {
	Circle *circlesArray = (Circle *)malloc(500 * sizeof(Circle));
	Color colors[NUMBER_COLORS] = {WHITE, RED, GREEN, BLUE, PURPLE, ORANGE, BLACK};
	int thickness[NUMBER_THICKNESSES] = {LARGE, MEDIUM, SMALL};
	InitWindow(WIDTH, HEIGHT, "Paint v1.0");
	SetTargetFPS(FPS);
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			drawCircleOnMouse(thickness, circlesArray, colors);
			createColorButtons(colors);
			createThicknessButtons(thickness);
			createClearBackgroundButton(circlesArray, i);
			DrawText("Paint v1.0", WIDTH/2 - 70, 15, 30, WHITE);
			DrawFPS(WIDTH - 100, HEIGHT - 30);
		EndDrawing();
		printf("%d\n", i);
	}

	CloseWindow();

	return 0;
}