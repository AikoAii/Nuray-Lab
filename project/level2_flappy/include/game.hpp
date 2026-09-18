#pragma once
#include "config.hpp"
#include "raylib.h"

// GAME STATE
enum class GameState { Playing, GameOver };

// GAME OBJECT
struct Ball {
  Vector2 position{};
  float velocityY{};
  float radius{};
  Color color;
};

struct Pipe {
  Vector2 position{};

  float width{};
  float topHeight{};
  float bottomY{};
  float bottomHeight{};
  float speed{};

  bool passed;
  Color color;
};

// FUNCTION

// BOLA
Ball CreateBall();                     // Inisialisasi bola dg nilai awal
void UpdateBall(Ball &ball, float dt); // update gerakan dan fisika bola
// PIPA
void ResetPipe(Pipe &pipe, float x); // reset posisi dan celah pipa
Pipe CreatePipe(float x);            // Inisialisasi pipa dg nilai awal
void UpdatePipes(Pipe &pipeA, Pipe &pipeB,
                 float dt); // update posisi pipa recycling
// COLLISION & SCORE
bool CheckBallPipeCollision(const Ball &ball,
                            const Pipe &pipe); // cek tabrakan bola dg bola
void UpdateScore(const Ball &ball, Pipe &pipe,
                 int &score); // tambah score saat bola lewati pipa

// GAME LOGIC
void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB,
               int &score); // reset game ke kondisi awal

bool IsGameOver(const Ball &ball, const Pipe &pipeA,
                const Pipe &pipeB); // cek apakah game over

// DRAWING
void DrawBall(const Ball &ball);         // tampilkan bola dan outline
void DrawPipe(const Pipe &pipe);         // tampilkan pipa
void DrawUI(int score, GameState state); // tampilkan UI
void DrawGame(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB, int score,
              GameState state); // tampilkan semua elemen game
