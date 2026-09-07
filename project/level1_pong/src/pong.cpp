#include "raylib.h"

// struktur padel
struct Paddle {
  Rectangle rect;
  float speed;
  Color color;
};

// struktur bola
struct Ball {
  Vector2 position;
  Vector2 velocity;
  float radius;
  Color color;
};

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "C++ Game Lab - Level 1: Pong Game");
  SetTargetFPS(120);

  // init Paddle player;
  Paddle player = {
      {50.0f, (screenHeight / 2.0f) - 40.0f, 20.0f, 80.0f}, 500.0f, BLUE};

  Paddle enemy = {
      {(float)screenWidth - 70.0f, (screenHeight / 2.0f) - 40.0f, 20.0f, 80.0f},
      350.0f,
      RED};

  // init bola
  Ball ball = {{(float)screenWidth / 2, (float)screenHeight / 2},
               {-400.0f, 300.0f},
               10.0f,
               YELLOW};

  // skor player
  int playerScore = 0;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // update player
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      player.rect.y -= player.speed * dt;

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      player.rect.y += player.speed * dt;

    // batas layar paddle
    if (player.rect.y < 0)
      player.rect.y = 0;
    if (player.rect.y > screenHeight - player.rect.height)
      player.rect.y = screenHeight - player.rect.height;

    // update enemy
    float enemyCenter = enemy.rect.y + (enemy.rect.height / 2.0f);

    if (ball.position.y > enemyCenter) {
      enemy.rect.y += enemy.speed * dt; // kejar ke bawah
    } else if (ball.position.y < enemyCenter)
      enemy.rect.y -= enemy.speed * dt;

    // batas layar enemy
    if (enemy.rect.y < 0)
      enemy.rect.y = 0;
    if (enemy.rect.y > screenHeight - enemy.rect.height)
      enemy.rect.y = screenHeight - enemy.rect.height;

    // update bola
    ball.position.x += ball.velocity.x * dt;
    ball.position.y += ball.velocity.y * dt;

    // pantulan bola dengan dinding atas
    if (ball.position.y - ball.radius < 0) {
      ball.position.y = ball.radius;
      ball.velocity.y *= -1.0f;
    }

    // pantulan bola dengan dinding bawah
    if (ball.position.y + ball.radius > screenHeight) {
      ball.position.y = screenHeight - ball.radius;
      ball.velocity.y *= -1.0f;
    }

    // buat rectangle virtual agar bisa dicek dengan CheckCollisionRecs
    Rectangle ballRect = {ball.position.x - ball.radius,
                          ball.position.y - ball.radius, ball.radius * 2,
                          ball.radius * 2};

    Rectangle enemyRect = {enemy.rect.x, enemy.rect.y, enemy.rect.width,
                           enemy.rect.height};

    // pantulan bola dengan paddle
    if (CheckCollisionRecs(ballRect, player.rect) && ball.velocity.x < 0) {

      // keluarkan bola dari dalam paddle
      ball.position.x = player.rect.x + player.rect.width + ball.radius;

      // pantulkan bola ke kanan
      ball.velocity.x *= -1.0f;

      // tambah skor player
      ++playerScore;
    }

    // pantulan enemy
    if (CheckCollisionRecs(ballRect, enemyRect) && ball.velocity.x > 0) {
      // keluaran bola dari enemy
      ball.position.x = enemy.rect.x - ball.radius;

      // pantulkan bola
      ball.velocity.x *= -1.0f;
    }

    // pantulan bola dengan dinding kanan
    if (ball.position.x + ball.radius > screenWidth) {
      ball.position.x = screenWidth - ball.radius;
      ball.velocity.x *= -1.0f;
    }

    // kondisi jika bola melewati batas kiri
    // untuk sementara bola di-reset
    if (ball.position.x - ball.radius < 0) {
      ball.position = {(float)screenWidth / 2, (float)screenHeight / 2};

      // reset velocity bola ke arah kanan
      ball.velocity = {400.0f, (float)GetRandomValue(-250, 250)};

      // reset score
      playerScore = 0;
    }

    // kecepatan bola berdasarkan score
    int speedLevel = playerScore / 5;

    float ballSpeed = 400.0f;

    for (int i = 0; i < speedLevel; ++i) {
      ballSpeed *= 1.1f;
    }

    // pertahankan arah bola
    if (ball.velocity.x > 0)
      ball.velocity.x = ballSpeed;
    else if (ball.velocity.x < 0)
      ball.velocity.x = -ballSpeed;

    // BeginDrawing();
    //
    // ClearBackground(GREEN);
    //
    // // garis tengah sebagai net
    // for (int y = 0; y < screenHeight; y += 20) {
    //   DrawRectangle(screenWidth / 2 - 2, y, 4, 10, WHITE);
    // }
    //
    // DrawRectangle(0, screenHeight / 2 - 2, 800, 2, DARKGREEN);
    //
    // // gambar Player dan bola
    // DrawRectangleRec(player.rect, player.color);
    // DrawRectangleRec(enemy.rect, enemy.color);
    // DrawCircleV(ball.position, ball.radius, ball.color);
    //
    // // gambar skor player
    // DrawText(TextFormat("SCORE: %d", playerScore), 20, 20, 40, WHITE);
    //
    // EndDrawing();
    BeginDrawing();

    ClearBackground(Color{10, 12, 18, 255});

    // LAPANGAN
    // warna dasar lapangan
    DrawRectangle(10, 10, screenWidth - 20, screenHeight - 20, GREEN);

    // garis luar lapangan
    DrawRectangleLinesEx({10, 10, screenWidth - 20.0f, screenHeight - 20.0f}, 3,
                         Color{80, 90, 105, 255});

    // NET TENGAH

    for (int y = 15; y < screenHeight - 10; y += 24) {
      DrawRectangle(screenWidth / 2 - 2, y, 4, 12, Color{100, 110, 125, 255});
    }

    // GARIS TENGAH HORIZONTAL

    DrawLine(10, screenHeight / 2, screenWidth - 10, screenHeight / 2, WHITE);

    // LINGKARAN TENGAH

    DrawCircleLines(screenWidth / 2, screenHeight / 2, 55, WHITE);

    DrawCircleLines(screenWidth / 2, screenHeight / 2, 4, WHITE);

    // AREA PLAYER

    // garis vertikal area player
    DrawLine(140, 10, 140, screenHeight - 10, Color{30, 45, 40, 255});

    // garis vertikal area enemy
    DrawLine(screenWidth - 140, 10, screenWidth - 140, screenHeight - 10,
             Color{30, 45, 40, 255});

    // PLAYER & ENEMY

    DrawRectangleRec(player.rect, player.color);
    DrawRectangleRec(enemy.rect, enemy.color);

    // BOLA

    DrawCircleV(ball.position, ball.radius, ball.color);

    // outline bola
    DrawCircleLines((int)ball.position.x, (int)ball.position.y, ball.radius + 2,
                    WHITE);

    // SCORE

    DrawText(TextFormat("%02d", playerScore), 80, 30, 48, WHITE);

    DrawText("PLAYER", 82, 82, 16, Color{120, 130, 145, 255});

    // DrawText("CPU", screenWidth - 115, 82, 16, Color{120, 130, 145, 255});

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
