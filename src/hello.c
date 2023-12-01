#include "raylib.h"

int main() {
	InitWindow(400, 100, "AoC 2023");
	
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			DrawText("Select a day to run!", 60, 35, 30, BLACK);
		EndDrawing();
	}
	
	CloseWindow();
	
	return 0;
}