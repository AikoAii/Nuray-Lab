/*
 * @file flappy-biji.cpp
 * @author Annur Musthofa (Nuy)
 * @brief Game biji terbang
 * @version 1.0
 * @date 09-09-2026
 * @mood horny
 */

#include "raylib.h"
#include <chrono>

// struktur biji
struct Biji {
  Vector2 position; // posisi
  float velocityY;  // gerakan ke atas
  float radius;
  Color color;
};

// struct pipa
struct Pipe {
  Vector2 position;   // posisi x & y
  float width;        // lebar pipa
  float topHeight;    // tinggi pipa atas
  float bottomY;      // titik Y pipa bawah
  float bottomHeight; // tinggi pipa bawah
  float speed;        // kecepatan horizontal pipa
  Color color;        // warna
  bool passed;        // lacak status skor
};

int main() {

  // window
  constexpr int screenWidth{450};
  constexpr int screenHeight{800};
  InitWindow(screenWidth, screenHeight, "Flappy Biji");
  SetTargetFPS(120);

  // util
  constexpr float gap{200.0f};
  constexpr float gravity{1200.0f};
  constexpr float jumpForce{-400.0f};
  int score{0};

  // init biji dan pipa
  Biji biji = {
      {(float)screenWidth / 2, (float)screenHeight / 2}, 0.0f, 20.0f, YELLOW};

  Pipe pipeA;
  pipeA.position = {(float)screenWidth, 0.0f};
  pipeA.width = {60.0f};
  pipeA.topHeight = {(float)GetRandomValue(100, 350)};
  pipeA.bottomY = {pipeA.topHeight + gap};
  pipeA.bottomHeight = {(float)screenHeight - pipeA.bottomY};
  pipeA.speed = {300.0f};
  pipeA.color = DARKGREEN;

  Pipe pipeB;
  pipeB.position = {(float)screenWidth + 300.0f, 0.0f};
  pipeB.width = {60.0f};
  pipeB.topHeight = {(float)GetRandomValue(100, 350)};
  pipeB.bottomY = {pipeB.topHeight + gap};
  pipeB.bottomHeight = {(float)screenHeight - pipeB.bottomY};
  pipeB.speed = {300.0f};
  pipeB.color = DARKGREEN;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // BIJI
    // input biji
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))
      biji.velocityY = jumpForce;

    // gravity & update posisi biji
    biji.velocityY += gravity * dt;
    biji.position.y += biji.velocityY * dt;

    // clamp
    if (biji.position.y > screenHeight - 20) {
      biji.position.y = screenHeight - 20; // lantai
      biji.velocityY = {0.0f};             // reset kecepatan jatuh
    }
    if (biji.position.y < 20) {
      biji.position.y = {20}; // tahan di langit
      biji.velocityY = {0.0f};
    }

    // PIPA
    // update gerakan pipa
    // gerak dari kanan ke kiri
    pipeA.position.x -= pipeA.speed * dt;
    pipeB.position.x -= pipeB.speed * dt;

    // logic recycling & procedural generation
    // pipa A
    if (pipeA.position.x < -pipeA.width) // PR
    {
      pipeA.position.x = pipeB.position.x + 300.0f; // jarak ke pipa B

      // acak tinggi pipa
      pipeA.topHeight = (float)GetRandomValue(100, 350);

      // pipa bawah
      pipeA.bottomY = pipeA.topHeight + gap; // penyesuaian celah
      pipeA.bottomHeight = (float)screenHeight - pipeA.bottomY;

      // skor
      pipeA.passed = false; // reset
    }

    // pipa B
    if (pipeB.position.x < -pipeB.width) {
      pipeB.position.x = pipeA.position.x + 300.0f; // jarak ke pipa A

      // acak
      pipeB.topHeight = (float)GetRandomValue(100, 350);

      // pipa bawah
      pipeB.bottomY = pipeB.topHeight + gap; // celah
      pipeB.bottomHeight = (float)screenHeight - pipeB.bottomY;

      // skor
      pipeB.passed = false; // reset
    }

    // SCORE
    // skor pipa A
    if (!pipeA.passed && (pipeA.position.x + pipeA.width < biji.position.x)) {
      ++score;
      pipeA.passed = true; // kunci agar skor aman
    }

    // skor pipa B
    if (!pipeB.passed && (pipeB.position.x + pipeB.width < biji.position.x)) {
      ++score;
      pipeB.passed = true; // kunci
    }

    // collision
    // kotak biji
    Rectangle bijiRect = {biji.position.x - biji.radius,
                          biji.position.y - biji.radius, biji.radius * 2,
                          biji.radius * 2};

    // kotak pipa A
    Rectangle pipeATop = {pipeA.position.x, pipeA.position.y, pipeA.width,
                          pipeA.topHeight}; // atas

    Rectangle pipeABottom = {pipeA.position.x, pipeA.bottomY, pipeA.width,
                             pipeA.bottomHeight}; // bawah

    // kotak pipa B
    Rectangle pipeBTop = {pipeB.position.x, pipeB.position.y, pipeB.width,
                          pipeB.topHeight}; // atas

    Rectangle pipeBBottom = {pipeB.position.x, pipeB.bottomY, pipeB.width,
                             pipeB.bottomHeight}; // bawah

    // CEK COLLISION
    if (CheckCollisionRecs(bijiRect, pipeATop) ||
        CheckCollisionRecs(bijiRect, pipeABottom) ||
        CheckCollisionRecs(bijiRect, pipeBTop) ||
        CheckCollisionRecs(bijiRect, pipeBBottom)) {
    }

    BeginDrawing();
    ClearBackground(SKYBLUE); // warna BG

    // GAMBAR BIJI
    DrawCircleV(biji.position, biji.radius, biji.color); // biji

    DrawCircleLines((int)biji.position.x, (int)biji.position.y, biji.radius,
                    ORANGE); // outline biji

    // GAMBAR PIPA
    // Pipa A
    DrawRectangleRec(
        {pipeA.position.x, pipeA.position.y, pipeA.width, pipeA.topHeight},
        pipeA.color); // atas
    DrawRectangleRec(
        {pipeA.position.x, pipeA.bottomY, pipeA.width, pipeA.bottomHeight},
        pipeA.color); // bawah

    // Pipa B
    DrawRectangleRec(
        {pipeB.position.x, pipeB.position.y, pipeB.width, pipeB.topHeight},
        pipeB.color); // atas
    DrawRectangleRec(
        {pipeB.position.x, pipeB.bottomY, pipeB.width, pipeB.bottomHeight},
        pipeB.color); // bawah

    // TEKS
    DrawText("SPACE for jump!", 100, 700, 30, WHITE);
    DrawText(TextFormat("SCORE: %d", score), 20, 30, 30, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
