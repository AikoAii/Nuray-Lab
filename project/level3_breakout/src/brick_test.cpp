#include "raylib.h"
#include <vector>

struct Paddle {
  Rectangle rect;
  float speed;
  Color color;
};

struct Ball {
  Vector2 position;
  Vector2 velocity;
  float radius;
  Color color;
};

struct Brick {
  Rectangle rect;
  bool active;
  Color color;
};

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Breakout Game");
  SetTargetFPS(120);

  Paddle paddle{{(screenWidth - 100.0f) / 2.0f, 540.0f, 100.0f, 20.0f},
                500.0f,
                BLUE}; // Tinggi paddle disesuaikan jadi 20.0f

  Ball ball{{screenWidth / 2.0f, 300.0f}, {300.0f, -300.0f}, 10.0f, YELLOW};

  // --- INISIALISASI GRID BATA (VECTOR) ---
  std::vector<Brick> bricks;
  int rows = 5;
  int cols = 8;
  float brickWidth = 85.0f;
  float brickHeight = 25.0f;
  float padding = 10.0f;
  float offsetX = 55.0f;
  float offsetY = 60.0f;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      Brick b;
      b.rect = {offsetX + j * (brickWidth + padding),
                offsetY + i * (brickHeight + padding), brickWidth, brickHeight};
      b.active = true;

      // Warna bertahap tiap baris
      if (i == 0)
        b.color = RED;
      else if (i == 1)
        b.color = ORANGE;
      else if (i == 2)
        b.color = YELLOW;
      else if (i == 3)
        b.color = GREEN;
      else
        b.color = BLUE;

      bricks.push_back(b);
    }
  }

  int score = 0;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // INPUT (Gunakan IsKeyDown agar gerakan paddle mulus saat ditahan)
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
      paddle.rect.x -= paddle.speed * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
      paddle.rect.x += paddle.speed * dt;

    // BATAS LAYAR PADDLE (Perbaiki screenHeight jadi screenWidth)
    if (paddle.rect.x < 0)
      paddle.rect.x = 0;
    if (paddle.rect.x > screenWidth - paddle.rect.width)
      paddle.rect.x = screenWidth - paddle.rect.width;

    // UPDATE BOLA
    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;

    // PANTULAN DINDING
    if (ball.position.x - ball.radius < 0) {
      ball.position.x = ball.radius;
      ball.velocity.x *= -1.0f;
    }
    if (ball.position.x + ball.radius > screenWidth) {
      ball.position.x = screenWidth - ball.radius;
      ball.velocity.x *= -1.0f;
    }
    if (ball.position.y - ball.radius < 0) {
      ball.position.y = ball.radius;
      ball.velocity.y *= -1.0f;
    }

    // COLLISION: BOLA VS PADDLE
    Rectangle ballRect = {ball.position.x - ball.radius,
                          ball.position.y - ball.radius, ball.radius * 2,
                          ball.radius * 2};
    if (CheckCollisionRecs(ballRect, paddle.rect) && ball.velocity.y > 0) {
      ball.position.y = paddle.rect.y - ball.radius;
      ball.velocity.y *= -1.0f;
    }

    // COLLISION: BOLA VS BRICKS (Vector loop)
    for (auto &brick : bricks) {
      if (!brick.active)
        continue;

      if (CheckCollisionCircleRec(ball.position, ball.radius, brick.rect)) {
        brick.active = false;     // Hancurkan bata
        ball.velocity.y *= -1.0f; // Pantulkan bola
        score += 10;              // Tambah skor
        break; // Keluar loop agar tidak multi-collision dalam 1 frame
      }
    }

    // DRAWING
    BeginDrawing();
    ClearBackground(Color{15, 17, 23, 255});

    // Gambar Brick dari vector
    for (const auto &brick : bricks) {
      if (brick.active)
        DrawRectangleRec(brick.rect, brick.color);
    }

    // Gambar Paddle & Ball
    DrawRectangleRec(paddle.rect, paddle.color);
    DrawCircleV(ball.position, ball.radius, ball.color);

    // Gambar Skor
    DrawText(TextFormat("SCORE: %d", score), 20, 20, 24, WHITE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
