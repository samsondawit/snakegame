# Snake Game Using Raylib: Project Report

**Author:** Samson Dawit Bekele  
**Department:** Computer Science, Al-Farabi Kazakh National University  
**Course:** Languages and Programming Technologies  
**Instructor:** Mr. Makhmut Ye.  
**Date:** September 27, 2023

---

## Introduction

In the developed Snake game, the Raylib library has been employed, renowned for its simplicity and efficiency in game development. The game showcases an advanced version of the classic Snake game with additional features and gameplay elements. Valuable insights and code components were obtained from the GitHub repository by (educ8s, 2023) and YouTube video titled “Creating Simple Snake Game in C++” (ProgrammingKnowledge, 2021).

---

## Technologies

### C++

As the primary programming language, C++ serves as the foundation for developing the game's logic and mechanics. C++ 17 with the MinGW g++ compiler is used. ([DevDocs, 2023](https://devdocs.io/cpp/))

### Raylib

A C library used for graphics and input handling, suitable for game development. Raylib is open-source and cross-platform, accessible on various operating systems. Its goal is to provide a straightforward framework for creating 2D and 3D games. ([Raylib, n.d](https://www.raylib.com/))

### Deque

A C++ data structure used to manage the snake's body as a queue, enabling efficient control and manipulation of the snake's segments. ([Double-ended queue, 2023](https://en.wikipedia.org/wiki/Double-ended_queue))

### Raymath

A library for mathematical operations in Raylib, used for vector calculations crucial in movement and collision detection. ([Raylib, n.d](https://www.raylib.com/))

---

## Functionality

The Snake Game project is designed with an object-oriented approach, encapsulating game entities and functionalities. OOP characteristics such as inheritance, overloading, and encapsulation are used.

### Snake Class

The Snake class is responsible for operations like drawing, updating position, and resetting. The snake’s body is managed as a deque of `Vector2` objects.

### RandomSnake Class

Inherited from the Snake class, this class moves randomly and appears when the player's score is 5 or above.

### Food Class

Responsible for generating and rendering food on the game board, ensuring that food items do not overlap the snake's body.

### SpecialFood Class

A subclass of the Food class, represents a special food item that offers extra lives under certain conditions.

### Game Class

Serves as the central hub for managing the game state, updating entities, and handling collisions. Incorporates a scoring mechanism and audio feedback, and controls the game's difficulty level.

---

## Features

### Pausing, Restarting, Exiting

The game includes pause functionality with the "P" key and can be quit anytime with the "ESC" key. On game over (lives <= 0), the player is prompted to restart or quit.

### Increasing Difficulty

As players progress, the game's difficulty increases with a faster-moving snake and the appearance of a random snake.

### Adding Lives

Special food represented by a heart appears under certain conditions, increasing the snake's lives by 2 when consumed.

---

## Planned Future Improvements

### Intelligent AI Opponent

Integration of Q-learning or deep reinforcement learning for a smart AI opponent is planned.

### More Random Snakes

A menu to choose difficulty levels and game types is planned for future updates.

### Customization

Allowing user customization for parameters like speed, size, colors is envisioned.

### Advanced Score and High Score Tracking

Implementing a more advanced score system with levels and high score management is planned.

### More Power-Ups and Special Items

Addition of more power-ups and special items beyond the special food is considered.

### Parallel Processing

Improvements in multi-threading and synchronization are planned for future development.

---

## References

- [DevDocs. (2023). C++17 documentation.](https://devdocs.io/cpp/)
- [Double-ended queue. (2023). Wikipedia.](https://en.wikipedia.org/wiki/Double-ended_queue)
- [educ8s. (2023). Cpp-Retro-Snake-Game-with-raylib. GitHub.](https://github.com/educ8s/Cpp-Retro-Snake-Game-with-raylib)
- [Raylib. (n.d). Official website.](https://www.raylib.com/)
- [ProgrammingKnowledge. (2021). Creating Simple Snake Game in C++. YouTube.](https://www.youtube.com/watch?v=AxrQje7V65o)
