# Learning Resources for Achilles Codebase

This document contains Wikipedia links and tutorials for all technologies and concepts used in the Achilles evolutionary simulation.

## **Core Programming Language**

### C++
- **Wikipedia**: [C++ Programming Language](https://en.wikipedia.org/wiki/C%2B%2B)
- **Tutorials**:
  - [Learn OpenGL (C++ basics)](https://learnopengl.com/)
  - [cppreference.com](https://en.cppreference.com/)
  - [GeeksforGeeks C++ Tutorial](https://www.geeksforgeeks.org/cpp-tutorial/)

---

## **Graphics & Rendering**

### OpenGL
- **Wikipedia**: [OpenGL](https://en.wikipedia.org/wiki/OpenGL)
- **Tutorials**:
  - [Learn OpenGL](https://learnopengl.com/) - Comprehensive modern OpenGL tutorial
  - [OpenGL Programming Guide (Red Book)](http://www.opengl-redbook.com/)
  - [OpenGL-Tutorial.org](https://www.opengl-tutorial.org/)

### GLU (OpenGL Utility Library)
- **Wikipedia**: [OpenGL Utility Library](https://en.wikipedia.org/wiki/OpenGL_Utility_Library)

### GLUT (OpenGL Utility Toolkit)
- **Wikipedia**: [OpenGL Utility Toolkit](https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit)

### SDL (Simple DirectMedia Layer)
- **Wikipedia**: [Simple DirectMedia Layer](https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer)
- **Official Tutorials**: [Lazy Foo' Productions - SDL Tutorials](https://lazyfoo.net/tutorials/SDL/)
- **SDL Documentation**: [SDL Wiki](https://wiki.libsdl.org/)

#### SDL2 vs SDL1
- **SDL2 Migration Guide**: [SDL2 Migration Guide](https://wiki.libsdl.org/SDL2/MigrationGuide)
- **Key Differences**:
  - SDL2 uses scancodes for keyboard state (not keycodes)
  - Window creation API changed (`SDL_CreateWindow` vs `SDL_SetVideoMode`)
  - Event handling improvements
  - Better multi-monitor support
- **SDL2 API Reference**: [SDL2 API Documentation](https://wiki.libsdl.org/APIByCategory)

---

## **Build System**

### GNU Autotools (Autoconf, Automake)
- **Wikipedia**: [GNU Autotools](https://en.wikipedia.org/wiki/GNU_Autotools)
- **Tutorials**:
  - [Autotools Tutorial by Alexandre Duret-Lutz](https://www.lrde.epita.fr/~adl/autotools.html)
  - [Autotools Mythbuster](https://autotools.io/)
  - [GNU Automake Manual](https://www.gnu.org/software/automake/manual/)
- **Autoconf Macro Reference**: [Autoconf Macro Index](https://www.gnu.org/software/autoconf/manual/autoconf-2.69/html_node/Macro-Index.html)

### pkg-config
- **Wikipedia**: [pkg-config](https://en.wikipedia.org/wiki/Pkg-config)
- **Official Documentation**: [pkg-config Manual](https://www.freedesktop.org/wiki/Software/pkg-config/)
- **Why it's used**: Automatically detects library installation paths, compiler flags, and linker flags
- **Common usage**: `pkg-config --cflags --libs sdl2` to get SDL2 compilation flags
- **Tutorial**: [Understanding pkg-config](https://people.freedesktop.org/~dbn/pkg-config-guide.html)

### Shell Scripting
- **Wikipedia**: [Shell Script](https://en.wikipedia.org/wiki/Shell_script)
- **Tutorial**: [Advanced Bash-Scripting Guide](https://tldp.org/LDP/abs/html/)

### Build System Architecture (Achilles-specific)
The Achilles build system uses:
- **configure.in**: Autoconf script that detects system libraries (SDL2, OpenGL, GLU) using pkg-config
- **Makefile.am**: Automake template that defines build rules
- **Makefile.in**: Generated template with `@VARIABLE@` substitutions
- **Makefile**: Final generated file with actual paths and flags

**Build Process**:
1. `autoconf` generates `configure` from `configure.in`
2. `automake` generates `Makefile.in` from `Makefile.am`
3. `./configure` detects libraries and generates `Makefile`
4. `make` compiles using the generated `Makefile`

---

## **Neural Networks & Machine Learning**

### Neural Networks (General)
- **Wikipedia**: [Artificial Neural Network](https://en.wikipedia.org/wiki/Artificial_neural_network)
- **Tutorials**:
  - [3Blue1Brown Neural Networks Series](https://www.3blue1brown.com/topics/neural-networks)
  - [Neural Networks and Deep Learning (online book)](http://neuralnetworksanddeeplearning.com/)

### Hebbian Learning
- **Wikipedia (Theory)**: [Hebbian Theory](https://en.wikipedia.org/wiki/Hebbian_theory)
- **Wikipedia (Computational)**: [Hebbian Learning](https://en.wikipedia.org/wiki/Hebbian_learning)
- **Tutorial**: [Hebbian Learning Explained (Scholarpedia)](https://www.scholarpedia.org/article/Hebbian_learning)

---

## **Evolutionary Algorithms**

### Genetic Algorithms
- **Wikipedia**: [Genetic Algorithm](https://en.wikipedia.org/wiki/Genetic_algorithm)
- **Tutorials**:
  - [Introduction to Genetic Algorithms (TutorialsPoint)](https://www.tutorialspoint.com/genetic_algorithms/)
  - [Genetic Algorithms in Plain English](https://www.whitman.edu/Documents/Academics/Mathematics/2014/carrjk.pdf)

### Evolutionary Algorithms (General)
- **Wikipedia**: [Evolutionary Algorithm](https://en.wikipedia.org/wiki/Evolutionary_algorithm)
- **Wikipedia**: [Evolutionary Computation](https://en.wikipedia.org/wiki/Evolutionary_computation)

### Natural Selection & Evolution
- **Wikipedia**: [Natural Selection](https://en.wikipedia.org/wiki/Natural_selection)
- **Wikipedia**: [Evolution](https://en.wikipedia.org/wiki/Evolution)

---

## **Algorithms & Data Structures**

### Quicksort
- **Wikipedia**: [Quicksort](https://en.wikipedia.org/wiki/Quicksort)
- **Visual Tutorial**: [Visualgo - Sorting Algorithms](https://visualgo.net/en/sorting)

### Vector (Data Structure)
- **Wikipedia**: [Dynamic Array](https://en.wikipedia.org/wiki/Dynamic_array)
- **C++ Reference**: [std::vector](https://en.cppreference.com/w/cpp/container/vector)

### Stack (Data Structure)
- **Wikipedia**: [Stack (Abstract Data Type)](https://en.wikipedia.org/wiki/Stack_(abstract_data_type))
- **C++ Reference**: [std::stack](https://en.cppreference.com/w/cpp/container/stack)

---

## **3D Mathematics**

### Vector Mathematics
- **Wikipedia**: [Vector (Mathematics and Physics)](https://en.wikipedia.org/wiki/Vector_(mathematics_and_physics))
- **Tutorial**: [3D Math Primer for Graphics and Game Development](https://gamemath.com/)

### 3D Rotation & Angles
- **Wikipedia**: [Rotation (Mathematics)](https://en.wikipedia.org/wiki/Rotation_(mathematics))
- **Wikipedia**: [Euler Angles](https://en.wikipedia.org/wiki/Euler_angles)

### 3D Computer Graphics
- **Wikipedia**: [3D Computer Graphics](https://en.wikipedia.org/wiki/3D_computer_graphics)
- **Wikipedia**: [Transformation Matrix](https://en.wikipedia.org/wiki/Transformation_matrix)

---

## **Game Development Concepts**

### Game Loop
- **Wikipedia**: [Game Programming - Game Loop](https://en.wikipedia.org/wiki/Game_programming#Game_loop)
- **Tutorial**: [Game Loop Pattern (Game Programming Patterns)](https://gameprogrammingpatterns.com/game-loop.html)

### Collision Detection
- **Wikipedia**: [Collision Detection](https://en.wikipedia.org/wiki/Collision_detection)
- **Tutorial**: [Real-Time Collision Detection (book)](http://www.realtimecollisiondetection.net/)

### Spatial Data Structures
- **Wikipedia**: [Spatial Database](https://en.wikipedia.org/wiki/Spatial_database)
- **Wikipedia**: [Minimum Bounding Box](https://en.wikipedia.org/wiki/Minimum_bounding_box)

---

## **Recommended Learning Order**

1. **C++ Basics** - Classes, pointers, STL containers
2. **3D Mathematics** - Vectors, angles, transformations
3. **OpenGL Basics** - Rendering, transformations
4. **SDL** - Window management, input handling
5. **Neural Networks** - Feedforward networks, backpropagation
6. **Hebbian Learning** - Specific learning algorithm used
7. **Genetic Algorithms** - Crossover, mutation, selection
8. **Game Loop** - Simulation concepts
9. **Autotools** - Build system (if modifying build)
10. **pkg-config** - Library detection and flag management
11. **SDL2 Migration** - Understanding differences from SDL1 (if working with input handling)

---

## **Additional Resources**

- **OpenGL Mathematics (GLM)**: [GLM Library](https://github.com/g-truc/glm) - Useful for 3D math
- **Game Programming Patterns**: [Game Programming Patterns Book](https://gameprogrammingpatterns.com/)
- **Real-Time Rendering**: [Real-Time Rendering Website](https://www.realtimerendering.com/)

### Build System Resources
- **Autotools Introduction**: [Introduction to the Autotools](https://www.lrde.epita.fr/~adl/autotools.html)
- **Writing configure.ac**: [Autoconf Tutorial](https://www.gnu.org/software/autoconf/manual/autoconf.html)
- **pkg-config Usage**: [pkg-config Guide](https://people.freedesktop.org/~dbn/pkg-config-guide.html)

---

## **Quick Reference**

### Essential Concepts for Understanding Achilles:
- **Object-Oriented Programming** in C++
- **3D Vector Mathematics** (position, direction, distance)
- **Neural Network Architecture** (layers, neurons, weights)
- **Hebbian Learning Rule** (weight updates based on correlation)
- **Genetic Algorithm Operations** (crossover, mutation, selection)
- **OpenGL Rendering Pipeline** (vertices, transformations, drawing)
- **SDL2 Event Handling** (keyboard input using scancodes, window management)
- **Game Loop Pattern** (update, render, repeat)
- **pkg-config** (automatic library detection for build system)

### Build System Concepts:
- **Autoconf**: Generates `configure` script that detects system capabilities
- **Automake**: Generates `Makefile.in` from `Makefile.am` templates
- **pkg-config**: Provides compiler and linker flags for installed libraries
- **Variable Substitution**: `@VARIABLE@` in templates gets replaced by `configure`

---

## **Recent Build System Updates**

The Achilles build system has been updated to:
- **Automatically detect SDL2, OpenGL, and GLU** using pkg-config
- **Support both SDL1 and SDL2** with automatic fallback
- **Generate correct Makefiles** without manual path editing
- **Work across different Linux distributions** and installation methods (system packages, homebrew, etc.)

**Key Files**:
- `configure.in`: Contains library detection logic using pkg-config
- `Makefile.in`: Template with variable substitutions (`@SDL2_CFLAGS@`, etc.)
- `Makefile.am`: Automake source file defining build rules

**To rebuild after changes**:
```bash
autoconf          # Regenerate configure script
./configure       # Detect libraries and generate Makefile
make             # Compile
```

---

*This document was generated to help understand the Achilles evolutionary simulation codebase.*

