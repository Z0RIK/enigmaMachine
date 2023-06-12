# Enigma Machine
This is a graphical user interface (GUI) application for the Enigma machine, a historical encryption device used during World War II. The application allows users to configure the Enigma machine with various settings, such as rotor selection, plugboard connections, and reflector type. Users can then input plaintext and obtain the corresponding ciphertext.

## Features
&#9632; Interactive GUI for configuring the Enigma machine.

&#9632; Customizable rotor selection and order.

&#9632; Support for various reflector types.

&#9632; Plugboard functionality for swapping letters.

&#9632; Real-time encoding of input text to obtain ciphertext.

&#9632; Visual representation of the Enigma machine components.

&#9632; Error handling for incorrect user input.

## Building
Clone the repository:

```bash
git clone --recurse-submodules https://github.com/Z0RIK/enigmaMachine.git
```

This repository contains GLFW and ImGui submodules, thats why `--recurse-submodules` has to be used.
Other than that bussines as usual.

Create and enter build directory:

```bash
cd enigmaMachine
mkdir build
cd build
```

Configure and build the project:

```bash
cmake -S ..
cmake --build . --config=Release
```

Note: that all build configurations except `Debug` will supress console window creation on windows.

Binary files could be found in `bin` folder inside your build directory.

## License
This project is licensed under the MIT License.

## References and resources used during the development of this project. 
https://en.wikipedia.org/wiki/Enigma_machine

https://www.youtube.com/watch?v=ybkkiGtJmkM 

https://www.theguardian.com/technology/2014/nov/14/how-did-enigma-machine-work-imitation-game.
