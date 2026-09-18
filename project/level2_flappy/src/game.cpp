#include "../include/game.hpp"
#include <raylib.h>

Ball CreateBall() {
  return {{Config::SCREEN_WIDTH / 2.0f, Config::SCREEN_HEIGHT / 2.0f},
          0.0f,
          Config::BALL_RADIUS,
          YELLOW};
}

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

void ResetPipe(Pipe &pipe, float x) {
  pipe.position = {x, 0.0f};

  pipe.topHeight = static_cast<float>(
      GetRandomValue(Config::PIPE_MIN_TOP, Config::PIPE_MAX_TOP));

  pipe.bottomY = pipe.topHeight + Config::PIPE_GAP;
  pipe.bottomHeight = Config::SCREEN_HEIGHT - pipe.bottomY;
  pipe.passed = false;
}

Pipe CreatePipe(float x) {
  Pipe pipe{.width = Config::PIPE_WIDTH,
            .speed = Config::PIPE_SPEED,
            .color = DARKGREEN};

  ResetPipe(pipe, x);

  return pipe;
}

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

void UpdateScore(const Ball &ball, Pipe &pipe, int &score) {
  const bool passedPipe = pipe.position.x + pipe.width < ball.position.x;

  if (!pipe.passed && passedPipe) {
    ++score;
    pipe.passed = true;
  }
}

void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB, int &score) {

  ball = CreateBall();

  ResetPipe(pipeA, Config::SCREEN_WIDTH);
  ResetPipe(pipeB, Config::SCREEN_WIDTH + Config::PIPE_DISTANCE);

  score = 0;
}

bool IsGameOver(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB) {

  const bool hitPipe = CheckBallPipeCollision(ball, pipeA) ||
                       CheckBallPipeCollision(ball, pipeB);

  const bool hitFloor = ball.position.y + ball.radius >= Config::SCREEN_HEIGHT;

  const bool hitCeiling = ball.position.y - ball.radius <= 0.0f;

  return hitPipe || hitFloor || hitCeiling;
}

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
                  static_cast<int>(ball.position.y), ball.radius, ORANGE);
}

void DrawUI(int score, GameState state) {
  DrawText(TextFormat("SCORE: %d", score), 20, 30, 30, WHITE);

  if (state == GameState::Playing) {
    DrawText("SPACE / UP = JUMP!", 115, Config::SCREEN_HEIGHT - 60, 20, WHITE);

    return;
  }

  DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
                Fade(BLACK, 0.5f));

  DrawText("GAME OVER", 105, 300, 40, RED);

  DrawText(TextFormat("SCORE: %d", score), 160, 360, 25, WHITE);

  DrawText("PRESS SPACE TO RESTART", 70, 430, 20, WHITE);
}

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
