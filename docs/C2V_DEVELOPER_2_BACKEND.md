# C2V Developer 2: Backend & State Mapping
**Focus Area**: Runtime Instrumentation & Visualization  
**Timeline**: Weeks 2-4 (overlaps with Developer 1)

## Your Role

You are responsible for **injecting runtime logging into compiled code, capturing execution events, and converting them to visualization states**. Your work transforms execution traces into the data needed for the visualization engine. Think of you as building the "observation" layer - capturing what happens when code runs.

---

## Deliverables Overview

### Phase 1: LLVM Instrumentation (Week 2)
- ? Create LLVM passes for code instrumentation
- ? Inject logging callbacks at critical points
- ? Compile instrumented binaries

### Phase 2: Execution Event Capture (Week 3)
- ? Record execution events from instrumented code
- ? Serialize events to persistent logs
- ? Provide event access APIs

### Phase 3: State Mapping (Week 4)
- ? Convert execution events to visualization frames
- ? Generate Graphviz DOT code per frame
- ? Generate memory layout representations

---

## Dependencies

**Before starting**, ensure Developer 1 has completed:
- ? Working `ASTParser` (can parse C++ files)
- ? Working `StructureExtractor` (outputs `DataStructureInfo`)
- ? Compiled `clang_integration` library

You'll receive their `DataStructureInfo` objects and use them to guide instrumentation.

---

## Detailed Tasks

### Task 2.1: Create Execution Event Model

**File**: `src/analysis/state_mapping/execution_event.h`

```cpp
#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// Represents a single execution event (state change)
struct ExecutionEvent {
    /// Source file line where event occurred
    int lineNumber = -1;
    
    /// Type of operation: "assign", "add_node", "remove_node", "call", etc.
    std::string operation;
    
    /// Variable names and their values at this point
  std::map<std::string, std::string> variables;
    
    /// Memory addresses of objects (for pointer visualization)
    std::map<std::string, std::string> addresses;
    
    /// Nanoseconds since execution started
    uint64_t timestamp = 0;
    
    /// Convert to JSON for serialization
    json toJSON() const {
    json j;
  j["lineNumber"] = lineNumber;
        j["operation"] = operation;
        j["variables"] = variables;
        j["addresses"] = addresses;
        j["timestamp"] = timestamp;
        return j;
    }
  
    /// Create from JSON
    static ExecutionEvent fromJSON(const json &j) {
        ExecutionEvent event;
        event.lineNumber = j["lineNumber"];
        event.operation = j["operation"];
        event.variables = j["variables"].get<std::map<std::string, std::string>>();
      event.addresses = j["addresses"].get<std::map<std::string, std::string>>();
        event.timestamp = j["timestamp"];
        return event;
    }
};

/// Runtime logging function to be injected into instrumented code
/// Called at each instrumentation point
extern "C" void logExecutionEvent(
    const char *operation,
    int lineNumber,
  const char *varName,
    const char *varValue,
    const char *address
);
```

---

### Task 2.2: Implement Execution Logger

**File**: `src/analysis/state_mapping/execution_logger.h`

```cpp
#pragma once

#include "execution_event.h"
#include <vector>
#include <mutex>

class ExecutionLogger {
public:
    ExecutionLogger() = default;
    
    /// Record a single execution event
    void recordEvent(const ExecutionEvent &event);
    
    /// Get total number of recorded events
    size_t getEventCount() const;
    
    /// Get event at specific index
    const ExecutionEvent *getEventAt(size_t index) const;
    
    /// Get all events in range
    std::vector<ExecutionEvent> getEventsInRange(size_t start, size_t end) const;
    
    /// Save all events to JSON file
    bool saveToFile(const std::string &filePath) const;
    
    /// Load events from JSON file
    bool loadFromFile(const std::string &filePath);
    
  /// Clear all events
    void clear();
    
    /// Get all events (const)
    const std::vector<ExecutionEvent> &getAllEvents() const { return events_; }
    
private:
    std::vector<ExecutionEvent> events_;
    mutable std::mutex eventsMutex_;  // For thread-safe logging
};

/// Global logger instance (used by instrumented code)
extern ExecutionLogger g_executionLogger;
```

**File**: `src/analysis/state_mapping/execution_logger.cpp`

```cpp
#include "execution_logger.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

ExecutionLogger g_executionLogger;

void ExecutionLogger::recordEvent(const ExecutionEvent &event)
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    events_.push_back(event);
}

size_t ExecutionLogger::getEventCount() const
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    return events_.size();
}

const ExecutionEvent *ExecutionLogger::getEventAt(size_t index) const
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    if (index >= events_.size()) {
      return nullptr;
 }
    return &events_[index];
}

std::vector<ExecutionEvent> ExecutionLogger::getEventsInRange(
    size_t start, size_t end) const
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    std::vector<ExecutionEvent> result;
    
    if (start >= events_.size()) {
        return result;
    }
    
    end = std::min(end, events_.size());
    for (size_t i = start; i < end; ++i) {
        result.push_back(events_[i]);
    }
    
    return result;
}

bool ExecutionLogger::saveToFile(const std::string &filePath) const
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    
    json j;
    j["events"] = json::array();
    
    for (const auto &event : events_) {
 j["events"].push_back(event.toJSON());
    }
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << "\n";
        return false;
    }

    file << j.dump(2);  // Pretty-print with 2-space indent
    file.close();
  
    return true;
}

bool ExecutionLogger::loadFromFile(const std::string &filePath)
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filePath << "\n";
        return false;
    }
    
    try {
        json j;
        file >> j;
 
    events_.clear();
  for (const auto &eventJson : j["events"]) {
            events_.push_back(ExecutionEvent::fromJSON(eventJson));
 }
     
  return true;
    } catch (const std::exception &e) {
    std::cerr << "Error parsing JSON: " << e.what() << "\n";
        return false;
    }
}

void ExecutionLogger::clear()
{
    std::lock_guard<std::mutex> lock(eventsMutex_);
    events_.clear();
}

// Logging function called by instrumented code
extern "C" void logExecutionEvent(
    const char *operation,
    int lineNumber,
    const char *varName,
    const char *varValue,
    const char *address)
{
    ExecutionEvent event;
    event.operation = operation ? operation : "";
    event.lineNumber = lineNumber;
    event.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    
    if (varName && varValue) {
        event.variables[varName] = varValue;
    }
    if (varName && address) {
        event.addresses[varName] = address;
    }
    
    g_executionLogger.recordEvent(event);
}
```

---

### Task 2.3: Create State Model for Visualization

**File**: `src/analysis/state_mapping/visualization_state.h`

```cpp
#pragma once

#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/// Represents a single frame in the visualization animation
struct VisualizationState {
    /// Frame number in animation (0-based index)
    int stepNumber = 0;
    
    /// Nodes as JSON array: [{"id": "1", "label": "Node 1"}, ...]
    std::string nodeData;
    
    /// Edges as JSON array: [{"from": "1", "to": "2"}, ...]
    std::string edgeData;
    
    /// Memory layout: variable_name -> memory_address
    std::map<std::string, std::string> memoryState;
    
    /// Performance metrics: metric_name -> value
    std::map<std::string, std::string> metrics;
    
    /// Graphviz DOT code for this frame
    std::string dotCode;
    
    /// Operation description (what changed in this frame)
    std::string operationDescription;
  
    /// Convert to JSON
    json toJSON() const;
    
    /// Create from JSON
    static VisualizationState fromJSON(const json &j);
};

/// Represents a graph node in visualization
struct VisualizationNode {
    std::string id;
    std::string label;
    std::map<std::string, std::string> properties;
};

/// Represents a graph edge in visualization
struct VisualizationEdge {
    std::string from;
    std::string to;
    std::string label;
    std::map<std::string, std::string> properties;
};
```

**File**: `src/analysis/state_mapping/visualization_state.cpp`

```cpp
#include "visualization_state.h"

json VisualizationState::toJSON() const
{
    json j;
    j["stepNumber"] = stepNumber;
    j["nodeData"] = nodeData;
    j["edgeData"] = edgeData;
    j["memoryState"] = memoryState;
  j["metrics"] = metrics;
    j["dotCode"] = dotCode;
    j["operationDescription"] = operationDescription;
    return j;
}

VisualizationState VisualizationState::fromJSON(const json &j)
{
    VisualizationState state;
    state.stepNumber = j["stepNumber"];
    state.nodeData = j["nodeData"];
    state.edgeData = j["edgeData"];
  state.memoryState = j["memoryState"].get<std::map<std::string, std::string>>();
    state.metrics = j["metrics"].get<std::map<std::string, std::string>>();
    state.dotCode = j["dotCode"];
 state.operationDescription = j["operationDescription"];
    return state;
}
```

---

### Task 2.4: Implement State Mapper

**File**: `src/analysis/state_mapping/state_mapper.h`

```cpp
#pragma once

#include "execution_event.h"
#include "execution_logger.h"
#include "visualization_state.h"
#include "clang_integration/structure_extractor.h"
#include <string>
#include <vector>
#include <memory>

class StateMapper {
public:
    explicit StateMapper(const DataStructureInfo &dataStructure);
    
    /// Convert execution log to visualization frames
    /// @param logger The execution logger containing all events
    /// @return Vector of visualization states (one per frame)
    std::vector<VisualizationState> generateVisualizationStates(
 const ExecutionLogger &logger);
 
    /// Generate Graphviz DOT code for a state
    /// @param state The visualization state
    /// @return DOT code string
std::string generateDOTCode(const VisualizationState &state);
    
    /// Generate memory view data from state
    std::string generateMemoryViewData(const VisualizationState &state);
    
    /// Parse node JSON and extract graph nodes
    std::vector<VisualizationNode> parseNodes(const std::string &jsonString);
    
    /// Parse edge JSON and extract graph edges
    std::vector<VisualizationEdge> parseEdges(const std::string &jsonString);
  
private:
    DataStructureInfo dataStructure_;
    
    /// Convert a single event to a visualization state
    VisualizationState eventToState(const ExecutionEvent &event, int stepNum);
 
    /// Build node/edge data from event variables
    void updateGraphStructure(VisualizationState &state, 
   const ExecutionEvent &event);
};
```

**File**: `src/analysis/state_mapping/state_mapper.cpp`

```cpp
#include "state_mapper.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

StateMapper::StateMapper(const DataStructureInfo &dataStructure)
    : dataStructure_(dataStructure) {}

std::vector<VisualizationState> StateMapper::generateVisualizationStates(
    const ExecutionLogger &logger)
{
    std::vector<VisualizationState> states;
    const auto &events = logger.getAllEvents();
    
for (size_t i = 0; i < events.size(); ++i) {
        auto state = eventToState(events[i], static_cast<int>(i));
   
        // Update graph structure based on event
     updateGraphStructure(state, events[i]);
        
        // Generate DOT code
        state.dotCode = generateDOTCode(state);
        
        states.push_back(state);
 }
    
    return states;
}

VisualizationState StateMapper::eventToState(const ExecutionEvent &event, int stepNum)
{
    VisualizationState state;
    state.stepNumber = stepNum;
    state.operation Description = event.operation;
    state.memoryState = event.addresses;
  
    // Initialize node and edge data as empty JSON arrays
    state.nodeData = "[]";
    state.edgeData = "[]";
    
    return state;
}

void StateMapper::updateGraphStructure(VisualizationState &state, 
      const ExecutionEvent &event)
{
    // Parse current graph state
    auto nodes = parseNodes(state.nodeData);
    auto edges = parseEdges(state.edgeData);
 
  // Handle different operations
    if (event.operation == "add_node") {
     // Extract node ID from variables
        auto it = event.variables.find("node_id");
 if (it != event.variables.end()) {
            VisualizationNode node;
node.id = it->second;
     node.label = "Node " + it->second;
            nodes.push_back(node);
        }
    } 
    else if (event.operation == "add_edge") {
        // Extract from/to from variables
        auto fromIt = event.variables.find("from_id");
 auto toIt = event.variables.find("to_id");
     
        if (fromIt != event.variables.end() && toIt != event.variables.end()) {
      VisualizationEdge edge;
            edge.from = fromIt->second;
            edge.to = toIt->second;
        edges.push_back(edge);
    }
    }
    else if (event.operation == "remove_node") {
        auto it = event.variables.find("node_id");
      if (it != event.variables.end()) {
        // Remove node with this ID
 nodes.erase(
      std::remove_if(nodes.begin(), nodes.end(),
                    [&it](const VisualizationNode &n) { return n.id == it->second; }),
     nodes.end()
            );
        }
    }
    
    // Convert back to JSON
    json nodeJson = json::array();
  for (const auto &node : nodes) {
        nodeJson.push_back({
      {"id", node.id},
    {"label", node.label}
        });
    }
    state.nodeData = nodeJson.dump();
    
    json edgeJson = json::array();
    for (const auto &edge : edges) {
        edgeJson.push_back({
    {"from", edge.from},
  {"to", edge.to}
        });
    }
    state.edgeData = edgeJson.dump();
}

std::string StateMapper::generateDOTCode(const VisualizationState &state)
{
    std::stringstream ss;
    ss << "digraph {\n";
    ss << "  rankdir=LR;\n";
    ss << "  node [shape=box];\n";
    
    // Add nodes
    auto nodes = parseNodes(state.nodeData);
    for (const auto &node : nodes) {
 ss << "  \"" << node.id << "\" [label=\"" << node.label << "\"];\n";
    }
    
    // Add edges
    auto edges = parseEdges(state.edgeData);
    for (const auto &edge : edges) {
    ss << "  \"" << edge.from << "\" -> \"" << edge.to << "\";\n";
    }
    
    ss << "}\n";
    return ss.str();
}

std::string StateMapper::generateMemoryViewData(const VisualizationState &state)
{
    json j = json::object();
    
    for (const auto &[varName, address] : state.memoryState) {
        j[varName] = address;
    }
    
    return j.dump(2);
}

std::vector<VisualizationNode> StateMapper::parseNodes(const std::string &jsonString)
{
    std::vector<VisualizationNode> nodes;
    
    try {
        auto j = json::parse(jsonString);

        for (const auto &item : j) {
  VisualizationNode node;
    node.id = item["id"];
            node.label = item["label"];
   nodes.push_back(node);
        }
    } catch (const std::exception &e) {
        // Silently handle parse errors
    }
    
    return nodes;
}

std::vector<VisualizationEdge> StateMapper::parseEdges(const std::string &jsonString)
{
    std::vector<VisualizationEdge> edges;
    
    try {
        auto j = json::parse(jsonString);
        
        for (const auto &item : j) {
    VisualizationEdge edge;
            edge.from = item["from"];
            edge.to = item["to"];
    edges.push_back(edge);
 }
    } catch (const std::exception &e) {
     // Silently handle parse errors
    }
    
    return edges;
}
```

---

### Task 2.5: Implement LLVM Instrumentation

**File**: `src/analysis/instrumentation/llvm_instrumentation.h`

```cpp
#pragma once

#include "clang_integration/structure_extractor.h"
#include <string>
#include <vector>

class LLVMInstrumentation {
public:
    explicit LLVMInstrumentation(const DataStructureInfo &dataStructure);
    
    /// Instrument source code with logging calls
    /// @param sourceFile Input C++ source file
    /// @param outputFile Output instrumented C++ source file
    bool instrumentCode(const std::string &sourceFile,
     const std::string &outputFile);
    
    /// Compile instrumented code to object file
/// @param sourceFile The instrumented source
    /// @param outputFile Output object file (.o or .obj)
    bool compileInstrumented(const std::string &sourceFile,
                const std::string &outputFile);
    
private:
    DataStructureInfo dataStructure_;
    
    /// Helper to inject logging call at location
    std::string injectLoggingCall(const std::string &line, 
  int lineNum);
};
```

**File**: `src/analysis/instrumentation/llvm_instrumentation.cpp`

```cpp
#include "llvm_instrumentation.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <cstdlib>
#include <iostream>

LLVMInstrumentation::LLVMInstrumentation(const DataStructureInfo &dataStructure)
    : dataStructure_(dataStructure) {}

bool LLVMInstrumentation::instrumentCode(const std::string &sourceFile,
     const std::string &outputFile)
{
    std::ifstream inFile(sourceFile);
    if (!inFile.is_open()) {
std::cerr << "Cannot open source file: " << sourceFile << "\n";
        return false;
    }
    
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
     std::cerr << "Cannot open output file: " << outputFile << "\n";
        return false;
    }
    
    // Add logging include header
    outFile << "#include \"execution_logger.h\"\n";
    outFile << "extern \"C\" void logExecutionEvent(\n";
    outFile << "    const char*, int, const char*, const char*, const char*);\n\n";
    
    std::string line;
    int lineNum = 1;
    
 while (std::getline(inFile, line)) {
 // Check if line contains method from dataStructure
        bool isMethodCall = false;
     
        for (const auto &method : dataStructure_.memberFunctions) {
       if (line.find(method) != std::string::npos) {
          isMethodCall = true;
      break;
    }
        }
 
  if (isMethodCall) {
     // Inject logging call before this line
  std::string loggingCall = injectLoggingCall(line, lineNum);
            outFile << loggingCall << "\n";
        }
        
        outFile << line << "\n";
  lineNum++;
    }
    
    inFile.close();
    outFile.close();
    
    return true;
}

std::string LLVMInstrumentation::injectLoggingCall(const std::string &line, 
   int lineNum)
{
    std::stringstream ss;
    
    // Extract variable assignments from line
    std::regex assignmentPattern(R"((\w+)\s*=\s*([^;]+))");
    std::smatch match;
    
    if (std::regex_search(line, match, assignmentPattern)) {
 std::string varName = match[1];
        std::string varValue = match[2];
      
        ss << "logExecutionEvent(\"assign\", " << lineNum << ", "
      << "\"" << varName << "\", "
      << "std::to_string(" << varValue << ").c_str(), nullptr);";
    } else {
      ss << "logExecutionEvent(\"execute\", " << lineNum << ", "
         << "nullptr, nullptr, nullptr);";
    }
    
    return ss.str();
}

bool LLVMInstrumentation::compileInstrumented(const std::string &sourceFile,
   const std::string &outputFile)
{
    // Use system compiler to compile instrumented source
    std::string command = "clang++ -c -fPIC -o " + outputFile + " " + sourceFile;

    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Compilation failed with code: " << result << "\n";
  return false;
    }
    
    return true;
}
```

---

### Task 2.6: Create Integration Tests

**File**: `tests/test_state_mapper.cpp`

```cpp
#include <gtest/gtest.h>
#include "analysis/state_mapping/execution_logger.h"
#include "analysis/state_mapping/state_mapper.h"

class StateMapperTest : public ::testing::Test {
protected:
    DataStructureInfo createTestGraph() {
  DataStructureInfo info;
      info.className = "TestGraph";
        info.memberFunctions = {"addNode", "addEdge"};
   return info;
    }
};

TEST_F(StateMapperTest, ConvertsEventsToStates) {
    ExecutionLogger logger;
    
    // Record some test events
    ExecutionEvent event1;
    event1.operation = "add_node";
    event1.lineNumber = 10;
    event1.variables["node_id"] = "1";
    event1.timestamp = 0;
    logger.recordEvent(event1);
 
    ExecutionEvent event2;
    event2.operation = "add_node";
    event2.lineNumber = 11;
    event2.variables["node_id"] = "2";
    event2.timestamp = 100;
    logger.recordEvent(event2);
    
  // Generate visualization states
    auto graph = createTestGraph();
    StateMapper mapper(graph);
    auto states = mapper.generateVisualizationStates(logger);
    
    EXPECT_EQ(states.size(), 2);
    EXPECT_EQ(states[0].stepNumber, 0);
    EXPECT_EQ(states[1].stepNumber, 1);
}

TEST_F(StateMapperTest, GeneratesValidDOTCode) {
 VisualizationState state;
    state.nodeData = R"([{"id": "1", "label": "Node 1"}])";
    state.edgeData = "[]";
    
    auto graph = createTestGraph();
    StateMapper mapper(graph);
    auto dotCode = mapper.generateDOTCode(state);
    
    EXPECT_NE(dotCode.find("digraph"), std::string::npos);
  EXPECT_NE(dotCode.find("Node 1"), std::string::npos);
}

TEST_F(StateMapperTest, ExecutionLoggerSerialization) {
    ExecutionLogger logger1;
    
    ExecutionEvent event;
    event.operation = "test_op";
    event.lineNumber = 5;
    event.variables["x"] = "10";
 logger1.recordEvent(event);
    
    // Save to file
    logger1.saveToFile("test_log.json");
    
    // Load in new logger
    ExecutionLogger logger2;
    ASSERT_TRUE(logger2.loadFromFile("test_log.json"));
    
    EXPECT_EQ(logger2.getEventCount(), 1);
    auto loaded = logger2.getEventAt(0);
    EXPECT_EQ(loaded->operation, "test_op");
    EXPECT_EQ(loaded->lineNumber, 5);
}
```

---

### Task 2.7: Create CMake Configuration

**File**: `src/analysis/state_mapping/CMakeLists.txt`

```cmake
# State mapping library

# Find dependencies
find_package(nlohmann_json REQUIRED)

# Add library
add_library(state_mapping
    execution_logger.cpp
    visualization_state.cpp
    state_mapper.cpp
)

# Link dependencies
target_link_libraries(state_mapping
    PUBLIC
     clang_integration
        nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(state_mapping
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Enable C++17
set_property(TARGET state_mapping PROPERTY CXX_STANDARD 17)
```

**File**: `src/analysis/instrumentation/CMakeLists.txt`

```cmake
# LLVM Instrumentation library

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Add library
add_library(llvm_instrumentation
    llvm_instrumentation.cpp
)

# Link LLVM
target_link_libraries(llvm_instrumentation
    PUBLIC
        clang_integration
        LLVM
)

# Include directories
target_include_directories(llvm_instrumentation
    PUBLIC
        ${LLVM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Enable C++17
set_property(TARGET llvm_instrumentation PROPERTY CXX_STANDARD 17)
```

---

## Testing Checklist

Before considering your work complete:

- [ ] `ExecutionLogger` records and retrieves events correctly
- [ ] Events serialize/deserialize from JSON without errors
- [ ] `StateMapper` converts events to visualization states
- [ ] `generateDOTCode()` produces valid Graphviz output
- [ ] Memory addresses tracked in `memoryState`
- [ ] Graph operations (add_node, add_edge) properly updated
- [ ] `LLVMInstrumentation` injects logging calls
- [ ] Instrumented code compiles without errors
- [ ] All unit tests pass (Task 2.6)
- [ ] No memory leaks in logger/mapper

---

## Expected Inputs from Developer 1

You'll receive:
1. Compiled `clang_integration` library
2. `DataStructureInfo` objects describing user's data structures
3. Example structure definitions (LinkedList, Graph, Tree, etc.)

**Use these to**:
- Identify which methods to instrument
- Understand data structure layout
- Generate appropriate visualization frames

---

## Expected Outputs for Next Teams

Provide the downstream teams:
1. Compiled `state_mapping` library
2. Compiled `llvm_instrumentation` library
3. Example execution logs (JSON format)
4. Example visualization states and DOT codes
5. Documentation for using `ExecutionLogger` and `StateMapper`

---

## Key Concepts

| Term | Meaning |
|------|---------|
| **Execution Event** | A single state change during program execution |
| **Visualization State** | A frame in the animation (snapshot of graph/memory) |
| **DOT Code** | Graphviz graph description language |
| **JSON Serialization** | Converting objects to/from JSON format |

---

## Performance Considerations

- **Event Recording**: Should be fast (<1ms overhead per event)
- **State Mapping**: May be slower (done offline after execution)
- **DOT Generation**: Cached when possible to avoid re-generation

---

## Debugging Tips

**Problem**: JSON parsing fails
- ? Validate JSON structure matches schema
- ? Check string escaping (quotes, backslashes)
- ? Use online JSON validator before saving

**Problem**: Graph structure not updating
- ? Verify operation name matches event parsing logic
- ? Check variable names are extracted correctly
- ? Print intermediate state for debugging

**Problem**: DOT code invalid for Graphviz
- ? Test with simple graph first
- ? Verify node IDs are valid (no special chars)
- ? Check quotes around labels

---

## Timeline

| Week | Task | Status |
|------|------|--------|
| Week 2 | Task 2.1-2.3: Event & State Models | ? |
| Week 2-3 | Task 2.4: State Mapper | ? |
| Week 3 | Task 2.5: LLVM Instrumentation | ? |
| Week 3-4 | Task 2.6-2.7: Testing & Integration | ? |
| Week 4 | Handoff to V2C Team | ? |

---

## Resource Links

- **JSON Library**: https://github.com/nlohmann/json
- **Graphviz DOT**: https://graphviz.org/doc/info/lang.html
- **LLVM Pass Development**: https://llvm.org/docs/WritingAnLLVMPass/
- **Thread-safe Logging**: https://en.cppreference.com/w/cpp/thread/mutex

---

**Questions?** Contact your team lead or refer to the main C2V_GUIDE.md and V2C_GUIDE.md.
