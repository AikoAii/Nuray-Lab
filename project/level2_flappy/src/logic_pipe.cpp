/*
 * @file flappy-ball.cpp
 * @brief Game sedehana "Flappy Ball" pake Raylib
 * @author Annur Musthofa (Nuy)
 * @version 1.5
 * @date 10-09-2026
 */

#include "raylib.h"

// Ukuran layar
constexpr int SCREEN_WIDTH = 450;
constexpr int SCREEN_HEIGHT = 800;

// konfigurasi Ball
constexpr float BALL_RADIUS{20.0f}; // radius Ball
constexpr float GRAVITY{1200.0f};   // gaya gravitasi
constexpr float JUMP_FORCE{
    -400.0f}; // gaya lompatan Ball (diubah jadi minus agar bisa lompat ke atas)

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

//=======================================
// STRUKTUR
//======================================

// struktur Ball
struct Ball {
  Vector2 position; // posisi Ball di tengah layar
  float velocityY;  // kecepatan gerak (+kebawah -keatas)
  float radius;     // ukuran Ball
  Color color;      // warna Ball
};

// struktur pipa
struct Pipe {
  Vector2 position;   // posisi horizontal dan vertikal pipa
  float width;        // lebar pipa
  float topHeight;    // tinggi pipa atas
  float bottomY;      // posisi Y pipa bawah
  float bottomHeight; // tinggi pipa bawah
  float speed;        // kecepatan gerak pipa
  Color color;        // warna pipa
  bool passed;        // cek Ball lewat
};

//==========================
// BUAT FUNCTION REUSABILITY
//==========================

// DEKLARASI FUNCTION
Ball CreateBall();
Pipe CreatePipe(float x);
void ResetPipe(Pipe &pipe, float x);
void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB, int &score);
void UpdateBall(Ball &ball, float dt);
void UpdatePipes(Pipe &pipeA, Pipe &pipeB, float dt);
bool CheckBallPipeCollision(const Ball &ball, const Pipe &pipe);
void UpdateScore(const Ball &ball, Pipe &pipe, int &score);
void DrawGame(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB, int score,
              GameState state);

/**
 * @brief Buat objek Ball dengan nilai awal.
 *
 * @return Ball yang sudah diinisialisasi.
 */
Ball CreateBall() {
  return {{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f}, // tengah layar
          0.0f,
          BALL_RADIUS, // ukuran Ball
          YELLOW};     // warna
}

/**
 * @brief Membuat objek pipa di posisi X tertentu
 *
 * @param x Posisi X awal pipa.
 *
 * @return Pipa yang udah diinisialisasi.
 */
Pipe CreatePipe(float x) {
  // nilai default
  Pipe pipe{};
  pipe.position = {x, 0.0f};
  pipe.width = PIPE_WIDTH;
  pipe.speed = PIPE_SPEED;
  pipe.color = DARKGREEN;
  pipe.passed = false;

  // pas data dasar dibuat, generate tinggi pipa

  ResetPipe(pipe, x);

  return pipe;
}

/**
 * @brief reset pipa dan buat celah baru acaak
 *
 * function ini dipakai pas pipa keluar
 *
 * @param pipe Pipa yang direset
 * @param x Posisi X baru pipa
 */
void ResetPipe(Pipe &pipe, float x) {
  pipe.position.x = x;    // posisi horizontal baru
  pipe.position.y = 0.0f; // pipa atas dimulai dari atas layar

  // set tinggi pipa acak
  pipe.topHeight = static_cast<int>(GetRandomValue(
      static_cast<int>(PIPE_MIN_TOP), static_cast<int>(PIPE_MAX_TOP)));

  // set pipa bawah dg celah
  pipe.bottomY = pipe.topHeight + PIPE_GAP;

  // tinggi pipa bawah dari sisa layar dari bottomY sampai bawah
  pipe.bottomHeight = SCREEN_HEIGHT - pipe.bottomY;

  // pipa baru belum dilewati player
  pipe.passed = false;
}

/**
 * @brief reset game ke awal
 *
 * function dipakai saat player mau restart.
 *
 * yang direset:
 * - posisi
 * - speed
 * - posisi pipa
 * - tinggi pipa
 * - status score
 *
 * @param Ball Objek player.
 * @param pipeA Pipa pertama.
 * @param pipeB Pipa kedua.
 * @param score Score game.
 */
void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB, int &score) {
  // set Ball ke awal game;
  ball = CreateBall();

  // reset kedua pipa ke ujung kanan lagi
  ResetPipe(pipeA, SCREEN_WIDTH);
  ResetPipe(pipeB, SCREEN_WIDTH + PIPE_DISTANCE); // ada jarak

  // reset score ke awal
  score = 0;
}

/**
 * @brief Set gerakan dan physic Ball
 *
 * yang diset:
 * - input lompatan
 * - gravitasi
 * - batas atas dan bawah layar
 *
 * @param Ball Objek player
 * @param dt Delta Time
 */
void UpdateBall(Ball &ball, float dt) {
  // INPUT LOMPAT
  if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) {
    ball.velocityY = JUMP_FORCE; // lompat tolak gravitasi
  }

  // GRAVITASI
  /**
   * gravitasi tambah velocityY perframe
   * JUMP_FORCE negatif = gerak keatas (lompat)
   */
  ball.velocityY += GRAVITY * dt;

  // UPDATE POSISI
  // rumus: posisi = posisi + kecepatan * waktu
  ball.position.y += ball.velocityY * dt;

  // CLAMP BALL (batas atas dan bawah )
  if (ball.position.y - ball.radius < 0.0f) {
    ball.position.y = ball.radius; // batas atas agar tidak keluar layar
    ball.velocityY = 0.0f;         // set diam jangan gerak
  }

  if (ball.position.y + ball.radius > SCREEN_HEIGHT) {
    ball.position.y =
        SCREEN_HEIGHT - ball.radius; // batas bawah agar tidak jatuh
    ball.velocityY = 0.0f;           // diam
  }
}

/**
 * @brief Posisi baru kedua pipa
 *
 * pipa bergerak dari kanan ke kiri
 *
 * ketika pipa keluar dari layar pipa tsb pindah ke belakang pipa lain
 * pipa yg dipindahkan dapat tinggi acak
 *
 * @param pipeA Pipa pertama.
 * @param pipeB Pipa kedua.
 * @param dt Delta Time
 */
void UpdatePipes(Pipe &pipeA, Pipe &pipeB, float dt) {
  // GERAKAN PIPA
  /**
   * Karena X makin kecil maka gerak ke kiri
   * dengan rumus yg sama x-=speed*dt
   */
  pipeA.position.x -= pipeA.speed * dt;
  pipeB.position.x -= pipeB.speed * dt;

  // RECYCLING PIPA A
  if (pipeA.position.x + pipeA.width < 0.0f) { // diperbaiki dari > jadi <
    ResetPipe(pipeA,
              pipeB.position.x +
                  PIPE_DISTANCE); // reset posisi ke belakang pipa selanjutnya
  }
  if (pipeB.position.x + pipeB.width < 0.0f) { // diperbaiki dari > jadi <
    ResetPipe(pipeB,
              pipeA.position.x + PIPE_DISTANCE); // set ke belakang pipa lain
  }
}

/**
 * @brief cek Ball collision dengan pipa
 *
 * @param Ball Objek player
 * @param pipe Pipa yang dicek
 *
 * @return true jika collision
 * @return false jika tidak collision
 */
bool CheckBallPipeCollision(const Ball &ball, const Pipe &pipe) {
  /**
   * satu pia terdiri dari pipa atas dan pipa bawah
   * buat dua kotak virtual buat collision
   */

  Rectangle topPipe = {pipe.position.x, pipe.position.y, pipe.width,
                       pipe.topHeight}; // pipa atas
  Rectangle bottomPipe = {pipe.position.x, pipe.bottomY, pipe.width,
                          pipe.bottomHeight}; // pipa bawah

  // collision pipa atas
  const bool hitTop =
      CheckCollisionCircleRec(ball.position, ball.radius, topPipe);

  // collision pipa bawah
  const bool hitBottom =
      CheckCollisionCircleRec(ball.position, ball.radius, bottomPipe);

  /*
   * cek kalau kena salah satu pipa
   * berarti ada collision
   */

  return hitTop || hitBottom;
}

/**
 * @brief Update score jika Ball melewati pipa
 *
 * Setiap pipa cuman dapat satu score
 *
 * @param Ball Objek
 * @param pipe Pipa yang dicek
 * @param score Skor sekarang
 */
void UpdateScore(const Ball &ball, Pipe &pipe, int &score) {
  // kalau Ball melewati pipa
  const bool passedPipe = pipe.position.x + pipe.width < ball.position.x;

  if (!pipe.passed && passedPipe) {
    ++score; // naikkan skor
    pipe.passed = true;
  }
}

/**
 * @brief Gambar isi game
 *
 * Yang digambar:
 * - Ball
 * - pipa
 * - score
 * - intruksi
 * - layar Game Over
 *
 * @param biji Objek player
 * @param pipeA Pipa pertama
 * @param pipeB Pipa kedua
 * @param score Skor sekarang
 * @partam state Kondisi game sekarang
 */
void DrawGame(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB, int score,
              GameState state) {
  /**
   * semua gambar ada diantara:
   *     BeginDrawing()
   *     _______________tempat semua gambar
   *     EndDRawing()
   */

  // Mulai gambar
  BeginDrawing();

  // BACKGROUND GAME
  ClearBackground(SKYBLUE);

  // PIPA A
  DrawRectangleRec(
      {pipeA.position.x, pipeA.position.y, pipeA.width, pipeA.topHeight},
      pipeA.color); // atas
  DrawRectangleRec(
      {pipeA.position.x, pipeA.bottomY, pipeA.width, pipeA.bottomHeight},
      pipeA.color); // bawah

  // PIPA B
  DrawRectangleRec(
      {pipeB.position.x, pipeB.position.y, pipeB.width, pipeB.topHeight},
      pipeB.color); // atas
  DrawRectangleRec(
      {pipeB.position.x, pipeB.bottomY, pipeB.width, pipeB.bottomHeight},
      pipeB.color); // bawah

  // Ball
  DrawCircleV(ball.position, ball.radius, ball.color); // Ball
  DrawCircleLines(static_cast<int>(ball.position.x),
                  static_cast<int>(ball.position.y), ball.radius,
                  ball.color); // outline Ball

  // SCORE
  DrawText(TextFormat("SCORE: %d", score), 20, 30, 30, WHITE);

  // UI PLAYING
  if (state == GameState::PLAYING) {
    DrawText("SPACE / UP = JUMP!", 95, SCREEN_HEIGHT - 60, 20, WHITE);
  }

  // UI GAME OVER
  if (state == GameState::GAME_OVER) {

    // Layer gelap (diperbaiki agar game di belakang tetap terlihat samar)
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5f));
    DrawText("GAME OVER", 105, 300, 40, RED);
    DrawText(TextFormat("SCORE: %d", score), 160, 360, 25, WHITE);
    DrawText("PRESS SPACE TO RESTART", 70, 430, 20, WHITE);
  }

  // Akhiri gambar
  EndDrawing();
}

//================================================
// MAIN
// ===============================================
int main() {
  // INISIALISASI WINDOW
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Ball");

  /*
   * Target FPS.
   * Hanya target FPS tp saat game berjalan pakai delta time bukan FPS
   */
  SetTargetFPS(120);

  //===============================================
  // RANDOM SEED
  //=============================================================

  /*
   * Buat seed random
   * Biar value dari GetRandomValue()
   * gk ikut pola yg sama tiap program dirun
   */
  SetRandomSeed(static_cast<unsigned int>(GetTime() * 1000));

  // ======================
  // OBJEK GAME
  // ===========================
  Ball ball = CreateBall();                              // Ball user
  Pipe pipeA = CreatePipe(SCREEN_WIDTH);                 // Pipa pertama
  Pipe pipeB = CreatePipe(SCREEN_WIDTH + PIPE_DISTANCE); // Pipa kedua

  // SCORE AWAL
  int score = 0;

  // set player ke PLAYING
  GameState gameState = GameState::PLAYING;

  //=================================================
  // GAME LOOP
  //=================================================

  /**
   * Alur game:
   * INPUT -> UPDATE -> COLLISION -> DRAW -> REPEAT
   *
   * loop terus berjalan sampai player keluar WIndowShouldCLose()
   * menjadi true
   */

  while (!WindowShouldClose()) {
    // -----------------------------
    // DELTA TIME
    // ----------------------------
    //
    // dipakai untuk physics.
    float dt = GetFrameTime();

    /*
     * batasi delta time buat cegah physic
     * mengalami lompatan besar pas lag
     */

    if (dt > MAX_DT)
      dt = MAX_DT;

    // ------------------------
    // STATE: PLAYING
    // --------------------

    if (gameState == GameState::PLAYING) {

      // update Ball
      UpdateBall(ball, dt);

      // update pipa
      UpdatePipes(pipeA, pipeB, dt);

      // update score
      UpdateScore(ball, pipeA, score);
      UpdateScore(ball, pipeB, score);

      // -------------------------
      // cek collision kedua pipa
      // --------------------
      const bool hitPipe = CheckBallPipeCollision(ball, pipeA) ||
                           CheckBallPipeCollision(ball, pipeB);

      /**
       * karena Ball punya radius jadi cek dg ukurannya jangan
       *    Ball.position.y + Ball.radius
       * jangan sampai keliru atas dan bawah
       */

      const bool hitFloor =
          ball.position.y + ball.radius >= SCREEN_HEIGHT;            // lantai
      const bool hitCeiling = ball.position.y - ball.radius <= 0.0f; // atap

      // --------------------------------
      // GAME OVER
      // ---------------------------

      if (hitPipe || hitFloor || hitCeiling)
        gameState = GameState::GAME_OVER;
    } else if (gameState == GameState::GAME_OVER) {

      /**
       * pas game over:
       * - Ball tidak update
       * - pipa diam
       * - score tidak bertambah
       *
       * tunggu player set restrat
       */

      if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        // reset ke awal
        ResetGame(ball, pipeA, pipeB, score);

        // kembali ke game
        gameState = GameState::PLAYING;
      }
    }

    // -----------------------
    // DRAW
    // -------------------------------
    /**
     * setelah data game update
     * gambar keadaan terbaru ke layar
     */
    DrawGame(ball, pipeA, pipeB, score, gameState);
  }

  // ================
  // CLEANUP
  // ===============

  // tutup program
  CloseWindow();

  return 0;
}
