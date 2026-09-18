#pragma once
#include "config.hpp"
#include "raylib.h"
#include <vector>

// GAME STATE
enum class GameState { Playing, GameOver };

// ---------------
// GAME OBJECT
// ---------------

// Bola
struct Ball {
  Vector2 position{};
  Vector2 velocity{};
  float radius{};
  Color color;
};

// Paddle
struct Paddle {
  Rectangle rect{};
  float speed{};
  Color color;
};

// Bata
struct Brick {
  Rectangle rect{};

  float brickWidth{};
  float brickHeight{};

  float padding{};
  float offsetX{};
  float offsetY{};

  bool active;
  Color color;
};

// --------------
// FUNCTION
// --------------

// BOLA
Ball CreateBall();                     // inisialisasi awal bola
void UpdateBall(Ball &ball, float dt); // update gerakan bola

// PADDLE
Paddle CreatePaddle();                       // inisialisasi paddle
void UpdatePaddle(Paddle &paddle, float dt); // update gerakan paddle

// BRICK
void ResetBricks(std::vector<Brick> &bricks); // reset bata ke awal
std::vector<Brick> CreateBricks();            // inisialisasi awal bata
void UpdateBricks();                          // update status bata

// COLLISION & SCORE
bool CollisionBallPaddle(Ball &ball,
                         Paddle &paddle); // cek tabrakan bola dg paddle
void UpdateScore(Ball &ball, std::vector<Brick> &bricks,
                 int &score); // cek bola dg bata

// LOGIC GAME
void ResetGame(Ball &ball, Paddle &paddle, std::vector<Brick> &brick,
               int &score, int &attempts); // reset game ke awal
bool IsGameOver(Ball &ball);

// DRAWING
void DrawBall(const Ball &ball);                          // gambar bola
void DrawPaddle(const Paddle &paddle);                    // gambar paddle
void DrawBrick(const std::vector<Brick> &bricks);         // gambar bata-bata
void DrawUI(int &score, int &attempts, GameState &state); // UI
void DrawGame(const Ball &ball, const Paddle &paddle,
              const std::vector<Brick> &bricks, int &score, int &attempts,
              GameState &state); // game
