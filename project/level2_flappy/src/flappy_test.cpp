#include "raylib.h"

int main() {
  const int screenWidth = 450;
  const int screenHeight = 800;
  InitWindow(screenWidth, screenHeight, "Gravity Test");
  SetTargetFPS(120);

  // posisi awal burung di sisi kiri layar
  Vector2 birdPos = {(float)screenWidth / 2, (float)screenHeight / 2};

  // velocity vertikal awal
  float birdVelocityY = 0.0f;

  // konstanta fisika
  const float gravity = 1200.0f;   // tarikan gravitasi (px/s^2)
  const float jumpForce = -400.0f; // power lompat

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // 1. INPUT: spasi untuk lompat
    if (IsKeyPressed(KEY_SPACE)) {
      birdVelocityY = jumpForce;
    }

    // 2. FISIKA GRAVITASI: Tambah velocity dari gravity dan dt
    birdVelocityY += gravity * dt;

    // 3. UPDATE POSISI: Pindah posisi vertical dari velocity
    birdPos.y += birdVelocityY * dt;

    // 4. BATAS BAWAH (Lantai) & ATAS LAYAR
    if (birdPos.y > screenHeight - 20) {
      birdPos.y = screenHeight - 20; // tahan dibawah (lantai)
      birdVelocityY = 0.0f;          // reset kecepatan jatuh
    }
    if (birdPos.y < 20) {
      birdPos.y = 20; // tahan dilangit
      birdVelocityY = 0.0f;
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);

    // Gambar karaktelr burung (yeow circle)
    DrawCircleV(birdPos, 20.0f, YELLOW);
    DrawCircleLines((int)birdPos.x, (int)birdPos.y, 20.0f, ORANGE);

    // intruksi Test
    DrawText("SPACE for jump!", 50, 40, 20, DARKBLUE);
    DrawText(TextFormat("Velocity Y: %.f", birdVelocityY), 50, 70, 18,
             DARKBLUE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
