/*
 * @file flappy-ball.cpp
 * @brief Game sederhana "Flappy Ball" menggunakan Raylib.
 * @author Annur Musthofa (Nuy)
 * @version 2.0
 * @date 12-09-2026
 */

#include "raylib.h"

#include <algorithm>

namespace Config {

inline constexpr int SCREEN_WIDTH = 450;
inline constexpr int SCREEN_HEIGHT = 800;

inline constexpr float BALL_RADIUS = 20.0f;
inline constexpr float GRAVITY = 1200.0f;
inline constexpr float JUMP_FORCE = -400.0f;

inline constexpr float PIPE_WIDTH = 60.0f;
inline constexpr float PIPE_GAP = 200.0f;
inline constexpr float PIPE_DISTANCE = 300.0f;
inline constexpr float PIPE_SPEED = 300.0f;

inline constexpr int PIPE_MIN_TOP = 100;
inline constexpr int PIPE_MAX_TOP = 350;

// Membatasi delta time agar physics tetap stabil saat frame drop.
inline constexpr float MAX_DT = 1.0f / 30.0f;

} // namespace Config

// Menentukan kondisi permainan saat ini.
enum class GameState { Playing, GameOver };

// Menyimpan data dan properti bola.
struct Ball {
  Vector2 position{};
  float velocityY{};
  float radius{};
  Color color{};
};

// Menyimpan data dan properti pipa.
struct Pipe {
  Vector2 position{};

  float width{};
  float topHeight{};
  float bottomY{};
  float bottomHeight{};
  float speed{};

  Color color{};
  bool passed{};
};

// ============================================================
// BOLA
// ============================================================

/**
 * Membuat bola dengan nilai awal.
 */
Ball CreateBall() {
  return {
      .position = {Config::SCREEN_WIDTH / 2.0f, Config::SCREEN_HEIGHT / 2.0f},
      .velocityY = 0.0f,
      .radius = Config::BALL_RADIUS,
      .color = YELLOW};
}

/**
 * Memperbarui gerakan dan physics bola.
 */
void UpdateBall(Ball &ball, float dt) {
  if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) {
    ball.velocityY = Config::JUMP_FORCE;
  }

  ball.velocityY += Config::GRAVITY * dt;
  ball.position.y += ball.velocityY * dt;

  // Menjaga bola tetap berada di dalam layar.
  if (ball.position.y - ball.radius < 0.0f) {
    ball.position.y = ball.radius;
    ball.velocityY = 0.0f;
  }

  if (ball.position.y + ball.radius > Config::SCREEN_HEIGHT) {
    ball.position.y = Config::SCREEN_HEIGHT - ball.radius;
    ball.velocityY = 0.0f;
  }
}

// ============================================================
// PIPA
// ============================================================

/**
 * Mengatur ulang posisi dan celah pipa.
 */
void ResetPipe(Pipe &pipe, float x) {
  pipe.position = {x, 0.0f};

  pipe.topHeight = static_cast<float>(
      GetRandomValue(Config::PIPE_MIN_TOP, Config::PIPE_MAX_TOP));

  pipe.bottomY = pipe.topHeight + Config::PIPE_GAP;
  pipe.bottomHeight = Config::SCREEN_HEIGHT - pipe.bottomY;
  pipe.passed = false;
}

/**
 * Membuat pipa dengan nilai awal.
 */
Pipe CreatePipe(float x) {
  Pipe pipe{.width = Config::PIPE_WIDTH,
            .speed = Config::PIPE_SPEED,
            .color = DARKGREEN};

  ResetPipe(pipe, x);

  return pipe;
}

/**
 * Menggerakkan pipa dan mendaur ulang pipa yang keluar layar.
 */
void UpdatePipes(Pipe &pipeA, Pipe &pipeB, float dt) {
  const float movement = Config::PIPE_SPEED * dt;

  pipeA.position.x -= movement;
  pipeB.position.x -= movement;

  // Pipa yang keluar layar dipindahkan ke belakang pipa lainnya.
  if (pipeA.position.x + pipeA.width < 0.0f) {
    ResetPipe(pipeA, pipeB.position.x + Config::PIPE_DISTANCE);
  }

  if (pipeB.position.x + pipeB.width < 0.0f) {
    ResetPipe(pipeB, pipeA.position.x + Config::PIPE_DISTANCE);
  }
}

// ============================================================
// COLLISION & SCORE
// ============================================================

/**
 * Mengecek apakah bola bertabrakan dengan pipa.
 */
bool CheckBallPipeCollision(const Ball &ball, const Pipe &pipe) {
  const Rectangle topPipe{pipe.position.x, pipe.position.y, pipe.width,
                          pipe.topHeight};

  const Rectangle bottomPipe{pipe.position.x, pipe.bottomY, pipe.width,
                             pipe.bottomHeight};

  const bool hitTop =
      CheckCollisionCircleRec(ball.position, ball.radius, topPipe);

  const bool hitBottom =
      CheckCollisionCircleRec(ball.position, ball.radius, bottomPipe);

  return hitTop || hitBottom;
}

/**
 * Menambah score ketika bola berhasil melewati pipa.
 */
void UpdateScore(const Ball &ball, Pipe &pipe, int &score) {
  const bool passedPipe = pipe.position.x + pipe.width < ball.position.x;

  if (!pipe.passed && passedPipe) {
    ++score;
    pipe.passed = true;
  }
}

// ============================================================
// LOGIKA GAME
// ============================================================

/**
 * Mengembalikan seluruh game ke kondisi awal.
 */
void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB, int &score) {

  ball = CreateBall();

  ResetPipe(pipeA, Config::SCREEN_WIDTH);
  ResetPipe(pipeB, Config::SCREEN_WIDTH + Config::PIPE_DISTANCE);

  score = 0;
}

/**
 * Mengecek apakah kondisi Game Over terpenuhi.
 */
bool IsGameOver(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB) {

  const bool hitPipe = CheckBallPipeCollision(ball, pipeA) ||
                       CheckBallPipeCollision(ball, pipeB);

  const bool hitFloor = ball.position.y + ball.radius >= Config::SCREEN_HEIGHT;

  const bool hitCeiling = ball.position.y - ball.radius <= 0.0f;

  return hitPipe || hitFloor || hitCeiling;
}

// ============================================================
// GAMBAR
// ============================================================

/**
 * Menggambar bagian atas dan bawah sebuah pipa.
 */
void DrawPipe(const Pipe &pipe) {
  DrawRectangleRec(
      {pipe.position.x, pipe.position.y, pipe.width, pipe.topHeight},
      pipe.color);

  DrawRectangleRec(
      {pipe.position.x, pipe.bottomY, pipe.width, pipe.bottomHeight},
      pipe.color);
}

/**
 * Menggambar bola beserta outline-nya.
 */
void DrawBall(const Ball &ball) {
  DrawCircleV(ball.position, ball.radius, ball.color);

  DrawCircleLines(static_cast<int>(ball.position.x),
                  static_cast<int>(ball.position.y), ball.radius, ball.color);
}

/**
 * Menggambar UI sesuai kondisi game.
 */
void DrawUI(int score, GameState state) {
  DrawText(TextFormat("SCORE: %d", score), 20, 30, 30, WHITE);

  if (state == GameState::Playing) {
    DrawText("SPACE / UP = JUMP!", 95, Config::SCREEN_HEIGHT - 60, 20, WHITE);

    return;
  }

  DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
                Fade(BLACK, 0.5f));

  DrawText("GAME OVER", 105, 300, 40, RED);

  DrawText(TextFormat("SCORE: %d", score), 160, 360, 25, WHITE);

  DrawText("PRESS SPACE TO RESTART", 70, 430, 20, WHITE);
}

/**
 * Menggambar seluruh elemen game ke layar.
 */
void DrawGame(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB, int score,
              GameState state) {

  BeginDrawing();

  ClearBackground(SKYBLUE);

  DrawPipe(pipeA);
  DrawPipe(pipeB);
  DrawBall(ball);
  DrawUI(score, state);

  EndDrawing();
}

// ============================================================
// MAIN
// ============================================================

/**
 * Menjalankan inisialisasi dan game loop utama.
 */
int main() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "Flappy Ball");

  SetTargetFPS(120);

  // Membuat pola pipa berbeda setiap program dijalankan.
  SetRandomSeed(static_cast<unsigned int>(GetTime() * 1000));

  Ball ball = CreateBall();

  Pipe pipeA = CreatePipe(Config::SCREEN_WIDTH);
  Pipe pipeB = CreatePipe(Config::SCREEN_WIDTH + Config::PIPE_DISTANCE);

  int score = 0;
  GameState gameState = GameState::Playing;

  while (!WindowShouldClose()) {
    // Membatasi dt agar physics tidak melonjak saat frame drop.
    const float dt = std::min(GetFrameTime(), Config::MAX_DT);

    if (gameState == GameState::Playing) {
      UpdateBall(ball, dt);
      UpdatePipes(pipeA, pipeB, dt);

      UpdateScore(ball, pipeA, score);
      UpdateScore(ball, pipeB, score);

      if (IsGameOver(ball, pipeA, pipeB)) {
        gameState = GameState::GameOver;
      }
    }

    if (gameState == GameState::GameOver) {
      if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {

        ResetGame(ball, pipeA, pipeB, score);
        gameState = GameState::Playing;
      }
    }

    DrawGame(ball, pipeA, pipeB, score, gameState);
  }

  CloseWindow();

  return 0;
}
