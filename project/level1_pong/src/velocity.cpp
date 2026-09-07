#include "raylib.h"

int main() {
  // init ukuran layar
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "C++ Game Lab - Eksperimen Velocity");
  SetTargetFPS(120);

  // posisi awal bola1 di tengah layar
  Vector2 ballPos = {(float)screenWidth / 2, (float)screenHeight / 2};

  // velocity awal: bergerak ke kanan-bawah dg speed 300px/s
  Vector2 ballVel = {300.0f, 250.0f};
  float ballRadius = 15.0f;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // 1. update posisi bola berdasarlam velocity dan dt
    ballPos.x += ballVel.x * dt;
    ballPos.y += ballVel.y * dt;

    // 2. logic pantuan sederhana (Bounce)
    // kalau bola nabrak dinding akan memantul ke arah berlawanan
    if (ballPos.x - ballRadius < 0 || ballPos.x + ballRadius > screenWidth)
      ballVel.x *= -1.0f; // dikalikan -1 unntuk membalik arah

    if (ballPos.y - ballRadius < 0 || ballPos.y + ballRadius > screenHeight)
      ballVel.y *= -1.0f;

    BeginDrawing();
    ClearBackground(BLACK);

    // gambar bola
    DrawCircleV(ballPos, ballRadius, YELLOW);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
