/*
 * @file flappy-biji.cpp
 * @author Annur Musthofa (Nuy)
 * @brief Game biji terbang
 * @version 1.0
 * @date 09-09-2026
 * @mood horny
 */

#include "raylib.h"

// struktur biji
struct Biji {
  Vector2 position; // posisi
  float velocityY;  // gerakan ke atas
  float radius;
  Color color;
};

// struct pipa
struct Pipe {
  Vector2 position;   // posisi x & y
  float width;        // lebar pipa
  float topHeight;    // tinggi pipa atas
  float bottomY;      // titik Y pipa bawah
  float bottomHeight; // tinggi pipa bawah
  float speed;        // kecepatan horizontal pipa
};
