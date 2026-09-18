/*
 * @file flappy-ball.cpp
 * @brief Game sederhana "Flappy Ball" pake Raylib.
 * @author Annur Musthofa (Nuy)
 * @version 2.0
 * @date 12-09-2026
 */

#include "raylib.h"
#include <algorithm>

// ---------------------
// GAME CONFIG
// ------------------------

namespace Config {

inline constexpr int SCREEN_WIDTH = 450;  // lebar layar
inline constexpr int SCREEN_HEIGHT = 800; // tinggi layar

inline constexpr float BALL_RADIUS = 20.0f;  // besar bola
inline constexpr float GRAVITY = 1200.0f;    // gravitasi
inline constexpr float JUMP_FORCE = -400.0f; // gaya lompat

inline constexpr float PIPE_WIDTH = 60.0f;     // lebar pipa
inline constexpr float PIPE_GAP = 200.0f;      // celah pipa
inline constexpr float PIPE_DISTANCE = 300.0f; // jarak pipa
inline constexpr float PIPE_SPEED = 300.0f;    // kecepatan pipa

inline constexpr int PIPE_MIN_TOP = 100; // max tinggi pipa
inline constexpr int PIPE_MAX_TOP = 350; // min

// batas delta time biar physics gak loncat terlalu jauh
// pas game lagi lag atau frame drop
inline constexpr float MAX_DT = 1.0f / 30.0f;

} // namespace Config

// -----------------------------
// GAME STATE
// -------------------------

/**
 * @brief Menentukan keadaan game sekarang.
 */
enum class GameState { Playing, GameOver };

// ============================================================
// GAME OBJECTS
// ============================================================

/**
 * @brief Menyimpan data yang dimiliki Ball.
 */
struct Ball {
  Vector2 position{};
  float velocityY{};
  float radius{};
  Color color{};
};

/**
 * @brief Menyimpan data yang dimiliki pipa.
 */
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
// BALL
// ============================================================

/**
 * @brief Membuat Ball dengan nilai awal.
 *
 * Ball dimulai dari tengah layar dengan velocity 0.
 *
 * @return Ball yang sudah dibuat dan diisi nilai awal.
 */
Ball CreateBall() {
  return {
      .position = {Config::SCREEN_WIDTH / 2.0f, Config::SCREEN_HEIGHT / 2.0f},
      .velocityY = 0.0f,
      .radius = Config::BALL_RADIUS,
      .color = YELLOW};
}

/**
 * @brief Update gerakan dan physics Ball.
 *
 * Input jump akan mengubah velocity Ball ke arah atas,
 * lalu gravitasi akan menarik Ball kembali ke bawah.
 *
 * @param ball Ball yang akan di-update.
 * @param dt Delta time dari frame sebelumnya.
 */
void UpdateBall(Ball &ball, float dt) {
  // INPUT LOMPAT
  // Tekan Space atau Arrow Up buat lompat.
  if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)) {
    ball.velocityY = Config::JUMP_FORCE;
  }

  // GRAVITASI
  // Gravitasi nambah velocity ke arah bawah setiap frame.
  ball.velocityY += Config::GRAVITY * dt;

  // UPDATE POSISI
  // Rumusnya: posisi = posisi + velocity * waktu.
  ball.position.y += ball.velocityY * dt;

  // CLAMP BALL
  // Biar Ball gak bisa keluar dari bagian atas layar.
  if (ball.position.y - ball.radius < 0.0f) {
    ball.position.y = ball.radius;
    ball.velocityY = 0.0f;
  }

  // Biar Ball gak bisa keluar dari bagian bawah layar.
  if (ball.position.y + ball.radius > Config::SCREEN_HEIGHT) {
    ball.position.y = Config::SCREEN_HEIGHT - ball.radius;
    ball.velocityY = 0.0f;
  }
}

// ============================================================
// PIPE
// ============================================================

/**
 * @brief Reset pipa ke posisi baru dan bikin celah baru secara acak.
 *
 * Function ini dipakai saat pipa pertama kali dibuat atau
 * saat pipa yang lama sudah keluar dari layar.
 *
 * @param pipe Pipa yang akan di-reset.
 * @param x Posisi X baru untuk pipa.
 */
void ResetPipe(Pipe &pipe, float x) {
  pipe.position = {x, 0.0f};

  // Set tinggi pipa atas secara acak.
  // GetRandomValue() ngasih nilai int,
  // sedangkan topHeight disimpan sebagai float.
  // Jadi conversion-nya dibuat secara jelas pakai static_cast.
  pipe.topHeight = static_cast<float>(
      GetRandomValue(Config::PIPE_MIN_TOP, Config::PIPE_MAX_TOP));

  // Pipa bawah dimulai setelah celah.
  pipe.bottomY = pipe.topHeight + Config::PIPE_GAP;

  // Tinggi pipa bawah dihitung dari sisa layar.
  pipe.bottomHeight = Config::SCREEN_HEIGHT - pipe.bottomY;

  // Pipa baru belum dilewati Ball.
  pipe.passed = false;
}

/**
 * @brief Membuat pipa dengan nilai awal.
 *
 * @param x Posisi X awal pipa.
 *
 * @return Pipe yang sudah dibuat dan diisi nilai awal.
 */
Pipe CreatePipe(float x) {
  // Isi data dasar pipa dulu.
  Pipe pipe{.width = Config::PIPE_WIDTH,
            .speed = Config::PIPE_SPEED,
            .color = DARKGREEN};

  // Setelah data dasarnya ada, buat posisi dan celah pipa.
  ResetPipe(pipe, x);

  return pipe;
}

/**
 * @brief Update posisi kedua pipa.
 *
 * Pipa bergerak dari kanan ke kiri.
 * Kalau pipa sudah keluar layar, pipa dipindah ke belakang
 * pipa yang satunya dan dibuatkan celah baru.
 *
 * @param pipeA Pipa pertama.
 * @param pipeB Pipa kedua.
 * @param dt Delta time dari frame sebelumnya.
 */
void UpdatePipes(Pipe &pipeA, Pipe &pipeB, float dt) {
  // Hitung perpindahan pipa berdasarkan speed dan dt.
  const float movement = Config::PIPE_SPEED * dt;

  // X makin kecil berarti pipa bergerak ke kiri.
  pipeA.position.x -= movement;
  pipeB.position.x -= movement;

  // Kalau pipa A sudah keluar layar,
  // pindahkan ke belakang pipa B.
  if (pipeA.position.x + pipeA.width < 0.0f) {
    ResetPipe(pipeA, pipeB.position.x + Config::PIPE_DISTANCE);
  }

  // Kalau pipa B sudah keluar layar,
  // pindahkan ke belakang pipa A.
  if (pipeB.position.x + pipeB.width < 0.0f) {
    ResetPipe(pipeB, pipeA.position.x + Config::PIPE_DISTANCE);
  }
}

// ============================================================
// COLLISION & SCORE
// ============================================================

/**
 * @brief Cek apakah Ball nabrak salah satu bagian pipa.
 *
 * Satu pipa terdiri dari dua kotak collision:
 * pipa atas dan pipa bawah.
 *
 * @param ball Ball yang dicek.
 * @param pipe Pipa yang dicek.
 *
 * @return true kalau Ball nabrak pipa.
 * @return false kalau Ball gak nabrak pipa.
 */
bool CheckBallPipeCollision(const Ball &ball, const Pipe &pipe) {
  // Buat kotak collision untuk pipa atas.
  const Rectangle topPipe{pipe.position.x, pipe.position.y, pipe.width,
                          pipe.topHeight};

  // Buat kotak collision untuk pipa bawah.
  const Rectangle bottomPipe{pipe.position.x, pipe.bottomY, pipe.width,
                             pipe.bottomHeight};

  // Cek collision Ball dengan pipa atas.
  const bool hitTop =
      CheckCollisionCircleRec(ball.position, ball.radius, topPipe);

  // Cek collision Ball dengan pipa bawah.
  const bool hitBottom =
      CheckCollisionCircleRec(ball.position, ball.radius, bottomPipe);

  // Kalau kena salah satu berarti collision.
  return hitTop || hitBottom;
}

/**
 * @brief Update score saat Ball berhasil melewati pipa.
 *
 * Setiap pipa cuma bisa memberikan score satu kali.
 *
 * @param ball Ball yang dipakai untuk cek apakah pipa sudah dilewati.
 * @param pipe Pipa yang sedang dicek.
 * @param score Score game sekarang.
 */
void UpdateScore(const Ball &ball, Pipe &pipe, int &score) {
  // Kalau ujung kanan pipa sudah melewati posisi Ball,
  // berarti Ball sudah berhasil melewati pipa.
  const bool passedPipe = pipe.position.x + pipe.width < ball.position.x;

  if (!pipe.passed && passedPipe) {
    ++score;

    // Tandai supaya pipa ini gak kasih score lagi.
    pipe.passed = true;
  }
}

// ============================================================
// GAME LOGIC
// ============================================================

/**
 * @brief Reset semua data game ke kondisi awal.
 *
 * Yang di-reset:
 * - posisi Ball
 * - posisi dan celah pipa
 * - score
 *
 * @param ball Ball yang akan di-reset.
 * @param pipeA Pipa pertama yang akan di-reset.
 * @param pipeB Pipa kedua yang akan di-reset.
 * @param score Score yang akan dikembalikan ke 0.
 */
void ResetGame(Ball &ball, Pipe &pipeA, Pipe &pipeB, int &score) {
  // Balikin Ball ke kondisi awal.
  ball = CreateBall();

  // Balikin kedua pipa ke posisi awal.
  ResetPipe(pipeA, Config::SCREEN_WIDTH);

  ResetPipe(pipeB, Config::SCREEN_WIDTH + Config::PIPE_DISTANCE);

  // Score mulai lagi dari 0.
  score = 0;
}

/**
 * @brief Cek apakah game sudah masuk kondisi Game Over.
 *
 * Game Over terjadi kalau Ball:
 * - nabrak pipa
 * - kena lantai
 * - kena bagian atas layar
 *
 * @param ball Ball yang dicek.
 * @param pipeA Pipa pertama.
 * @param pipeB Pipa kedua.
 *
 * @return true kalau kondisi Game Over terpenuhi.
 * @return false kalau game masih bisa dimainkan.
 */
bool IsGameOver(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB) {
  // Cek collision dengan kedua pipa.
  const bool hitPipe = CheckBallPipeCollision(ball, pipeA) ||
                       CheckBallPipeCollision(ball, pipeB);

  // Cek kalau Ball sudah menyentuh lantai.
  const bool hitFloor = ball.position.y + ball.radius >= Config::SCREEN_HEIGHT;

  // Cek kalau Ball sudah menyentuh bagian atas layar.
  const bool hitCeiling = ball.position.y - ball.radius <= 0.0f;

  // Kalau salah satu kondisi terpenuhi, game over.
  return hitPipe || hitFloor || hitCeiling;
}

// ============================================================
// DRAW
// ============================================================

/**
 * @brief Menggambar satu pipa ke layar.
 *
 * Satu pipa digambar sebagai dua bagian:
 * pipa atas dan pipa bawah.
 *
 * @param pipe Pipa yang akan digambar.
 */
void DrawPipe(const Pipe &pipe) {
  // Gambar pipa atas.
  DrawRectangleRec(
      {pipe.position.x, pipe.position.y, pipe.width, pipe.topHeight},
      pipe.color);

  // Gambar pipa bawah.
  DrawRectangleRec(
      {pipe.position.x, pipe.bottomY, pipe.width, pipe.bottomHeight},
      pipe.color);
}

/**
 * @brief Menggambar Ball ke layar.
 *
 * Ball digambar sebagai lingkaran dan outline.
 *
 * @param ball Ball yang akan digambar.
 */
void DrawBall(const Ball &ball) {
  // Gambar Ball.
  DrawCircleV(ball.position, ball.radius, ball.color);

  // Gambar outline Ball.
  DrawCircleLines(static_cast<int>(ball.position.x),
                  static_cast<int>(ball.position.y), ball.radius, ball.color);
}

/**
 * @brief Menggambar UI berdasarkan keadaan game.
 *
 * Saat Playing, UI menampilkan score dan instruksi jump.
 * Saat Game Over, UI menampilkan layar Game Over dan instruksi restart.
 *
 * @param score Score game sekarang.
 * @param state Keadaan game sekarang.
 */
void DrawUI(int score, GameState state) {
  // SCORE
  // Score selalu ditampilkan.
  DrawText(TextFormat("SCORE: %d", score), 20, 30, 30, WHITE);

  // UI PLAYING
  if (state == GameState::Playing) {
    DrawText("SPACE / UP = JUMP!", 95, Config::SCREEN_HEIGHT - 60, 20, WHITE);

    return;
  }

  // UI GAME OVER

  // Layer gelap biar game di belakang masih kelihatan samar.
  DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT,
                Fade(BLACK, 0.5f));

  DrawText("GAME OVER", 105, 300, 40, RED);

  DrawText(TextFormat("SCORE: %d", score), 160, 360, 25, WHITE);

  DrawText("PRESS SPACE TO RESTART", 70, 430, 20, WHITE);
}

/**
 * @brief Menggambar semua isi game ke layar.
 *
 * Urutan gambar:
 * - background
 * - pipa
 * - Ball
 * - UI
 *
 * @param ball Ball yang akan digambar.
 * @param pipeA Pipa pertama.
 * @param pipeB Pipa kedua.
 * @param score Score game sekarang.
 * @param state Keadaan game sekarang.
 */
void DrawGame(const Ball &ball, const Pipe &pipeA, const Pipe &pipeB, int score,
              GameState state) {
  // Mulai proses drawing.
  BeginDrawing();

  // Background game.
  ClearBackground(SKYBLUE);

  // Gambar object game.
  DrawPipe(pipeA);
  DrawPipe(pipeB);
  DrawBall(ball);

  // Gambar UI paling akhir biar berada di atas object.
  DrawUI(score, state);

  // Selesai drawing.
  EndDrawing();
}

// ============================================================
// MAIN
// ============================================================

/**
 * @brief Entry point program dan game loop utama.
 *
 * Alur game:
 * INPUT → UPDATE → COLLISION → DRAW → REPEAT
 *
 * Loop berjalan terus sampai window ditutup.
 *
 * @return 0 kalau program selesai dengan normal.
 */
int main() {
  // ========================================================
  // INISIALISASI WINDOW
  // ========================================================

  InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "Flappy Ball");

  // Target FPS.
  // Physics tetap pakai delta time, jadi tidak bergantung
  // langsung pada jumlah FPS.
  SetTargetFPS(120);

  // ========================================================
  // RANDOM SEED
  // ========================================================

  // Pakai waktu sekarang sebagai seed random.
  // Biar pola pipa gak sama setiap program dijalankan.
  SetRandomSeed(static_cast<unsigned int>(GetTime() * 1000));

  // ========================================================
  // BUAT OBJECT GAME
  // ========================================================

  Ball ball = CreateBall();

  Pipe pipeA = CreatePipe(Config::SCREEN_WIDTH);

  Pipe pipeB = CreatePipe(Config::SCREEN_WIDTH + Config::PIPE_DISTANCE);

  // Score awal.
  int score = 0;

  // Game dimulai dalam keadaan Playing.
  GameState gameState = GameState::Playing;

  // ========================================================
  // GAME LOOP
  // ========================================================

  while (!WindowShouldClose()) {
    // DELTA TIME
    //
    // GetFrameTime() ngasih waktu yang berlalu sejak frame
    // sebelumnya.
    //
    // Dipakai buat physics supaya gerakan tetap konsisten
    // walaupun FPS berubah-ubah.
    //
    // dt juga dibatasi supaya physics gak melonjak terlalu
    // jauh kalau tiba-tiba terjadi frame drop.
    const float dt = std::min(GetFrameTime(), Config::MAX_DT);

    // ====================================================
    // PLAYING
    // ====================================================

    if (gameState == GameState::Playing) {
      // Update Ball dan physics-nya.
      UpdateBall(ball, dt);

      // Update posisi pipa.
      UpdatePipes(pipeA, pipeB, dt);

      // Cek apakah Ball sudah melewati pipa.
      UpdateScore(ball, pipeA, score);
      UpdateScore(ball, pipeB, score);

      // Cek apakah player sudah mati.
      if (IsGameOver(ball, pipeA, pipeB)) {
        gameState = GameState::GameOver;
      }
    }

    // ====================================================
    // GAME OVER
    // ====================================================

    if (gameState == GameState::GameOver) {
      // Tekan Space atau Enter buat restart.
      if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        ResetGame(ball, pipeA, pipeB, score);

        // Balik lagi ke Playing.
        gameState = GameState::Playing;
      }
    }

    // ====================================================
    // DRAW
    // ====================================================

    DrawGame(ball, pipeA, pipeB, score, gameState);
  }

  // ========================================================
  // CLEANUP
  // ========================================================

  // Tutup window dan bersihkan resource Raylib.
  CloseWindow();

  return 0;
}
