# Superliminal: OpenGL Graphics Engine

   

A custom 3D game engine built from scratch using **C++** and **OpenGL**, designed to replicate the perspective-based puzzle mechanics of the game *Superliminal*. This project features a hand-written **Entity-Component-System (ECS)** architecture, a forward rendering pipeline, and a physics-based interaction system using **ReactPhysics3D**.

-----

## 🚀 Key Features

### 🎨 Graphics & Rendering

  * **Forward Rendering Pipeline**: Custom implementation supporting opaque and transparent passes.
  * **Shader System**: Dynamic GLSL shader management with support for:
      * **Phong/Blinn-Phong Lighting**: Multiple light sources (Directional, Point, Spot).
      * **Post-Processing Effects**: Framebuffer-based pipeline including **Chromatic Aberration**, **Vignette**, **Grayscale**, and **Radial Blur**.
      * **Texture Mapping**: Diffuse, Specular, Roughness, and Emissive mapping.
  * **Skybox Rendering**: Cubemap-based sky sphere implementation.

### ⚙️ Engine Architecture

  * **Entity-Component-System (ECS)**: A cache-friendly, data-oriented architecture for managing game objects.
      * **Entities**: ID-based identifiers.
      * **Components**: POD (Plain Old Data) structs for `Transform`, `MeshRenderer`, `Camera`, `Light`, and `RigidBody`.
      * **Systems**: Logic processors including `ForwardRenderer`, `MovementSystem`, and `PhysicsSystem`.
  * **State Management**: Stack-based state machine handling transitions between `MenuState`, `PlayState`, and various `TestStates`.
  * **Asset Management**: Centralized loader for Meshes (OBJ/GLTF), Textures, and Shaders to prevent resource duplication.

### 🎮 Gameplay Mechanics

  * **Forced Perspective Interaction**: Implemented the core *Superliminal* mechanic where objects scale based on the player's perspective and distance, seamlessly blending optical illusions with 3D physics.
  * **Physics Integration**: Rigid body dynamics and collision detection powered by **ReactPhysics3D**.

-----

## 🛠️ Tech Stack

  * **Language**: C++17
  * **Graphics API**: OpenGL 3.3+
  * **Build System**: CMake
  * **Dependencies**:
      * **GLFW & GLAD**: Windowing and OpenGL context management.
      * **GLM**: Mathematics (Vectors, Matrices, Quaternions).
      * **ImGui**: Immediate mode GUI for debugging and scene inspection.
      * **ReactPhysics3D**: Physics engine for collision and dynamics.
      * **nlohmann/json**: Scene serialization/deserialization.
      * **stb\_image**: Image loading.
      * **tinyobjloader**: 3D model parsing.

-----

## 📂 Project Structure

```
├── assets/                 # Shaders, Models, Textures
│   ├── shaders/            # GLSL Vertex and Fragment shaders
│   └── textures/           # PBR Texture maps
├── config/                 # JSON Scene definitions
├── source/
│   ├── common/
│   │   ├── components/     # ECS Components (Camera, Light, etc.)
│   │   ├── ecs/            # Core ECS implementation (World, Entity)
│   │   ├── systems/        # Logic Systems (Renderer, Physics)
│   │   └── shader/         # Shader compilation and linking utils
│   └── states/             # Game States (Menu, Play, Tests)
├── vendor/                 # Third-party libraries (ImGui, ReactPhysics3D, etc.)
└── CMakeLists.txt          # Build configuration
```

-----

## 🔧 Build & Installation

### Prerequisites

  * **CMake** (3.10 or higher)
  * **C++ Compiler** (GCC, Clang, or MSVC) supporting C++17.
  * **OpenGL** drivers installed on your GPU.

### Build Steps

```bash
# 1. Clone the repository
git clone https://github.com/AbdullahAyman02/CMPS205-Project-Superliminal.git
cd CMPS205-Project-Superliminal

# 2. Create build directory
mkdir build && cd build

# 3. Generate project files
cmake ..

# 4. Build the project
cmake --build . --config Release
```

-----

## 🕹️ Controls

| Input | Action |
| :--- | :--- |
| **W, A, S, D** | Move Camera |
| **Mouse** | Look Around |
| **Left Click** | Grab / Release Object (Superliminal Mechanic) |
| **ESC** | Pause / Open Menu |
| **Shift** | Sprint |

-----

## 📸 Gallery

<img width="727" height="412" alt="image" src="https://github.com/user-attachments/assets/00cabd4a-7e34-4c51-af57-0e2ffd100969" />
<img width="701" height="396" alt="image" src="https://github.com/user-attachments/assets/9afe7dec-c047-4c11-adc1-004bdeb8d9ac" />
<img width="707" height="414" alt="image" src="https://github.com/user-attachments/assets/da671137-759c-4dde-9b98-49bdfb8a4d47" />


-----

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.
