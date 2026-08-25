#include "raylib.h"

struct Player {
  Vector2 position;
  float speed;
  int size;
  Color color;
};

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "C++ Game LAb - Level 0: Input");

  // Inisialisasi posisi awal kotak
  Player player = {{400.0f, 225.0f}, 4.0f, 50, RED};
  // kecepatan gerak per frame

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    // 1. UPDATE

    // fitur sprint
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
      player.speed = 8.0f; // mode lari
    } else {
      player.speed = 4.0f; // normal
    }
    if (IsKeyDown(KEY_RIGHT))
      player.position.x += player.speed;
    if (IsKeyDown(KEY_LEFT))
      player.position.x -= player.speed;
    if (IsKeyDown(KEY_DOWN))
      player.position.y += player.speed;
    if (IsKeyDown(KEY_UP))
      player.position.y -= player.speed;

    // boundary check (batas layar)
    if (player.position.x < 0) {
      player.position.x = 0; // tahan di batas kiri
    }
    if (player.position.x > (screenWidth - 50)) {
      player.position.x = screenWidth - 50; // tahan di batas kanan
    }
    if (player.position.y < 0) {
      player.position.y = 0;
    }
    if (player.position.y > (screenHeight - 50)) {
      player.position.y = screenHeight - 50;
    }

    // 2. DRAWING
    BeginDrawing();
    ClearBackground(DARKBLUE);

    DrawRectangle((int)player.position.x, (int)player.position.y, player.size,
                  player.size, player.color);

    DrawText("Gunakan tombol panah (Arrow Keys) untuk menggerakan kotak!", 10,
             10, 15, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
