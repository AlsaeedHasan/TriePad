# TriePad

TriePad is a fast, interactive text-based terminal editor written in C++. Designed with built-in data structures and algorithms, it offers a lightweight but powerful environment for editing files right from the command line. It features a modal user interface reminiscent of Vim alongside a robust backend utilizing custom text buffers, the Knuth-Morris-Pratt (KMP) search algorithm, and a Trie-based dictionary for rapid autocomplete suggestions.

## Features

- **Interactive Terminal UI:** Built with the FTXUI framework, providing a fullscreen application with dynamic views, multiple modes (Normal and Insert), modals, and an integrated status bar and footer.
- **Trie-based Autocomplete:** Autocomplete system built recursively over a 26-ary Trie data structure. Given a partial word, TriePad searches an underlying dictionary file and provides dynamic suggestions.
- **Advanced Text Buffer Engine:** Utilizes a doubly linked list of lines rather than a monolithic string. This allows for scalable optimization on insertion and deletion operations.
- **Knuth-Morris-Pratt (KMP) Search:** Features an instantaneous text search implemented using the KMP algorithm ensuring O(N+M) time complexity when parsing large buffers.
- **Action History (Undo/Redo):** Records mutations linearly, allowing for simple and accurate undo and redo functionality during editing operations.

## Keyboard Controls

The application is heavily driven by keyboard shortcuts to ensure a smooth, mouse-free development experience:

- **Insert** - Enter Insert Mode
- **Escape** - Exit Insert Mode / Close active modal overlays
- **Ctrl+S** - Open the Save File Modal
- **Ctrl+O** - Open the Load File Modal
- **Ctrl+F** - Open the Search Modal (KMP Find)
- **Ctrl+U** - Undo last action
- **Ctrl+R** - Redo next action
- **Tab** - Cycle through autocomplete suggestions (Insert mode only)
- **Ctrl+Q** - Quit the editor

## Requirements

To build and run TriePad, the following minimum requirements must be met:

- **C++17 Compiler:** The project utilizes modern C++ standard features.
- **CMake 3.14 or newer:** Required for building the application.
- **FTXUI Framework:** No manual installation needed. The build configuration leverages CMake's FetchContent to securely download and link the FTXUI library at build time.

## Build Instructions

Building TriePad is straightforward via CMake.

1. Clone the repository and navigate to its directory:

   ```bash
   git clone <repository_url>
   cd TriePad
   ```

2. Create a build directory and enter it:

   ```bash
   mkdir build && cd build
   ```

3. Configure the CMake project (this step automatically fetches FTXUI):

   ```bash
   cmake ..
   ```

4. Compile the project:

   ```bash
   make
   ```

5. Run the application:
   ```bash
   ./TriePad
   ```

## Project Structure

- `CMakeLists.txt`: Build configuration and dependency resolution.
- `src/`: Main source files (`main.cpp`, `ActionHistory.cpp`, `TrieDictionary.cpp`, etc.).
- `include/`: Header definitions corresponding to the source files.
- `resources/`: Contains the dictionary file (`english_words.txt`) used by the Trie data structure for autocomplete matching.

## License

This project is licensed under the terms described in the [LICENSE](LICENSE) file found within the repository root.
