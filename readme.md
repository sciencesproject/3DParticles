# 3D particles

**3D particles** is a 3D non-physic particle simulator. It means it simulate the interaction of multiple virtual particles that (dis)attract each other, but without using Newton's gravity law.

**3D particles** is entirely written in C++11/17. It takes a raw text configuration file as input and output a series of 3D models files. See [how to use section](#how-to-use) for more.

> ## Table of contents
>
> - [Building](#building)
>   * [Linux](#linux)
>   * [Windows](#windows)
>   * [MacOS](#macos)
> - [How to use](#how-to-use)
>   * [Command line:](#command-line-)
>   * [Configuration file](#configuration-file)
>   * [Outputs files](#outputs-files)
>   * [Using Blender](#using-blender)
> - [Features](#features)
>   * [Checklist](#checklist)
>   * [Algorithms](#algorithms)

## Building

For all system, you need a g++ installation with the standard libraries and C++17 support (C++11 may also work).

### Linux
```bash
g++ -c main.cpp -o ./bin/main.o
g++ -c particle.cpp -o ./bin/particle.o
g++ -c simulation.cpp -o ./bin/simulation.o
g++ -c group.cpp -o ./bin/group.o
g++ -c vector3D.cpp -o ./bin/vector3D.o
g++ ./bin/main.o ./bin/particle.o ./bin/simulation.o ./bin/group.o ./bin/vector3D.o -o ./bin/3Dparticles.exe -static-libgcc
```

### Windows
```bash
g++.exe -c main.cpp -o bin\main.o
g++.exe -c particle.cpp -o bin\particle.o
g++.exe -c simulation.cpp -o bin\simulation.o
g++.exe -c group.cpp -o bin\group.o
g++.exe -c vector3D.cpp -o bin\vector3D.o
windres.exe 3Dparticles.rc bin\3Dparticles-res.o
g++.exe bin\main.o bin\particle.o bin\simulation.o bin\group.o bin\vector3D.o bin\3Dparticles-res.o -o bin\3Dparticles.exe -static-libgcc
```

> Note that using the linux command will also work, but *Windows defender* will slow down the program.

Using MSVC is certainly possible, but not yet tested.

### MacOS
I don't know how to build C++ programs on Mac, but I think Linux command works on it.

## How to use

The program takes a `.conf` file as input and output a Wavefront `.obj` file for each step and particle after a certain amount of simulation.

### Command line:
```bash
path/to/3Dparticles path/to/3Dparticles.conf
```
The command should be run in the object output folder.

### Configuration file

The configuration file can be named as you wish, extension included (so, you can create presets). It should always be a raw text file formatted as follows. Only numbers should appear and they must *always* contain there 9 lines: 
```
all_obj : Whatever the program should output a global obj file at each step jump | 0 for NO, 1 for YES
particle_obj : Whatever the program should output an obj file for each particle at each step jump | 0 for NO, 1 for YES
group_obj : Whatever the program should output an obj file for each group at each step jump | 0 for NO, 1 for YES
file_log : Whatever the program should write the logs to a file | 0 for NO, 1 for YES
term_log : Whatever the program should write the logs to the terminal | 0 for NO, 1 for YES
seed : The simulation seed, if 0, no random
simulation_range : The max distance needed between 2 particles to interact
simulation_air_resistance : Speed loose by the particles on each step | particle->speed *= (1-air_resistance)
total_steps : The total amount of steps
step_log : The amount of steps simulated between outputs creations
```

Plus, you can add as many particle groups as you want/need using pairs of lines like that:
```
total_particles : The cubic root of the amount of particles in the group
self_interaction : The attraction between the particles of the same group | negative value for repulsion
group_interaction : The attraction between the particles of an other group, must be repeted in the groups registration order for each previusly created groups | negative value for repulsion
```

Finally, the file must end with a `#` on his own line.

### Outputs files
There are 3 outputs files types :
 - `allX.obj` : each vertex represents a single particle at the X step
 - `pX.obj` : each vertex represents the same particle, but at different times
 - `groupY_X.obj` : each vertex represents a single particle of the Y group at the X step

### Using Blender
> [Blender](https://blender.org) is the free and open source 3D creation suite.

To get a visual output of the simulation, you can use there 2 options as follows:

blenderview.py:
 1. Create a new scene, no need to delete something or save.
 2. Change the default “*properties*” view to “*text editor*”.
 3. Open the `blenderview.py` file.
 4. Set your output folder absolute path at line 4.
 5. Set the prefix you want to import at line 5. ("all"; "p"; "groupX_")
 6. Click the run button.

blenderview add-on:
 1. Create a `zip` archive of the `blenderview` folder.
 2. In Blender, Go to `Edit>Preferences>Add-ons>Install` and search for `blenderview.zip`.
 3. Activate the `3D particles` add-on.
 4. Create a new scene, no need to delete something or save.
 5. Open the left panel in the 3D view and select `3D particles` at the bottom.
 6. Set your output path, initialize the scene and import what you want

Now, you can run the animation and look the particles interacting !

> The add-on is easier to use, but unfinished and only tested on 3.6 version.

## Features

### Checklist
- [x] High-speed Simulator
- [x] Easy to use
- [x] Simple integration in other apps
- [ ] Custom generation
- [ ] Custom interaction function
- [x] Multiple output possibilities
- [ ] Simulation pause option
- [ ] Boinc support

### Algorithms

When created, the original position of a particle is defined like that:
```cpp
x = i%ceil(p**0.3333333) + rand(0, 1)
y = floor(i/(p**0.3333333))%ceil(p**0.3333333) + rand(0, 1)
z = floor(i/floor(ceil(p**0.3333333)**2)) + rand(0, 1)
```
Here, `p` is the total number of particles in the group and `i` is the number of the particle in generation. The random value is chosen using the [std C++ rand function](https://cplusplus.com/reference/cstdlib/rand/) and the predefined seed of the [Configuration file](#configuration-file).

The interaction force between 2 particles is computed like that:
```cpp
strength = interaction / (distance ** distance)
speed = speed + normal * strength
```
The normal is the translation needed to go from the particle to the other in the range [0; 1].
