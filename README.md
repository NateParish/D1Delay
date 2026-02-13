# D1 Digital Delay

A fully custom-built digital delay plugin developed with **JUCE 8** (C++).

**D1 Digital Delay** is a stereo delay featuring BPM-synced timing, millisecond mode, ping-pong routing, high/low-pass filtering, stereo width control, and a complete preset system (factory + user presets). The GUI is custom rendered (knobs, switches, and visualizations) to resemble a rack-style hardware unit.

---

## Screenshots

### Delay View
![Delay Screen](docs/screenshots/delay-screen.png)

### EQ View
![EQ Screen](docs/screenshots/eq-screen.png)

### Presets Menu
![Presets](docs/screenshots/presets.png)

---

## Features

- **Stereo Digital Delay**
- **BPM Sync** (incl. musical subdivisions / time signature display)
- **Millisecond Mode** for manual delay time
- **Ping-Pong Mode**
- **Feedback** control
- **Mix / Level** control
- **High-pass / Low-pass filtering**
- **Stereo width** control and visualization
- **Independent Left / Right blend** controls
- **Preset system**
  - Built-in factory presets
  - Save and load user presets
  - Real-time preset switching
- **Custom-rendered GUI**
  - Custom knobs and toggle switches
  - Custom delay visualization
  - Custom EQ curve visualization
  - Real-time parameter display readout

---

## DSP Overview

The DSP engine is implemented in C++ and designed for responsive, real-time parameter control. The plugin includes:

- Delay buffer management with feedback routing
- Ping-pong routing across stereo channels
- Real-time high-pass / low-pass filtering
- Width manipulation and visualization
- Smooth UI interaction with custom parameter displays

---

## UI Architecture

The user interface is rendered using JUCE `Graphics` primitives and custom components:

- Filmstrip-based knob rendering
- Custom button / toggle styling
- Graph-based delay visualization (tap “bell” representation)
- EQ curve visualization view
- Preset dropdown/menu and save system integration

---

## Preset System

D1 Delay supports:

- Factory presets (included)
- User presets (save current settings)
- Loading and switching presets while running

---

## Build Instructions

1. Install **JUCE 8** (tested with **JUCE 8.0.1**).
2. Open `D1Delay.jucer` in **Projucer**.
3. Set your **global modules path** (or update modules paths as needed).
4. Select your exporter (Xcode / Visual Studio).
5. Export and build the **VST3** target.

---

## Notes

The production version includes a commercial licensing/authentication system.
This public repository uses a **local stub** so the demo project builds and runs without network dependencies.

---

## Author

**Nate Parish**  
Parish Audio

---

## License

This project is provided for demonstration/portfolio purposes.
