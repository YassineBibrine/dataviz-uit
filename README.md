# DataViz-UiT

DataViz-UiT is an educational visualization tool for data structures and algorithms. It provides interactive visualizations, step-by-step playback, and controls for inspecting algorithm execution on common data structures (arrays, linked lists, trees, graphs, etc.). The project is implemented in C++17 and uses CMake for build orchestration (Visual Studio172022 generator).

## Key features

- Interactive visualizations for data structures (arrays, linked lists, trees, graphs).
- Animated algorithm execution with frame recording and playback controls.
- Implementations of common algorithms: BFS, DFS, Dijkstra, sorting algorithms (insertion, selection, bubble), tree traversals (preorder, inorder, postorder), and more.
- Modular architecture separating core data models, algorithm engines, UI components, visualization rendering, and orchestration factories.
- Session save/load support and a code generation component for exporting structure definitions.

## Project structure

- `CMakeLists.txt` - Project build configuration (targets, language standard, generator settings).
- `src/` - Source code root.
 - `core/` - Fundamental data models and managers: `array_structure`, `list_structure`, `tree_structure`, `graph_structure`, `session_manager`, `data_model_manager`.
 - `algorithms/` - Algorithm implementations and runners (e.g., `bfs_runner`, `dfs_runner`, `dijkstra_runner`, `bubble_sort`, `insertion_sort`, `selection_sort`, `tree_*` traversals).
 - `visualization/` - Rendering, animation frames, interaction management, playback controller.
 - `ui/` - UI panels and dialogs such as main window, control panel, structure selector, color legend.
 - `orchestration/` - Factories and managers that wire algorithms to visualization and UI controls.
 - `codegen/` - Code generation utilities for structure parsing and exporting.
 - `main.cpp` - Application entry point.

## Requirements

- CMake3.16 or newer (project tested with3.31.6-msvc6).
- Visual Studio2022 (generator: `Visual Studio172022`).
- C++17 toolset / standard.

## Build (Windows / Visual Studio)

1. Install Visual Studio2022 with C++ workload and CMake support.
2. From the repository root, open `CMakeLists.txt` in Visual Studio (File -> Open -> CMake...).
3. Configure and generate the solution using the `Visual Studio172022` generator.
4. Build the `ALL_BUILD` target or the specific project target.

Alternatively, use the command line (Developer PowerShell for VS2022):

```powershell
mkdir build
cd build
cmake -G "Visual Studio172022" ..
cmake --build . --config Release
```

## Run

- Launch the built executable from Visual Studio or run the binary in the build output folder. The UI allows selecting a data structure, choosing an algorithm, supplying inputs (where applicable), and starting the animated visualization.
- Use the playback controls to step forward/backward, pause, resume, and record frames.

## Architecture & Design notes

- The codebase separates concerns across modules: core data structures, algorithm runners that produce execution frames, visualization renderer which consumes frames and draws animated states, and UI/orchestration layers that provide user controls and map UI events to algorithm execution.
- Algorithm implementations are written to populate an `execution_state` (frame sequence) rather than directly mutating UI components. The renderer plays back these frames for deterministic animations.
- Factories in `orchestration/` decouple specific algorithm implementations from the UI and enable registering new algorithms with minimal UI changes.

## Extending the project

- Adding a new algorithm:
1. Implement the algorithm logic in `src/algorithms/` following the existing runner patterns (produce frames via `execution_state`).
2. Add a header and implementation file and register the algorithm with the appropriate factory in `src/orchestration/`.
3. Add UI inputs if the algorithm needs custom parameters (use `algorithm_input_dialog`).

- Adding a new data structure:
1. Create the data model in `src/core/` and provide serialization for sessions if needed.
2. Update the structure selector UI and any code generators that should export the structure.
3. Ensure visualization renderer supports drawing the new structure (add new rendering code in `src/visualization/`).

## Contributing

- Follow existing coding style and module separations.
- Keep algorithm implementations deterministic and frame-driven for reliable visual playback.
- Add unit tests or quick tests where appropriate and update the documentation files under the repo root.

## Troubleshooting

- If build fails, ensure the correct CMake generator and Visual Studio components are installed.
- For runtime issues check console output and logs if available. Search the repo for `TODO` and `FIXME` markers for known issues and workarounds.

## References

- CMake: https://cmake.org
- Visual Studio CMake support documentation: https://learn.microsoft.com/visualstudio/cpp/cmake

## License

- Check the repository root or `LICENSE` file for licensing details. If none exists, add an appropriate license before distribution.

---

For development questions or to request specific documentation inside code files, update comments in the corresponding source files and include a short description of the intended behavior to keep documentation close to implementation.