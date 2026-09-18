#include "../include/config.hpp"
#include "../include/game.hpp"
#include "algorithm"
#include "raylib.h"
#include <algorithm>
#include <vector>

int main() {
  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "BREAKOUT");

  SetTargetFPS(Config::TARGET_FPS);

  Ball ball = CreateBall();

  Paddle paddle = CreatePaddle();

  std::vector<Brick> bricks = CreateBricks();

  int score = 0;
  int attempts = 3;
  GameState state = GameState::Playing;

  while (!WindowShouldClose()) {
    const float dt = std::min(GetFrameTime(), Config::MAX_DT);

    if (state == GameState::Playing) {
      UpdateBall(ball, dt);
      UpdatePaddle(paddle, dt);
      UpdateScore(ball, bricks, score);
      CollisionBallPaddle(ball, paddle);

      if (IsGameOver(ball)) {
        --attempts;

        if (attempts <= 0) {
          state = GameState::GameOver;
        } else {
          ball = CreateBall();
          paddle = CreatePaddle();
        }
      }
    }

    if (state == GameState::GameOver) {
      if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        ResetGame(ball, paddle, bricks, score, attempts);
        state = GameState::Playing;
      }
    }

    DrawGame(ball, paddle, bricks, score, attempts, state);
  }
  CloseWindow();

  return 0;
}
