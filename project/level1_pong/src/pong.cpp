/*
 * @file pong.cpp
 * @author Annur Musthofa (Nuy)
 * @brief GameDev level-1: Pong Game pakai raylib (Structured Clean Code)
 * @version 2.0
 * @date 07-09-2026
 */

#include "raylib.h"
#include <algorithm>

// ============================================================
// CONFIGURATION
// ============================================================

namespace Config {
inline constexpr int SCREEN_WIDTH = 800;   // lebar layar
inline constexpr int SCREEN_HEIGHT = 450;  // tinggi layar
inline constexpr int TARGET_FPS = 120;     // target FPS

inline constexpr float PADDLE_WIDTH = 20.0f;
inline constexpr float PADDLE_HEIGHT = 80.0f;
inline constexpr float PLAYER_SPEED = 500.0f;
inline constexpr float ENEMY_SPEED = 350.0f;

inline constexpr float BALL_RADIUS = 10.0f;
inline constexpr float BALL_INITIAL_SPEED = 400.0f;

// Batas delta time biar physics gak loncat terlalu jauh
// pas game lagi lag atau frame drop
inline constexpr float MAX_DT = 1.0f / 30.0f;
} // namespace Config

// ============================================================
// GAME STATE
// ============================================================

/**
 * @brief Menentukan keadaan game sekarang.
 */
enum class GameState { Playing, GameOver };

// ============================================================
// GAME OBJECTS
// ============================================================

/**
 * @brief Menyimpan data yang dimiliki Paddle (Player / Enemy).
 */
struct Paddle {
  Rectangle rect{};
  float speed{};
  Color color{};
};

/**
 * @brief Menyimpan data yang dimiliki Ball.
 */
struct Ball {
  Vector2 position{};
  Vector2 velocity{};
  float radius{};
  Color color{};
};

// ============================================================
// PADDLE & BALL FACTORIES
// ============================================================

/**
 * @brief Membuat Paddle untuk Player.
 * @return Paddle player yang sudah diinisialisasi.
 */
Paddle CreatePlayerPaddle() {
  return Paddle{
      .rect = {50.0f, (Config::SCREEN_HEIGHT / 2.0f) - (Config::PADDLE_HEIGHT / 2.0f),
               Config::PADDLE_WIDTH, Config::PADDLE_HEIGHT},
      .speed = Config::PLAYER_SPEED,
      .color = BLUE};
}

/**
 * @brief Membuat Paddle untuk Enemy (CPU).
 * @return Paddle enemy yang sudah diinisialisasi.
 */
Paddle CreateEnemyPaddle() {
  return Paddle{
      .rect = {(float)Config::SCREEN_WIDTH - 70.0f,
               (Config::SCREEN_HEIGHT / 2.0f) - (Config::PADDLE_HEIGHT / 2.0f),
               Config::PADDLE_WIDTH, Config::PADDLE_HEIGHT},
      .speed = Config::ENEMY_SPEED,
      .color = RED};
}

/**
 * @brief Membuat Ball dengan nilai awal.
 * @return Ball yang sudah diinisialisasi.
 */
Ball CreateBall() {
  return Ball{
      .position = {(float)Config::SCREEN_WIDTH / 2.0f, (float)Config::SCREEN_HEIGHT / 2.0f},
      .velocity = {-Config::BALL_INITIAL_SPEED, 300.0f},
      .radius = Config::BALL_RADIUS,
      .color = YELLOW};
}

// ============================================================
// UPDATE LOGIC
// ============================================================

/**
 * @brief Update posisi dan input Player.
 * @param player Paddle player.
 * @param dt Delta time.
 */
void UpdatePlayer(Paddle &player, float dt) {
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    player.rect.y -= player.speed * dt;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    player.rect.y += player.speed * dt;

  // Batas layar paddle player
  if (player.rect.y < 0.0f)
    player.rect.y = 0.0f;
  if (player.rect.y > Config::SCREEN_HEIGHT - player.rect.height)
    player.rect.y = Config::SCREEN_HEIGHT - player.rect.height;
}

/**
 * @brief Update AI Enemy untuk mengejar posisi bola.
 * @param enemy Paddle enemy.
 * @param ball Ball target.
 * @param dt Delta time.
 */
void UpdateEnemy(Paddle &enemy, const Ball &ball, float dt) {
  const float enemyCenter = enemy.rect.y + (enemy.rect.height / 2.0f);

  if (ball.position.y > enemyCenter) {
    enemy.rect.y += enemy.speed * dt;
  } else if (ball.position.y < enemyCenter) {
    enemy.rect.y -= enemy.speed * dt;
  }

  // Batas layar paddle enemy
  if (enemy.rect.y < 0.0f)
    enemy.rect.y = 0.0f;
  if (enemy.rect.y > Config::SCREEN_HEIGHT - enemy.rect.height)
    enemy.rect.y = Config::SCREEN_HEIGHT - enemy.rect.height;
}

/**
 * @brief Update posisi dan kecepatan bola berdasarkan skor.
 * @param ball Ball yang di-update.
 * @param playerScore Score player untuk scaling kecepatan.
 * @param dt Delta time.
 */
void UpdateBall(Ball &ball, int playerScore, float dt) {
  ball.position.x += ball.velocity.x * dt;
  ball.position.y += ball.velocity.y * dt;

  // Pantulan dinding atas
  if (ball.position.y - ball.radius < 0.0f) {
    ball.position.y = ball.radius;
    ball.velocity.y *= -1.0f;
  }

  // Pantulan dinding bawah
  if (ball.position.y + ball.radius > (float)Config::SCREEN_HEIGHT) {
    ball.position.y = (float)Config::SCREEN_HEIGHT - ball.radius;
    ball.velocity.y *= -1.0f;
  }

  // Kecepatan bola bertambah seiring naiknya skor
  const int speedLevel = playerScore / 5;
  float ballSpeed = Config::BALL_INITIAL_SPEED;

  for (int i = 0; i < speedLevel; ++i) {
    ballSpeed *= 1.1f;
  }

  // Pertahankan arah horizontal bola
  if (ball.velocity.x > 0.0f) {
    ball.velocity.x = ballSpeed;
  } else if (ball.velocity.x < 0.0f) {
    ball.velocity.x = -ballSpeed;
  }
}

// ============================================================
// COLLISION & GAME OVER
// ============================================================

/**
 * @brief Menangani tabrakan bola dengan paddle dan dinding kanan.
 * @param ball Ball.
 * @param player Paddle player.
 * @param enemy Paddle enemy.
 * @param playerScore Score player.
 */
void CheckCollisions(Ball &ball, const Paddle &player, const Paddle &enemy, int &playerScore) {
  const Rectangle ballRect{
      ball.position.x - ball.radius,
      ball.position.y - ball.radius,
      ball.radius * 2.0f,
      ball.radius * 2.0f
  };

  const Rectangle enemyRect{enemy.rect.x, enemy.rect.y, enemy.rect.width, enemy.rect.height};

  // Pantulan bola dengan player
  if (CheckCollisionRecs(ballRect, player.rect) && ball.velocity.x < 0.0f) {
    ball.position.x = player.rect.x + player.rect.width + ball.radius;
    ball.velocity.x *= -1.0f;
    ++playerScore;
  }

  // Pantulan bola dengan enemy
  if (CheckCollisionRecs(ballRect, enemyRect) && ball.velocity.x > 0.0f) {
    ball.position.x = enemy.rect.x - ball.radius;
    ball.velocity.x *= -1.0f;
  }

  // Pantulan bola dengan dinding kanan
  if (ball.position.x + ball.radius > (float)Config::SCREEN_WIDTH) {
    ball.position.x = (float)Config::SCREEN_WIDTH - ball.radius;
    ball.velocity.x *= -1.0f;
  }
}

/**
 * @brief Cek apakah bola melewati batas kiri layar (Game Over / Player Miss).
 * @param ball Ball.
 * @return true jika bola lewat batas kiri.
 */
bool IsGameOver(const Ball &ball) {
  return (ball.position.x - ball.radius < 0.0f);
}

/**
 * @brief Reset game ke kondisi awal.
 */
void ResetGame(Ball &ball, Paddle &player, Paddle &enemy, int &score) {
  ball = CreateBall();
  player = CreatePlayerPaddle();
  enemy = CreateEnemyPaddle();
  score = 0;
}

// ============================================================
// DRAW FUNCTIONS
// ============================================================

/**
 * @brief Menggambar lapangan dan dekorasi arena.
 */
void DrawField() {
  // Warna dasar & garis luar lapangan
  DrawRectangle(10, 10, Config::SCREEN_WIDTH - 20, Config::SCREEN_HEIGHT - 20, GREEN);
  DrawRectangleLinesEx({10.0f, 10.0f, (float)Config::SCREEN_WIDTH - 20.0f, (float)Config::SCREEN_HEIGHT - 20.0f}, 3.0f, Color{80, 90, 105, 255});

  // Net tengah
  for (int y = 15; y < Config::SCREEN_HEIGHT - 10; y += 24) {
    DrawRectangle(Config::SCREEN_WIDTH / 2 - 2, y, 4, 12, Color{100, 110, 125, 255});
  }

  // Garis tengah horizontal & lingkaran tengah
  DrawLine(10, Config::SCREEN_HEIGHT / 2, Config::SCREEN_WIDTH - 10, Config::SCREEN_HEIGHT / 2, WHITE);
  DrawCircleLines(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2, 55.0f, WHITE);
  DrawCircleLines(Config::SCREEN_WIDTH / 2, Config::SCREEN_HEIGHT / 2, 4.0f, WHITE);

  // Garis area samping player & enemy
  DrawLine(140, 10, 140, Config::SCREEN_HEIGHT - 10, Color{30, 45, 40, 255});
  DrawLine(Config::SCREEN_WIDTH - 140, 10, Config::SCREEN_WIDTH - 140, Config::SCREEN_HEIGHT - 10, Color{30, 45, 40, 255});
}

/**
 * @brief Menggambar paddle ke layar.
 */
void DrawPaddle(const Paddle &paddle) {
  DrawRectangleRec(paddle.rect, paddle.color);
}

/**
 * @brief Menggambar bola dan outline-nya ke layar.
 */
void DrawBall(const Ball &ball) {
  DrawCircleV(ball.position, ball.radius, ball.color);
  DrawCircleLines((int)ball.position.x, (int)ball.position.y, ball.radius + 2.0f, WHITE);
}

/**
 * @brief Menggambar UI (Skor & Pesan Game Over).
 */
void DrawUI(int playerScore, GameState state) {
  // Skor selalu tampil
  DrawText(TextFormat("%02d", playerScore), 80, 30, 48, WHITE);
  DrawText("PLAYER", 82, 82, 16, Color{120, 130, 145, 255});

  if (state == GameState::GameOver) {
    // Overlay gelap transparan
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Fade(BLACK, 0.6f));

    DrawText("GAME OVER", Config::SCREEN_WIDTH / 2 - 110, Config::SCREEN_HEIGHT / 2 - 50, 40, RED);
    DrawText("PRESS SPACE TO RESTART", Config::SCREEN_WIDTH / 2 - 130, Config::SCREEN_HEIGHT / 2 + 10, 20, WHITE);
  }
}

/**
 * @brief Menggambar keseluruhan elemen game.
 */
void DrawGame(const Ball &ball, const Paddle &player, const Paddle &enemy, int playerScore, GameState state) {
  BeginDrawing();
  ClearBackground(Color{10, 12, 18, 255});

  DrawField();
  DrawPaddle(player);
  DrawPaddle(enemy);
  DrawBall(ball);
  DrawUI(playerScore, state);

  EndDrawing();
}

// ============================================================
// MAIN
// ============================================================

int main() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "C++ Game Lab - Level 1: Pong Game");
  SetTargetFPS(Config::TARGET_FPS);

  // Random seed
  SetRandomSeed(static_cast<unsigned int>(GetTime() * 1000));

  // Inisialisasi Objek Game
  Paddle player = CreatePlayerPaddle();
  Paddle enemy = CreateEnemyPaddle();
  Ball ball = CreateBall();
  int playerScore = 0;

  GameState gameState = GameState::Playing;

  // Main game loop
  while (!WindowShouldClose()) {
    const float dt = std::min(GetFrameTime(), Config::MAX_DT);

    // ====================================================
    // PLAYING STATE
    // ====================================================
    if (gameState == GameState::Playing) {
      UpdatePlayer(player, dt);
      UpdateEnemy(enemy, ball, dt);
      UpdateBall(ball, playerScore, dt);
      CheckCollisions(ball, player, enemy, playerScore);

      // Cek apakah bola melewati batas kiri (Game Over)
      if (IsGameOver(ball)) {
        gameState = GameState::GameOver;
      }
    }
    // ====================================================
    // GAME OVER STATE
    // ====================================================
    else if (gameState == GameState::GameOver) {
      if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        ResetGame(ball, player, enemy, playerScore);
        gameState = GameState::Playing;
      }
    }

    // ====================================================
    // DRAW
    // ====================================================
    DrawGame(ball, player, enemy, playerScore, gameState);
  }

  CloseWindow();
  return 0;
}
