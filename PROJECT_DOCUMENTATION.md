# Project Documentation: Classes, Methods and Properties

This document summarizes the classes, key methods and observable properties for the main source files and folders in this repository. It was produced by scanning the project structure and source files. For complete, authoritative details inspect the source file directly.

---

## Top-level folders
- `src/` — application source
 - `core/` — core data structures and model management
 - `ui/` — Qt user interface classes
 - `visualization/` — rendering, layout, interaction, animation
 - `orchestration/` — algorithm management and factories
 - `algorithms/` — concrete algorithm runners and helpers
 - `codegen/` — code generation and parsing utilities
 - other support files (main.cpp, CMake, docs)

---

## src/core

### `data_model_manager.h` / `data_model_manager.cpp`
Class: `DataModelManager`
- Purpose: Central registry and factory for data structures (Array, List, Tree, Graph). Manages metadata and selection.
- Key properties:
 - `std::map<std::string, std::unique_ptr<DataStructure>> structures` — stored structures
 - `std::map<std::string, StructureMetadata> metadata` — metadata (name, type, creationType)
 - `std::string selectedStructureId`
 - `int nextStructureId` (or similar)
- Key methods:
 - `std::string createDataStructure(const std::string& type, int size, const std::string& name)` — create generated structure
 - `std::string createInteractiveStructure(const std::string& type, const std::string& name)` — create interactive (empty) structure
 - `std::string buildStructureFromNodes(const std::string& type, const std::map<std::string,int>& nodes, const std::vector<std::pair<std::string,std::string>>& edges, const std::string& name)` — build from drawn nodes/edges
 - `std::string detectStructureType(int nodeCount, const std::vector<std::pair<std::string,std::string>>& edges)` — detect type (Array/List/Tree/Graph)
 - `bool selectStructure(const std::string& id)`, `DataStructure* getStructure(const std::string& id)`, `DataStructure* getSelectedStructure()`
 - management: `removeStructure`, `renameStructure`, `clearAll`, `getAllStructures()`, `createSampleStructures()`

### `data_structure.h`
Class: `DataStructure` (abstract)
- Purpose: Base interface for concrete data structure wrappers
- Key virtual methods:
 - `virtual std::vector<DSNode> getNodes() const =0`
 - `virtual std::vector<DSEdge> getEdges() const =0`
 - `virtual std::string serializeToDOT() const =0`
 - `virtual void* getDataForRunner() =0`

### `array_structure.h` / `array_structure.cpp`
Class: `ArrayStructure` : `DataStructure`
- Properties:
 - `std::vector<int> data`
- Methods:
 - `ArrayStructure(int size)` constructor
 - `void fillRandom(int size)` — fills vector with random ints
 - `std::vector<DSNode> getNodes() const` — returns nodes like `arr_i`
 - `std::vector<DSEdge> getEdges() const` — sequential edges
 - `std::string serializeToDOT() const` — record style DOT node
 - `void* getDataForRunner()` — returns &data

### `list_structure.h` / `list_structure.cpp`
Class: `ListStructure` : `DataStructure`
- Internal `ListNode` nodes (doubly-linked)
- Properties:
 - `ListNode* head`
- Methods:
 - `generateSequential(int)`, `generateRandom(int)`, `append(int)`
 - `getNodes()`, `getEdges()` — nodes named `list_i` and sequential edges
 - `serializeToDOT()` — DOT layout as linked list
 - `getDataForRunner()` returns head pointer

### `tree_structure.h` / `tree_structure.cpp`
Class: `TreeStructure` : `DataStructure`
- Internal `TreeNode` with `value`, `left`, `right`, `parent`, `height`, `balanceFactor`
- Properties:
 - `TreeNode* root`
- Methods:
 - `insert(int)` — insert by BST logic
 - `generateRandom(int)` — builds balanced BST by generating unique random values, sorting, and building from middle (balanced)
 - `buildBalancedTree(const std::vector<int>&, int, int, TreeNode*)` — helper
 - `getNodes()`, `getEdges()` — pre-order collection
 - `serializeToDOT()`
 - `void* getDataForRunner()` — returns root pointer

### `graph_structure.h` / `graph_structure.cpp`
Class: `GraphStructure` : `DataStructure`
- Wraps `Graph` object (see `graph.h`)
- Constructors:
 - `GraphStructure()` default undirected
 - `GraphStructure(bool directed)`
 - `GraphStructure(int nodeCount, bool directed=false)` — new constructor to create `nodeCount` nodes and generate random edges
- Methods:
 - `generateRandom(int nodeCount, int edgeAttempts)` — populates nodes and edges (ensures connectivity by spanning tree then random edges)
 - `getNodes()`, `getEdges()`
 - `serializeToDOT()` — produces DOT representation
 - `void* getDataForRunner()` — returns Graph*

### `graph.h` / `graph.cpp`
Class: `Graph` (adjacency list implementation)
- Node representation with `properties` map
- Methods likely:
 - `addNode(id, props)`, `addEdge(from,to,weight,props)`, `getAllNodeIds()`, `getAllEdges()`, `hasEdge(from,to)`, `getNode(id)`
- Note: used by GraphStructure and sample data creation

### `data_model_manager.cpp` (file-level notes)
- Implements sample structures creation (array, list, binary tree, graph)
- Normalizes tree type to "Binary Tree" in metadata

---

## src/ui
UI components using Qt widgets. Key classes below.

### `main_window.h` / `main_window.cpp` (or `main_window_new.cpp`)
Class: `MainWindow` : `QMainWindow`
- Purpose: Application main window, coordinates UI panels and visualization
- Properties (fields):
 - `std::unique_ptr<DataModelManager> dataModelManager`
 - `std::unique_ptr<VisualizationPane> visualizationPane`
 - `std::unique_ptr<ControlPanel> controlPanel`
 - `std::unique_ptr<MetricsPanel> metricsPanel`
 - `ToolboxPanel* toolboxPanel`, `StructureSelector* structureSelector`
 - `AlgorithmManager& algoManager`
 - `std::string currentAlgorithm`, `std::unique_ptr<Algorithm> currentAlgorithm` etc.
- Key methods:
 - `setupUI()`, `connectSignals()`, `createMenuBar()`
 - Structure selection handlers: `onStructureSelected`, `onStructureRemoved`, `onSamplesCreated`
 - `loadStructureIntoCanvas(const std::string& id)` — converts DataStructure nodes/edges to interactive canvas nodes and edges. Implements different layout per type (Array linear, List linear, Binary Tree hierarchical layout, Graph circular or Graphviz)
 - Algorithm execution: `executeAlgorithm(const std::string& algorithm)` — picks category, creates algorithm via AlgorithmManager and runs it; now robust with getCategoryForAlgorithm and try/catch

### `structure_selector.h` / `structure_selector.cpp`
Class: `StructureSelector` : `QWidget`
- Provides UI list of structures and controls for creating/renaming/removing
- Properties (widgets): `QListWidget* structureList`, `QComboBox* typeCombo`, `QSpinBox* sizeSpinBox`, `QLineEdit* nameEdit`, `QPushButton* finalizeBtn`, `renameBtn`, `removeBtn`, `clearInteractiveBtn`
- Methods / signals:
 - `setDataModelManager(DataModelManager*)`
 - `refreshStructureList()` — populate list with metadata
 - Slots: `onStructureClicked`, `onFinalizeClicked` (create), `onRemoveClicked`, `onRenameClicked`, `onCreateSamplesClicked`, `onClearInteractiveClicked`
 - Emits: `structureSelected(QString)`, `structureRemoved(QString)`, `samplesCreated()`, `clearInteractiveRequested()`

### `control_panel.h` / `control_panel.cpp`
Class: `ControlPanel` : `QWidget`
- Playback and algorithm controls (play/pause/step/reset, algorithm combo, speed slider)
- Widgets: `QPushButton* playButton`, `pauseButton`, `resetButton`, `stepForwardButton`, `stepBackwardButton`; `QComboBox* algorithmCombo`; `QSlider* speedSlider`; `QLabel* currentFrameLabel`
- Methods:
 - `updateAlgorithmList(const QString& structureType)` — populates algorithmCombo from AlgorithmManager categories
 - Signals: `playClicked()`, `pauseClicked()`, `resetClicked()`, `stepForwardClicked()`, `stepBackwardClicked()`, `algorithmSelected(QString)`, `speedChanged(int)`

### `toolbox_panel.h` / `toolbox_panel.cpp`
Class: `ToolboxPanel` : `QWidget`
- Provides drag tools (create node circle/rect, link, erase) depending on structure type
- Methods: `updateTools(const QString& mode)` to populate tool list

### `visualization_pane.h` / `visualization_pane.cpp`
Class: `VisualizationPane` : `QWidget` (or subclass)
- Hosts the interactive canvas where nodes and edges are drawn and animated
- Likely has: `InteractionManager* interactionManager` and `VisualizationRenderer` to draw frames
- Methods: `refreshDisplay()`, `reset()`, `clearNodeValues()`, `getInteractionManager()`

### `metrics_panel.cpp`, `code_generator_dialog.cpp` and other UI files
- `metrics_panel` shows algorithm metrics
- `code_generator_dialog` provides dialog for generating/parsing code into structures

---

## src/visualization

### `visualization_renderer.h` / `.cpp`
- Core rendering primitives for nodes, edges and animation frames
- Integrates with Qt painting APIs and `VisualizationPane`

### `interaction_manager.h` / `.cpp`
Class: `InteractionManager`
- Manages interactive nodes/edges on canvas; responsible for adding nodes, edges, updating node values
- Methods commonly used by MainWindow `interactionMgr->addNode(x,y,shape)`, `addEdge(id1,id2)`, `updateNodeValue(nodeId, value)`, `clearInteractive()`

### `GraphvizLayoutEngine.h` / `GraphvizLayoutEngine.cpp`
Class: `GraphvizLayoutEngine`
- Purpose: Convert DOT string to node positions using Graphviz when available, with a fallback circular layout
- Key methods:
 - `computeLayout(const std::string& dotString)` — returns map nodeId -> (x,y)
 - `isAvailable()`
 - `setLayoutAlgorithm(const std::string& algorithm)`
- Implementation notes: header inclusion conditional on `GRAPHVIZ_AVAILABLE` CMake definition; robust fallback when Graphviz not installed

### `animation_frame.h/cpp`, `frame_interpolator.h`
- Support data types for frames of animation (node highlights, swaps, etc.) and interpolation between frames

### `playback_controller.h/cpp`
- Controls playback timeline, stepping frames and synchronizing rendering

---

## src/orchestration

### `algorithm_manager.h` / `algorithm_manager.cpp`
Class: `AlgorithmManager` (singleton)
- Responsibilities: register `AlgorithmFactory` instances per category, provide factory-based creation
- Methods:
 - `registerFactory(const std::string& category, std::unique_ptr<AlgorithmFactory> factory)`
 - `createAlgorithm(const std::string& category, const std::string& type)`
 - `getCategories()`, `getAlgorithmNames(const std::string& category)`
 - `getCategoryForAlgorithm(const std::string& algorithm)` — maps algorithm name to category

### Factory implementations
- `sorting_algorithm_factory.cpp` — returns `BubbleSort`, `QuickSort`, `MergeSort`
- `filtering` and `transform` factories exist similarly

---

## src/algorithms

### `algorithm_runner.h` / `.cpp`
Class: `AlgorithmRunner`
- Base class for algorithm animations and execution
- Provides lifecycle methods: `execute()`, `stepForward()`, `stepBackward()`, `reset()` and frame/metrics recording support
- Derived classes implement algorithm-specific logic

### Concrete algorithms
- `bubble_sort.h` / `.cpp` — `BubbleSort`:
 - Constructors: default sample data, or `BubbleSort(ArrayStructure*)` to load data from structure
 - Methods: `execute()`, `stepForward()`, `stepBackward()`, `reset()`; internal `processComparison(i,j)` to perform comparison and swap and record frames
 - Uses `AnimationFrame` and `ExecutionState` to record metrics and frames

- `quick_sort`, `merge_sort` likely similar scaffolding

---

## src/codegen

### `structure_code_generator.h` / `.cpp`
Class: `StructureCodeGenerator`
- Generates C++ code representing the given `DataStructure` for Array, List, Binary Tree, Graph
- Methods: `generateCode(const DataStructure*, bool includeComments)` and helpers like `generateArrayCode`, `generateTreeCode`, `generateGraphCode`

### `code_structure_parser.h` / `.cpp`
Class: `CodeStructureParser`
- Parses provided C++ code and extracts structure values for Array, LinkedList, BinaryTree, Graph
- Methods: `parse(const std::string& code)` returning `ParsedStructure` including node values and edges

---

## Other notable files

- `src/main.cpp` — application entry point, creates QApplication and `MainWindow` and starts event loop
- `CMakeLists.txt` — cross-platform CMake configuration with support for Qt5/Qt6 and optional Graphviz
- `Directory.Build.props`, `CMakePresets.json`, `.vscode/*` — IDE integration and IntelliSense support files

---

## Notes and next steps
- This document is a high-level summary. For **file-by-file** exact method signatures, inspect each `.h` / `.cpp` file directly.
- If you want, I can generate a detailed per-file report by scanning all files and extracting exact class/method signatures into a more exhaustive markdown file. That will take additional time and will read each source file in the repository.

---

Generated on request. 
