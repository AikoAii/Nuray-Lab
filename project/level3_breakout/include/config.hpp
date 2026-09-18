#pragma once

namespace Config {
inline constexpr int SCREEN_WIDTH = 800;  // lebar
inline constexpr int SCREEN_HEIGHT = 600; // tingi
inline constexpr int TARGET_FPS = 120;    // FPS

inline constexpr float MAX_DT{1.0f / 30.0f}; // batas delta time

inline constexpr int ROWS = 5; // baris
inline constexpr int COLS = 8; // kolom

inline constexpr float BALL_RADIUS{10.0f}; // besar bola
inline constexpr float BALL_SPEED{300.0f}; // kecepatan bola

inline constexpr float PADDLE_SPEED{500.0f}; // kecepatan paddle
inline constexpr float PADDLE_WIDTH{77.5f};  // lebar paddle
inline constexpr float PADDLE_HEIGHT{20.0f}; // tingi paddle

inline constexpr float BRICK_WIDTH{77.5f};  // lebar bata
inline constexpr float BRICK_HEIGHT{25.0f}; // tinggi bata
inline constexpr float PADDING{10.0f};      // jarak antar bata
inline constexpr float OFFSET_X{55.0f};     // batas x
inline constexpr float OFFSET_Y{60.0f};     // batas y

inline int POINT = 10;

} // namespace Config
