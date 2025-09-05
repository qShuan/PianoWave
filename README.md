# PianoWave
A C++ piano "synthesizer" application

##  Table of Contents

- [About](#about)  
- [Features](#features)  
- [Prerequisites](#prerequisites)
- [Building & Running](#building--running)
- [Resources](#resources)
- [License](#license)  
- [Contact](#contact)
  
---

## About

A piano application that allows you to play MIDI (.mid) music using synthesized sounds.

---

## Features

- Midi playback: load and play midi files
- Waveform synthesis
- Interactive interface: play notes directly from mouse clicks

---

## Prerequisites

To compile and run this project, make sure you have:

- **Visual Studio 2022** - Ensure that Visual Studio 2022 is installed with the "Desktop Development with C++" workload. 
- **C++23** standard support

---

## Building & Running

### Using Visual Studio 2022

1. Clone this repository:

   ```bash
   git clone https://github.com/qShuan/PianoWave.git
   cd PianoWave

2. Generate the solution file using premake (Or use the existing one):
   - Open the `scripts/` folder
   - Run `build.bat`
   
3. Open the solution file in Visual Studio 2022:  
   - Double-click `Piano Wave.sln`  
   - Or open Visual Studio → **File → Open → Project/Solution…** → select `Piano Wave.sln`
     
4. Build the project:  
   - Menu: **Build → Build Solution**  
   - Shortcut: `Ctrl+Shift+B`  

5. Run the program:  
   - Menu: **Debug → Start Debugging**  
   - Shortcut: `F5`  

---

## Resources

[SFML](https://www.sfml-dev.org/)

[ImGui](https://github.com/ocornut/imgui)

[ImGui-SFML](https://github.com/SFML/imgui-sfml)

[Midifile](https://github.com/craigsapp/midifile)

[Native File Dialog](https://github.com/btzy/nativefiledialog-extended)

---

[Tuning](https://www.ub.edu/pde/tomas.sanz.perela/static/assets/papers/GraciaSanz-pianotuning.pdf)

[Overtones](https://courses.physics.illinois.edu/phys398dlp/sp2022/documents/group1_final.pdf)

[The Math Behind Music and Sound Synthesis](https://www.youtube.com/watch?v=Y7TesKMSE74)

[Making Music With Math](https://aatishb.com/stringmath/)

[Key Frequencies](https://en.wikipedia.org/wiki/Piano_key_frequencies)

[Sound Generation](https://www.youtube.com/watch?v=5xd9BMxoXqo)

---

## License
This project is released under the **MIT License**. See [LICENSE](LICENSE) for full details.

---

## Contact
Feel free to reach out with feedback, bug reports, or suggestions!  
