#pragma once

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

inline constexpr float MAX_DT = 1.0f / 30.0f; // batas delta time

} // namespace Config
