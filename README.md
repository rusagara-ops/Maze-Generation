# Maze Generation

## Description

This repository contains the implementation of a maze generation program using four different algorithms:
- Randomized Depth-First Search (DFS)
- Simplified Prim's Algorithm
- Least Recently Used (LRU) Algorithm
- Combined Algorithm (a mix of the above three)

The goal is to practice using structs, enums, and data structures in C while exploring different maze generation techniques.

## Getting Started
Follow these instructions to set up and run the maze generation program on your local machine.

### Prerequisites
- GCC (GNU Compiler Collection)
- Make

### Installation

Clone the repository
   
**Compile the program using the provided Makefile:**

make

**Usage**
Run the program with the following command:

**./mazegen width height [mode]**

width: The width of the maze (must be greater than 1).

height: The height of the maze (must be greater than 1).

[mode]: The maze generation algorithm to use (optional). 

Options are depth, prim, lru, or all. Defaults to depth if not specified.

Example:

./mazegen 20 20 all
