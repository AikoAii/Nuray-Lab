#include "raylib.h"

int main() {
  InitWindow(800, 450, "C++ Game Lab - EX Delta Time");
  SetTargetFPS(120);

  float posX_tanpa_dt = 100.0f;
  float posX_dengan_dt = 100.0f;

  float kecepatan = 300.0f; // 300px/detik

  while (!WindowShouldClose()) {
    float dt = GetFrameTime(); // ambil waktu per frame (dalam detik)

    // 1. Tanpa Delta Time (berdasarkan detik nyata)
    posX_tanpa_dt += 2.0f;

    // 2. Dengan Delta Time
    posX_dengan_dt += kecepatan * dt;

    // reset posisi ke kiri jika sudah lewat layar
    if (posX_tanpa_dt > 800)
      posX_tanpa_dt = 0;
    if (posX_dengan_dt > 800)
      posX_dengan_dt = 0;

    BeginDrawing();
    ClearBackground(DARKBLUE);

    // kotak atas: Tanpa DT (Merah)
    DrawRectangle((int)posX_tanpa_dt, 100, 40, 40, RED);
    DrawText("Tanpa Delta Time (Bergantung FPS", 100, 70, 20, WHITE);

    // kotak bawah: Dengan DT (Hijau)
    DrawRectangle((int)posX_dengan_dt, 250, 40, 40, GREEN);
    DrawText("Mangunakan Delta Time (Konsisten per detik", 100, 220, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
