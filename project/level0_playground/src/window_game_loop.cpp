#include "raylib.h"

int main() {
  // 1. inisialisasi window
  const int screenWidth = 800;  // lebar window
  const int screenHeight = 450; // tinggi window
  InitWindow(screenWidth, screenHeight, "C++ Game Lab - Level 0");

  SetTargetFPS(60); // mengunci framerate pada 60 FPS

  // 2. Game Loop Utama
  while (!WindowShouldClose()) {
    // --- UPDATE (Logika Game dan Input) ---
    // belum ada isinya

    // --- Drawing (rendering grafis) ---
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Selamat datang di C++ Game Lab!", 225, 200, 20, WHITE);

    EndDrawing();
  }

  // 3. Cleanup / inisialisasi
  CloseWindow();

  return 0;
}
