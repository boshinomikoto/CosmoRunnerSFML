
# Project Title

A brief description of what this project does and who it's for

# Cosmo Runner

An arcade space survival game built with C++ and SFML where you pilot a spaceship, dodge incoming threats, manage fuel and try to beat your high score.

---

## Gameplay

You control a spaceship navigating through space. The goal is to survive as long as possible while avoiding meteors and enemy drones. Fuel depletes over time — collect fuel pickups to stay alive. Extra lives occasionally drop as well.

**You lose when:**
- All 3 lives are lost (from meteor or drone collisions)
- Fuel runs out completely

---

## Features

- Smooth ship movement with acceleration and deceleration
- Animated ship exhaust jet
- Meteor shower with randomized size, speed and rotation
- Enemy drones that fly toward the player on spawn
- Fuel management system with a visual indicator
- Extra life pickups
- Score system based on distance traveled and time survived
- Speed display in real time
- Explosion animation on death
- High score saved between sessions
- Pause menu with Resume / Restart / Exit
- Scrolling space background

---

## Controls

| Key | Action |
|---|---|
| W A S D / Arrow Keys | Move ship |
| Escape | Pause |
| Space | Restart after death |

---

## Math & Algorithms

### LERP — smooth movement
Instead of moving the ship instantly at full speed, velocity is interpolated each frame:
```cpp
currentSpeed.x += (targetSpeed.x - currentSpeed.x) * acceleration * dt;
currentSpeed.y += (targetSpeed.y - currentSpeed.y) * acceleration * dt;
```
This gives the ship a natural feeling of weight — it accelerates when a key is pressed and gradually slows down when released.

### Distance tracking — score calculation
Each frame the Euclidean distance between the ship's current and previous position is calculated:
```
distance = sqrt((x2 - x1)² + (y2 - y1)²)
```
The result is added to the score, meaning actively moving the ship earns more points than staying still.

### Enemy direction — vector math
When an enemy drone spawns, it calculates a normalized direction vector toward the player:
```
dir = (playerX - spawnX, playerY - spawnY)
dir = dir / length(dir)
velocity = dir * speed
```
The drone's sprite is also rotated using `atan2` to face the direction it's flying:
```cpp
float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
```

### Fisher-Yates Shuffle — randomized meteors
Each meteor randomly selects a texture from a pool using the Fisher-Yates shuffle algorithm, which guarantees an unbiased uniform distribution — every permutation is equally likely:
```cpp
for (size_t i = array.size() - 1; i > 0; i--) {
    uniform_int_distribution<size_t> distr(0, i);
    size_t j = distr(rng);
    swap(array[i], array[j]);
}
```

### Delta time — frame rate independence
All movement and animation is multiplied by `dt` (time elapsed since last frame), so the game runs at the same speed regardless of frame rate:
```cpp
sprite.move(velocity * dt);
```

### Hitbox shrinking — collision tuning
Raw sprite bounds are intentionally reduced before collision checks to make the game feel fair:
```cpp
float sx = bounds.width * 0.2f;
float sy = bounds.height * 0.2f;
sf::FloatRect hitbox(bounds.left + sx, bounds.top + sy,
                     bounds.width - sx * 2, bounds.height - sy * 2);
```

---

## Architecture

The project is structured around individual classes with dedicated manager classes:

| Class | Responsibility |
|---|---|
| `Player` | Input handling, movement, animation, bounds |
| `ShipDamage` | Plays explosion sprite animation on death |
| `SpaceTrash` | Single meteor — position, rotation, speed, collision |
| `TrashManager` | Spawns and updates a pool of meteors |
| `Enemies` | Single enemy drone — spawn direction, movement, collision |
| `EnemManager` | Controls enemy drones, respawn delay, escape counter |
| `PickupItem` | Base class for all collectible objects |
| `Fuel` | Fuel pickup — inherits from PickupItem |
| `FuelManager` | Manages fuel pickup objects |
| `FuelIndicator` | Animated fuel gauge sprite |
| `AddictiveLive` | Extra life pickup — inherits from PickupItem |
| `AddictiveLiveManager` | Manages extra life pickup objects |
| `Health` | Single heart sprite — show and hide |
| `ViewGameOverTitle` | Game over screen title |
| `PauseMenu` | Pause overlay with hover effects |
| `Records` | Saves and loads high score from file |
| `AlgorithmiCalculations` | Fisher-Yates shuffle, distance tracking, speed, random ranges |
| `Game` | Main loop — ties everything together |

`Fuel` and `AddictiveLive` both inherit from a common `PickupItem` base class since they share identical behavior (spawn, move, collision) and differ only in texture and spawn parameters.

---
[![Coso runner gameplay]([https://youtube.com](https://www.youtube.com/shorts/o-TQfVPCtJY))]([https://youtube.com](https://www.youtube.com/shorts/o-TQfVPCtJY))
---

---

## Built With

- **C++17**
- **SFML 2.x** — window, rendering, audio, input

---

## How to Run

1. Download and extract `CosmoBattle-v1.0.zip` from [Releases](https://github.com/boshinomikoto/CosmoRunnerSFML/releases/tag/v1.0)
2. Launch `SFML_COSMO.exe`
3. No installation required — all DLLs are included

**Requirements:** Windows 10 / 11 (64-bit)



