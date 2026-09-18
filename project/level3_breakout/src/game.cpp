#include "../include/game.hpp"
#include "raylib.h"
#include <vector>

// BOLA
Ball CreateBall() {
  return {{Config::SCREEN_WIDTH / 2.0f, 535.0f},
          {Config::BALL_SPEED, -Config::BALL_SPEED},
          Config::BALL_RADIUS,
          YELLOW};
}

void UpdateBall(Ball &ball, float dt) {
  // gerakan bola
  ball.position.x += ball.velocity.x * dt;
  ball.position.y += ball.velocity.y * dt;

  // pantulan bola
  // layar kiri
  if (ball.position.x - ball.radius < 0) {
    ball.position.x = ball.radius;
    ball.velocity.x *= -1.0f;
  }

  // layar kanan
  if (ball.position.x + ball.radius > Config::SCREEN_WIDTH) {
    ball.position.x = Config::SCREEN_WIDTH - ball.radius;
    ball.velocity.x *= -1.0f;
  }

  // layar atas
  if (ball.position.y < 0) {
    ball.position.y = ball.radius;
    ball.velocity.y *= -1.0f;
  }

  /**
   * untuk layar bawah nanti ada gameover
   * ganti state game
   */
}

// PADDLE
Paddle CreatePaddle() {
  return {{(Config::SCREEN_WIDTH - 100.0f) / 2.0f, 540.0f, Config::PADDLE_WIDTH,
           Config::PADDLE_HEIGHT},
          Config::PADDLE_SPEED,
          WHITE};
}

void UpdatePaddle(Paddle &paddle, float dt) {
  // input
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    paddle.rect.x -= paddle.speed * dt; // gerak kekiri
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    paddle.rect.x += paddle.speed * dt; // gerak kekanan

  // batas layar
  if (paddle.rect.x < 0)
    paddle.rect.x = 0; // layar kiri
  if (paddle.rect.x > Config::SCREEN_WIDTH - paddle.rect.width)
    paddle.rect.x = Config::SCREEN_WIDTH - paddle.rect.width; // layar kanan
}

// BRICKS
// inisialisasi bata-bata
std::vector<Brick> CreateBricks() {
  /**
   * Selalu pakai vector untuk @param brick
   * vector = semua bata
   */
  std::vector<Brick> bricks;
  for (size_t i = 0; i < Config::ROWS; i++) {
    for (size_t j = 0; j < Config::COLS; j++) {
      Brick b;
      b.rect = {Config::OFFSET_X + j * (Config::BRICK_WIDTH + Config::PADDING),
                Config::OFFSET_Y + i * (Config::BRICK_HEIGHT + Config::PADDING),
                Config::BRICK_WIDTH, Config::BRICK_HEIGHT};
      b.active = true;

      // warna bata
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

  return bricks;
}

void ResetBricks(std::vector<Brick> &bricks) {
  for (auto &brick : bricks) {
    brick.active = true;
  }
}

// COLLISION & SCORE
bool CollisionBallPaddle(Ball &ball, Paddle &paddle) {
  Rectangle ballRect = {ball.position.x, ball.position.y, ball.radius * 2,
                        ball.radius * 2};

  if (CheckCollisionRecs(ballRect, paddle.rect) && ball.velocity.y > 0) {
    ball.position.y = paddle.rect.y - ball.radius;
    ball.velocity.y *= -1.0f;
    return true;
  }
  return false;
}

void UpdateScore(Ball &ball, std::vector<Brick> &bricks, int &score) {
  // cek collision dg bata
  for (auto &brick : bricks) {

    if (!brick.active)
      continue;

    // kalau collision tambah skor
    if (CheckCollisionCircleRec(ball.position, ball.radius, brick.rect)) {
      brick.active = false;
      ball.velocity.y *= -1.0f;
      score += Config::POINT;
    }
  }
}

// LOGIC GAME
void ResetGame(Ball &ball, Paddle &paddle, std::vector<Brick> &bricks,
               int &score, int &attempts) {
  ball = CreateBall();
  paddle = CreatePaddle();
  ResetBricks(bricks);
  score = 0;
  attempts = 3;
}

bool IsGameOver(Ball &ball) {
  return ball.position.y - ball.radius > Config::SCREEN_HEIGHT;
}

// DRAWING
void DrawBall(const Ball &ball) {
  // Gambar isi bola
  DrawCircleV(ball.position, ball.radius, ball.color);

  // Gambar outline bola
  DrawCircleLines(static_cast<int>(ball.position.x),
                  static_cast<int>(ball.position.y), ball.radius, ORANGE);
}

void DrawPaddle(const Paddle &paddle) {
  DrawRectangleRec(paddle.rect, paddle.color);
}

void DrawBrick(const std::vector<Brick> &bricks) {
  for (const auto &brick : bricks) {
    if (brick.active) {
      DrawRectangleRec(brick.rect, brick.color);
    }
  }
}

void DrawUI(int &score, int &attempts, GameState &state) {
  DrawText(TextFormat("SCORE: %d", score), 10, 10, 25, WHITE);
  DrawText(TextFormat("ATTEMPTS: %d", attempts), Config::SCREEN_WIDTH - 180, 10,
           25, WHITE);

  if (state == GameState::Playing) {
    // DrawText("Tekan A/D untuk bergerak ke kanan/kiri", 115,
    //        Config::SCREEN_HEIGHT - 60, 20, WHITE);

    return;
  }

  DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
                Fade(BLACK, 0.5f));

  DrawText("GAME OVER", 280, 200, 40, RED);

  DrawText(TextFormat("SCORE: %d", score), 330, 290, 25, WHITE);

  DrawText("PRESS SPACE TO RESTART", 250, 380, 20, WHITE);
}

void DrawGame(const Ball &ball, const Paddle &paddle,
              const std::vector<Brick> &brick, int &score, int &attempts,
              GameState &state) {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawBall(ball);
  DrawPaddle(paddle);
  DrawBrick(brick);
  DrawUI(score, attempts, state);
  EndDrawing();
}
