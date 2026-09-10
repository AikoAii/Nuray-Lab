/*
 * @file flappy-ball.cpp
 * @brief Game sedehana "Flappy Ball" pake Raylib
 * @author Annur Musthofa (Nuy)
 * @version 1.3
 * @date 10-09-2026
 */

#include "raylib.h"

// Ukuran layar
constexpr int SCREEN_WIDTH = 450;
constexpr int SCREEN_HEIGHT = 800;

// konfigurasi bola
constexpr float BALL_RADIUS{20.0f}; // radius bola
constexpr float GRAVITY{1200.0f};   // gaya gravitasi
constexpr float JUMP_FORCE{400.0f}; // gaya lompatan bola

// konfigurasi pipa
constexpr float PIPE_WIDTH{60.0f};     // lebar pipa
constexpr float PIPE_GAP{200.0f};      // jarak vertikal pipa
constexpr float PIPE_DISTANCE{300.0f}; // jarak horizontal pipa
constexpr float PIPE_SPEED{300.0f};    // kecepatan pipa

// batas tinggi pipa
constexpr float PIPE_MIN_TOP{100.0f}; // minimum
constexpr float PIPE_MAX_TOP{350.0f}; // maksimum

// batas Delta Time
constexpr float MAX_DT{1.0f / 30.0f};

// keadaan permainan
enum class GameState { PLAYING, GAME_OVER };

// struktur bola
struct Bola {
  Vector2 position; // posisi bola di tengah layar
  float velocityY;  // kecepatan gerak (+kebawah -keatas)
  float radius;     // ukuran bola
  Color color;      // warna bola
};

// struktur pipa
struct Pipe {
  Vector2 position;   // posisi horizontal dan vertikal pipa
  float width;        // lebar pipa
  float topHeight;    // tinggi pipa atas
  float bottomY;      // posisi Y pipa bawah
  float bottomHeight; // tinggi pipa bawah
  float speed;        // kecepatan gerak pipa
  bool passed;        // cek bola lewat
};
