#include "raylib.h"

int main() {
	InitWindow(800, 450, "Hello!!!");
	
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			DrawText("Hello world", 300, 225, 30, BLACK);
		EndDrawing();
	}
	
	CloseWindow();
	
	return 0;
}