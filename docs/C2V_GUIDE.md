# C2V Team (Code-to-Visualization) Implementation Guide

## Overview

The C2V Team builds the **Analysis Engine**, responsible for parsing C++ source code, extracting data structures, instrumenting code for runtime execution capture, and mapping execution states to visualization representations.

## Team Responsibilities

### Primary Responsibilities
1. **AST Parsing** - Parse C++ code using Clang/LLVM
2. **Structure Extraction** - Identify data structures (graphs, trees, linked lists)
3. **Runtime Instrumentation** - Inject logging callbacks via LLVM passes
4. **State Logging** - Capture execution events during program execution
5. **State Mapping** - Convert execution logs to visualization states

### File Locations
All analysis files are located in: `src/analysis/`

```
src/analysis/
??? CMakeLists.txt
??? clang_integration/
?   ??? ast_parser.h/.cpp         ? Start here
?   ??? structure_extractor.h/.cpp
??? instrumentation/
?   ??? llvm_instrumentation.h/.cpp
??? state_mapping/
    ??? execution_logger.h/.cpp
    ??? state_mapper.h/.cpp
```

## Component Details

### 1. ASTParser (Clang/LLVM Integration)
**File**: `src/analysis/clang_integration/ast_parser.h/.cpp`

**Responsibilities**:
- Load C++ source files
- Parse into Abstract Syntax Tree (AST) using Clang
- Handle compilation database for complex projects
- Provide AST context to other components

**Key Methods to Implement**:
```cpp
bool parseFile(const std::string &filePath,
    const std::vector<std::string> &compilationFlags = {});
    
bool parseCompilationDatabase(const std::string &databasePath);

clang::ASTContext *getASTContext();
```

**Implementation Steps**:

```cpp
// Step 1: Create CompilerInstance
clang::CompilerInstance compiler;

// Step 2: Set compilation options
auto &opts = compiler.getInvocation().getCodeGenOpts();
opts.OptimizationLevel = 2;

// Step 3: Initialize diagnostics
auto DiagClient = new clang::DiagnosticConsumer();

// Step 4: Parse the file
compiler.createASTContext();

// Step 5: Return AST context for next stage
clang::ASTContext *ctx = &compiler.getASTContext();
```

**LLVM/Clang Concepts**:
- **CompilerInstance**: Main interface to Clang compiler
- **ASTContext**: Provides context for AST operations
- **CompilationDatabase**: Reads compile_commands.json for build flags
- **SourceManager**: Manages source file locations
- **Diagnostic**: Error/warning reporting

**Key Headers to Include**:
```cpp
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/ASTMatchers/ASTMatchers.h"
```

### 2. StructureExtractor (LibASTMatchers)
**File**: `src/analysis/clang_integration/structure_extractor.h/.cpp`

**Responsibilities**:
- Identify user-defined data structures
- Find linked structures (linked lists, trees, graphs)
- Extract method signatures
- Catalog structure properties

**Key Methods to Implement**:
```cpp
std::vector<DataStructureInfo> extractStructures();

DataStructureInfo extractStructure(const std::string &className);

bool isLinkedStructure(const DataStructureInfo &info);
```

**Implementation Approach**:

```cpp
// Use LibASTMatchers to find classes/structs
using namespace clang::ast_matchers;

// Find all CXX record declarations
auto classMatcher = cxxRecordDecl(
 isDefinition(),
    hasMethod(hasName("insert")).bind("insertMethod"),
    hasMethod(hasName("remove")).bind("removeMethod")
);

// Find self-referential pointers (indicator of linked structures)
auto linkedNodeMatcher = cxxRecordDecl(
    hasField(
        fieldDecl(hasType(pointerType(pointsTo(
   cxxRecordDecl().bind("self")
     ))))
    )
);

// Execute matchers
MatchFinder finder;
MatchCallback callback;
finder.addMatcher(classMatcher, &callback);
finder.matchAST(astContext);
```

**Output Structure** (`DataStructureInfo`):
```cpp
struct DataStructureInfo {
    std::string className;          // e.g., "LinkedList"
    std::vector<std::string> memberVariables;  // e.g., {"head", "tail"}
    std::vector<std::string> memberFunctions;  // e.g., {"insert", "remove"}
    bool isSelfReferential;  // true if linked structure
    std::string pointerMember;      // e.g., "next"
};
```

**Key Headers to Include**:
```cpp
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
```

### 3. LLVMInstrumentation (Runtime State Capture)
**File**: `src/analysis/instrumentation/llvm_instrumentation.h/.cpp`

**Responsibilities**:
- Create LLVM passes for code instrumentation
- Inject callbacks for state logging
- Insert logging at critical points (assignments, pointer operations, function calls)
- Compile instrumented code

**Key Methods to Implement**:
```cpp
bool instrumentCode(const std::string &sourceFile,
    const std::string &outputFile);
    
bool createInstrumentationPass();
```

**Implementation Approach** (High-Level):

```cpp
// 1. Parse source to LLVM IR
llvm::LLVMContext context;
auto moduleOrErr = llvm::parseIRFile(sourceFile, diag, context);

// 2. Create custom LLVM Pass
struct InstrumentationPass : public llvm::BasicBlockPass {
    virtual bool runOnBasicBlock(llvm::BasicBlock &BB) override {
        for (auto &Inst : BB) {
   // Insert logging before assignment
            if (auto *assign = llvm::dyn_cast<llvm::StoreInst>(&Inst)) {
       insertLoggingCall(assign);
     }
     // Insert logging for function calls
         if (auto *call = llvm::dyn_cast<llvm::CallInst>(&Inst)) {
     insertLoggingCall(call);
            }
        }
        return true;
    }
};

// 3. Apply pass to module
llvm::PassManager PM;
PM.add(new InstrumentationPass());
PM.run(*module);

// 4. Emit instrumented code
llvm::raw_fd_ostream dest(outputFile, EC, llvm::sys::fs::OF_None);
llvm::WriteBitcodeToFile(*module, dest);
```

**Instrumentation Points**:
- Variable assignments
- Pointer dereferences
- Memory allocations
- Function calls on data structures
- Control flow changes

**Key Concepts**:
- **LLVM IR** (Intermediate Representation): Low-level, portable code
- **BasicBlock**: Sequence of instructions with no branches
- **Instruction**: Individual operations (store, load, call, etc.)
- **Pass**: Transformation or analysis of LLVM IR

**Key Headers to Include**:
```cpp
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
```

### 4. ExecutionLogger (Event Capture)
**File**: `src/analysis/state_mapping/execution_logger.h/.cpp`

**Responsibilities**:
- Record execution events from instrumented code
- Serialize events to persistent log
- Provide event queries for visualization

**Key Methods to Implement**:
```cpp
void recordEvent(const ExecutionEvent &event);

void loadFromFile(const std::string &logFilePath);

void saveToFile(const std::string &logFilePath) const;

const ExecutionEvent *getEventAt(size_t index) const;
```

**ExecutionEvent Structure**:
```cpp
struct ExecutionEvent {
    int lineNumber;          // Source line where change occurred
    std::string operation;           // "assign", "add_node", "call", etc.
    std::map<std::string, std::string> variables;  // var_name -> value
    std::map<std::string, std::string> addresses;  // var_name -> memory_address
    long long timestamp;             // Nanoseconds since execution start
};
```

**Implementation Notes**:
- Events should be captured at high frequency (microsecond granularity)
- Store variable values as strings for universal representation
- Record memory addresses for pointer visualization
- Use efficient serialization (binary or RapidJSON)

**Serialization Format** (JSON Example):
```json
{
  "events": [
    {
      "lineNumber": 42,
    "operation": "assign",
      "variables": {
"x": "10",
 "y": "20"
      },
      "addresses": {
        "list_ptr": "0x7ffee1234567"
      },
      "timestamp": 1000000
    }
  ]
}
```

### 5. StateMapper (Event-to-Visualization Conversion)
**File**: `src/analysis/state_mapping/state_mapper.h/.cpp`

**Responsibilities**:
- Convert execution events to visualization states
- Generate Graphviz DOT code from states
- Create memory layout representations
- Compute performance metrics

**Key Methods to Implement**:
```cpp
std::vector<VisualizationState> generateVisualizationStates(
    const ExecutionLogger &logger,
    const std::string &dataStructureType);
    
std::string generateDOTCode(const VisualizationState &state);

std::string generateMemoryViewData(const VisualizationState &state);
```

**VisualizationState Structure**:
```cpp
struct VisualizationState {
    int stepNumber;    // Frame number in animation
    std::string nodeData;            // JSON array of nodes
    std::string edgeData;      // JSON array of edges
    std::map<std::string, std::string> memoryState;  // Memory layout
    std::map<std::string, std::string> metrics;   // Perf metrics
};
```

**Implementation Example** (for Linked List):
```cpp
// Input: Execution event showing node insertion
// {
//   "operation": "insert_node",
//   "variables": { "new_node_id": "3", "prev_id": "2" },
//   "addresses": { "new_node_ptr": "0x...", "prev_ptr": "0x..." }
// }

// Output: Visualization State with DOT code
// digraph {
//     node1 [label="1"];
//     node2 [label="2"];
//     node3 [label="3"];
//     node1 -> node2;
//     node2 -> node3;
// }
```

**DOT Code Generation**:
```cpp
std::string StateMapper::generateDOTCode(const VisualizationState &state) {
    std::stringstream ss;
    ss << "digraph {\n";
    
// Parse nodeData JSON and generate DOT nodes
    for (const auto &node : parseNodes(state.nodeData)) {
        ss << "  \"" << node.id << "\" [label=\"" << node.label << "\"];\n";
    }
    
    // Parse edgeData JSON and generate DOT edges
    for (const auto &edge : parseEdges(state.edgeData)) {
        ss << "  \"" << edge.from << "\" -> \"" << edge.to << "\";\n";
    }
    
    ss << "}\n";
 return ss.str();
}
```

## C2V Pipeline Overview

```
User's C++ Code
    ?
    ?
????????????????????????
?  1. AST Parser       ?  Parse source code into AST
????????????????????????
    ?
    ?
????????????????????????
?  2. Structure     ?  Identify data structures
?     Extractor        ?  (graphs, trees, linked lists)
????????????????????????
    ?
    ?
????????????????????????
?  3. LLVM Instrm.  ?  Inject runtime logging calls
?     Engine   ?  Compile instrumented binary
????????????????????????
    ?
    ?
 Execute Instrumented
   Program
    ?
    ?
Execution Log (events)
    ?
    ?
????????????????????????
?  4. Execution  ?  Parse and store execution log
?     Logger           ?
????????????????????????
    ?
    ?
????????????????????????
?5. State Mapper     ?  Convert events to frames
?      ?  Generate DOT code per frame
????????????????????????
    ?
    ?
Animation Sequence
 (Array of DOT codes)
    ?
    ?
Visualization Engine
    ?
    ?
Interactive Visualization
```

## Integration Checklist

- [ ] Clang AST parsing working
- [ ] Can load compilation database
- [ ] Structure extraction identifies data structures
- [ ] Linked structure detection working
- [ ] LLVM pass compiles successfully
- [ ] Instrumented code executes without crash
- [ ] Execution events logged correctly
- [ ] State mapper generates valid DOT code
- [ ] Performance metrics calculated
- [ ] Memory addresses captured

## Build Instructions

### Dependencies Setup
```bash
# Ubuntu/Debian
sudo apt-get install llvm-14-dev clang-14 libclang-14-dev

# macOS
brew install llvm

# Windows
# Download from https://releases.llvm.org/
```

### Build
```bash
cd build
cmake -DLLVM_DIR=/path/to/llvm/cmake ..
cmake --build . --config Release
```

## Testing

### Test AST Parser
```cpp
ASTParser parser;
if (parser.parseFile("test.cpp")) {
    clang::ASTContext *ctx = parser.getASTContext();
    // Verify AST context is valid
}
```

### Test Structure Extraction
```cpp
StructureExtractor extractor(astContext);
auto structures = extractor.extractStructures();
assert(!structures.empty());
```

### Test Instrumentation
```cpp
LLVMInstrumentation instr;
bool success = instr.instrumentCode("source.cpp", "instrumented.bc");
assert(success);
```

## Useful Resources

- Clang Documentation: https://clang.llvm.org/docs/
- LLVM Tutorial: https://llvm.org/docs/tutorial/
- LibASTMatchers: https://clang.llvm.org/docs/LibASTMatchers.html
- LLVM IR Language: https://llvm.org/docs/LangRef/

## Common Issues

**Issue**: Clang headers not found
- Ensure LLVM_DIR points to LLVM installation
- Check CMakeLists.txt LLVM configuration

**Issue**: AST context is null
- Verify source file syntax is correct
- Check compilation flags are proper

**Issue**: Instrumentation pass doesn't execute
- Verify LLVM IR is valid
- Check PassManager configuration

## Next Steps

1. **Implement ASTParser**: Get basic Clang integration working
2. **Implement StructureExtractor**: Identify data structures in AST
3. **Implement LLVMInstrumentation**: Create LLVM pass for logging
4. **Implement ExecutionLogger**: Capture events from instrumented code
5. **Implement StateMapper**: Convert events to visualization states
6. **Integration Testing**: Test complete C2V pipeline

## Contact

Questions about C2V implementation? Refer to `README.md` or check team communications.
