# Double Pendulum Audio Visualizer

This project is a creative blend of physics simulation and audio synthesis, utilizing SDL for both graphical rendering and audio processing. The core concept revolves around simulating a double pendulum's motion and translating its dynamic movements into auditory experiences.

## Features

- Real-time double pendulum simulation with accurate physics.
- Audio synthesis that changes based on the pendulum's position, offering a unique way to 'hear' the motion.
- Utilizes SDL2 for graphics and audio, providing a cross-platform solution for visualization and sound generation.

## Requirements

- SDL2: This project relies on the SDL2 library for rendering graphics and handling audio. Ensure you have SDL2 installed on your system.
- C++ Compiler: A modern C++ compiler capable of C++11 or later.

## Setup

1. **Install SDL2:** Make sure SDL2 is installed on your system. You can download it from [SDL's official website](https://www.libsdl.org/download-2.0.php) or use your package manager if you're on Linux.

2. **Compilation:** Use the following command to compile the program, ensuring to link against the SDL2 library.
   
   ```bash
   g++ -o DoublePendulumAudioVisualizer main.cpp `sdl2-config --cflags --libs`

## How It Works

- The simulation calculates the positions of two pendulums connected in series, where the motion of one directly influences the other.
- The graphics part draws the pendulums and traces their path on the screen, creating intricate patterns over time.
- The audio synthesis component generates sound based on the position of the second pendulum, converting its motion into a varying tone. The frequency and waveform can be adjusted to create different sound textures.

## Customization

- **Pendulum Parameters:** Adjust the length, mass, and initial angle of the pendulums to explore different motion patterns.
- **Audio Waveform:** Modify the Func_Callback function to experiment with different waveforms like sine, square, or sawtooth for diverse auditory feedback.
- **Visuals:** Change the rendering settings to customize the appearance, such as colors, line thickness, and trace effects.
