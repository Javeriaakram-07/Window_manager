# 🪟 Window Manager — DSA Assignment

A terminal-based window manager built in C++ using a **doubly linked list**. Simulates a desktop environment where each window is a node — create, activate, resize, and delete windows with a color-coded ANSI interface.

## ✨ Features

- 🎨 **Color-coded windows** with ANSI terminal rendering
- 🔼 **Activate** any window — moves it to the front (head of list)
- ↔️ **Resize** windows dynamically
- ❌ **Delete** windows from any position
- 📋 **Live desktop view** refreshes after every action

## 🏗️ Data Structure

Each window is a node in a **doubly linked list**:
- `head` = active (frontmost) window
- Activating a window **unlinks** it and **prepends** it to the list
- Supports forward and backward traversal via `next` / `prev` pointers

## 🚀 Run

Compile and run with any C++ compiler:

```bash
g++ window_manager.cpp -o window_manager
./window_manager
```

> **Windows users:** use `window_manager.exe` after compiling with g++ or MSVC.

## 📸 Usage

```
1. Create window   → enter title + pick color
2. Activate window → moves it to front
3. Resize window   → set new width & height
4. Delete window   → removes from any position
5. Refresh         → redraws desktop
0. Exit
```

## 👨‍💻 Author
Javeria Akram
