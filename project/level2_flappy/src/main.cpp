#include "../include/config.hpp"
#include "../include/game.hpp"
#include "raylib.h"
#include <algorithm>

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
