# Project-HashMap-Data-Structures-and-Algorithms

C++ implementation with separate chaining, dynamic memory management, and GoogleTest unit tests.

## Project Description

This project is a custom HashMap implementation written in C++. It stores and manages key-value pairs, similar to how dictionaries or maps work in programming. I built this project for my Data Structures and Algorithms coursework to better understand hashing, collision handling, linked nodes, and dynamic memory management. The HashMap supports operations such as inserting data, searching for keys, removing values, clearing the table, copying maps, comparing maps, and iterating through stored data. I used separate chaining with linked nodes to handle collisions when multiple keys mapped to the same bucket. I also used GoogleTest to test duplicate keys, missing keys, resizing, collisions, copy behavior, erase behavior, and other edge cases.

## Features

- Stores key-value pairs using a custom HashMap class
- Supports insert, search, remove, clear, copy, assignment, equality checking, and iteration
- Handles collisions using separate chaining with linked nodes
- Resizes the table when the load factor becomes too high
- Uses dynamic memory management with constructors, destructors, deep copying, and safe deletion
- Includes GoogleTest unit tests for duplicates, missing keys, collisions, resizing, copy behavior, erase behavior, and edge cases

## Skills Used

- C++
- Data Structures
- Object-Oriented Programming
- Dynamic Memory Management
- Pointers
- Linked Lists
- Debugging
- Unit Testing
- GoogleTest

## Files

| File | Description |
|---|---|
| `hashmap.h` | Main HashMap implementation |
| `hashmap_tests.cpp` | GoogleTest unit tests for the HashMap |

## What I Learned

Through this project, I learned how hash tables work behind the scenes. I practiced handling collisions, resizing a data structure, managing memory safely, and testing edge cases. This project also helped me become more comfortable with debugging C++ code and writing tests to confirm that each required feature worked correctly.
