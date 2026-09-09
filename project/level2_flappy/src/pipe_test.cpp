#include "raylib.h"

struct Pipe {
  Vector2 position;   // posisix dan y
  float width;        // lebar pipa
  float topHeight;    // tinggi pipa atas
  float bottomY;      // titik Y mulai pipa bawah
  float bottomHeight; // tinggi pipa bawah
  float speed;        // kecepatan pipa ke kiri
};

int main() {
  const int screenWidth = 450;
  const int screenHeight = 800;
  InitWindow(screenWidth, screenHeight, "Pipe Test");
  SetTargetFPS(120);

  float gap = 150.0f; // ukuran celah

  // inisialisasi 1 Pipa di awal
  Pipe pipeOne;
  pipeOne.position = {(float)screenWidth, 0.0f};
  pipeOne.width = 60.0f;
  pipeOne.topHeight = (float)GetRandomValue(100, 350);
  pipeOne.bottomY = pipeOne.topHeight + gap;
  pipeOne.bottomHeight = (float)screenHeight - pipeOne.bottomY;
  pipeOne.speed = 300.0f;

  Pipe pipeTwo;
  pipeTwo.position = {(float)screenWidth + 300.0f, 0.0f};
  pipeTwo.width = 60.0f;
  pipeTwo.topHeight = (float)GetRandomValue(100, 350);
  pipeTwo.bottomY = pipeTwo.topHeight + gap;
  pipeTwo.bottomHeight = (float)screenHeight - pipeTwo.bottomY;
  pipeTwo.speed = 300.0f;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // 1. UPDATE GERAKAN PIPA
    pipeOne.position.x -= pipeOne.speed * dt;
    pipeTwo.position.x -= pipeTwo.speed * dt;

    // 2. LOGIC RECYCLING & PROCEDURAL GENERATION
    // kalau pipa keluar dr sisi kiri layar
    if (pipeOne.position.x < -pipeOne.width) {
      // pindahkan ke kanan lagi
      pipeOne.position.x = pipeTwo.position.x + 300.0f;

      // acak ulang tinggi pipa
      pipeOne.topHeight = (float)GetRandomValue(100, 350);

      // sesuaikan posisi dan tinggi pipa bawah
      pipeOne.bottomY = pipeOne.topHeight + gap;
      pipeOne.bottomHeight = (float)screenHeight - pipeOne.bottomY;
    }

    if (pipeTwo.position.x < -pipeTwo.width) {
      // pindahkan ke kanan lagi
      pipeTwo.position.x = pipeOne.position.x + 300.0f;

      // acak ulang tinggi pipa
      pipeTwo.topHeight = (float)GetRandomValue(100, 350);

      // sesuaikan posisi dan tinggi pipa bawah
      pipeTwo.bottomY = pipeTwo.topHeight + gap;
      pipeTwo.bottomHeight = (float)screenHeight - pipeTwo.bottomY;
    }

    // 3. GAMBAR PIPA
    BeginDrawing();
    ClearBackground(SKYBLUE);

    // pipa atas
    DrawRectangleRec({pipeOne.position.x, pipeOne.position.y, pipeOne.width,
                      pipeOne.topHeight},
                     DARKGREEN);

    // pipa bawah
    DrawRectangleRec({pipeOne.position.x, pipeOne.bottomY, pipeOne.width,
                      pipeOne.bottomHeight},
                     DARKGREEN);

    // pipa atas
    DrawRectangleRec({pipeTwo.position.x, pipeTwo.position.y, pipeTwo.width,
                      pipeTwo.topHeight},
                     DARKGREEN);

    // pipa bawah
    DrawRectangleRec({pipeTwo.position.x, pipeTwo.bottomY, pipeTwo.width,
                      pipeTwo.bottomHeight},
                     DARKGREEN);

    DrawText("Analisis gerakan pipa dan ketinggi pipa yg acak@", 20, 30, 18,
             WHITE);
    DrawText(TextFormat("Posisi X Pipa pertama: %.1f", pipeOne.position.x), 20,
             60, 16, WHITE);
    DrawText(TextFormat("Posisi X Pipa kedua: %.1f", pipeTwo.position.x), 20,
             90, 16, WHITE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
