# C2V Team Coordination Guide
**Code-to-Visualization Pipeline Coordination**

## Overview

The C2V Team consists of **2 developers** working on sequential phases of the code analysis and state mapping pipeline. This document coordinates their efforts to ensure seamless integration.

---

## Team Structure

| Role | Developer | Primary Focus | Timeline |
|------|-----------|---------------|----------|
| **C2V Dev 1** | Frontend Dev | AST Parsing & Structure Extraction | Weeks 1-3 |
| **C2V Dev 2** | Backend Dev | LLVM Instrumentation & State Mapping | Weeks 2-4 |

---

## Detailed Responsibilities

### Developer 1: Frontend & AST Parsing

**Weeks 1-3**

#### Core Deliverables
1. **ASTParser** (`src/analysis/clang_integration/ast_parser.h/.cpp`)
   - Parse C++ files using Clang
   - Load compilation databases
   - Provide AST context

2. **StructureExtractor** (`src/analysis/clang_integration/structure_extractor.h/.cpp`)
   - Identify data structures (classes, structs)
   - Extract method signatures
   - Detect linked structures (self-referential pointers)

3. **CMake Integration** (`src/analysis/clang_integration/CMakeLists.txt`)
   - Link LLVM/Clang libraries
   - Configure compilation flags

#### Key Outputs (for Dev 2)
```cpp
struct DataStructureInfo {
 std::string className;
    std::vector<std::string> memberVariables;
    std::vector<std::string> memberFunctions;
    bool isSelfReferential;
    std::string pointerMember;
};
```

#### Milestones
- **Week 1**: ASTParser working, can parse simple C++ files
- **Week 2**: StructureExtractor identifies all data structures
- **Week 3**: Unit tests pass, documentation complete

---

### Developer 2: Backend & State Mapping

**Weeks 2-4**

#### Core Deliverables
1. **ExecutionLogger** (`src/analysis/state_mapping/execution_logger.h/.cpp`)
   - Record execution events
   - Serialize/deserialize from JSON
   - Provide thread-safe logging

2. **StateMapper** (`src/analysis/state_mapping/state_mapper.h/.cpp`)
   - Convert execution events to visualization frames
   - Generate Graphviz DOT code
   - Calculate performance metrics

3. **LLVMInstrumentation** (`src/analysis/instrumentation/llvm_instrumentation.h/.cpp`)
 - Inject logging callbacks into code
   - Compile instrumented binaries

4. **VisualizationState Model** (`src/analysis/state_mapping/visualization_state.h/.cpp`)
   - Define frame structure
   - Support JSON serialization

#### Key Dependencies (from Dev 1)
- `DataStructureInfo` structure
- Compiled `clang_integration` library
- List of methods to instrument

#### Milestones
- **Week 2**: Event model and logger complete
- **Week 3**: State mapper converts events to frames
- **Week 4**: Instrumentation working, integration tests pass

---

## Integration Points

### Handoff 1: Dev 1 ? Dev 2 (End of Week 1)

**What Dev 1 Provides**:
- ? Working ASTParser that can parse C++ files
- ? Example `DataStructureInfo` outputs
- ? Documentation showing how to use ASTParser
- ? Compiled `clang_integration` library

**What Dev 2 Needs**:
- ? `DataStructureInfo` structure definition
- ? Understanding of available methods in data structures
- ? LLVM setup validation

**Action Items**:
- [ ] Dev 1 provides test files and expected outputs
- [ ] Dev 2 verifies LLVM can be linked
- [ ] Both verify CMakeLists.txt compile successfully

---

### Handoff 2: Dev 1 ? Dev 2 (End of Week 2)

**What Dev 1 Provides**:
- ? Complete StructureExtractor
- ? Unit tests for structure extraction
- ? List of all detected structures in test code
- ? Compiled `clang_integration` library

**What Dev 2 Uses**:
- ? DataStructureInfo for all user structures
- ? Method signatures to instrument
- ? Linked structure information for memory visualization

**Action Items**:
- [ ] Dev 2 builds StateMapper that uses DataStructureInfo
- [ ] Dev 2 creates test execution logs
- [ ] Both run integration tests

---

### Integration: Dev 1 + Dev 2 (Week 3-4)

**C2V Pipeline Flow**:
```
User's C++ Code
    ?
???????????????????
?  Dev 1: AST     ? Parse & Extract Structures
?  Parser & Ext.  ?
???????????????????
? (DataStructureInfo)
???????????????????????????
?  Dev 2: Instrumentation ? Inject Logging
?  & State Mapping        ?
???????????????????????????
         ?
 Instrumented Code
         ?
    Execute
         ?
Execution Log (JSON)
         ?
????????????????????
?  State Mapper    ? Convert to Frames
????????????????????
   ?
Animation Sequence (DOT)
    ?
    UI Layer
```

---

## Data Exchange Formats

### DataStructureInfo (Dev 1 ? Dev 2)
```json
{
  "className": "LinkedList",
  "memberVariables": ["head", "tail", "size"],
  "memberFunctions": ["insert", "remove", "search"],
  "isSelfReferential": true,
  "pointerMember": "next"
}
```

### ExecutionEvent (Dev 2 ? UI)
```json
{
  "lineNumber": 42,
  "operation": "add_node",
  "variables": {
    "node_id": "1",
    "node_value": "10"
  },
  "addresses": {
    "node_ptr": "0x7ffee1234567"
  },
  "timestamp": 1000000
}
```

### VisualizationState (Dev 2 ? UI)
```json
{
  "stepNumber": 0,
  "nodeData": "[{\"id\": \"1\", \"label\": \"Node 1\"}]",
  "edgeData": "[{\"from\": \"1\", \"to\": \"2\"}]",
  "dotCode": "digraph { \"1\" -> \"2\"; }",
  "operationDescription": "Added node 1"
}
```

---

## Common Tasks

### Code Review Checklist

**Dev 1 Deliverables**:
- [ ] Code compiles without warnings
- [ ] All methods in header have implementations
- [ ] Unit tests cover main functionality
- [ ] Documentation explains key algorithms
- [ ] Example outputs provided
- [ ] Error handling included
- [ ] Memory management correct (no leaks)

**Dev 2 Deliverables**:
- [ ] Code compiles without warnings
- [ ] Correctly uses DataStructureInfo from Dev 1
- [ ] Unit tests verify end-to-end pipeline
- [ ] JSON serialization/deserialization works
- [ ] Performance acceptable (<100ms per operation)
- [ ] Memory addresses correctly captured
- [ ] DOT code is valid and renders

---

## Dependency Management

### Required Libraries

| Library | Version | Used By |
|---------|---------|---------|
| LLVM | 14+ | Dev 1, Dev 2 |
| Clang | 14+ | Dev 1, Dev 2 |
| nlohmann_json | 3.11+ | Dev 2 |
| GoogleTest | Any | Both (testing) |

### CMake Configuration

**Build Order**:
1. `src/analysis/clang_integration/` (Dev 1)
2. `src/analysis/state_mapping/` (Dev 2 - depends on #1)
3. `src/analysis/instrumentation/` (Dev 2 - depends on #1)

**Master CMakeLists.txt** (`src/analysis/CMakeLists.txt`):
```cmake
add_subdirectory(clang_integration)
add_subdirectory(state_mapping)
add_subdirectory(instrumentation)
```

---

## Communication Protocol

### Daily Standup (Async)
- Post status in team channel
- Note blockers
- Highlight any interface changes

### Weekly Sync (Video Call)
- Review progress against timeline
- Discuss integration issues
- Plan next phase

### Integration Testing
- Run full C2V pipeline on test cases
- Verify data flows correctly
- Check performance metrics

---

## Testing Strategy

### Unit Testing

**Dev 1 Tests** (`tests/test_ast_parser.cpp`, `tests/test_structure_extractor.cpp`):
```
? Can parse simple C++ files
? Extracts all data structures
? Detects self-referential pointers
? Handles compilation errors gracefully
```

**Dev 2 Tests** (`tests/test_execution_logger.cpp`, `tests/test_state_mapper.cpp`):
```
? Records and retrieves events
? Serializes events to JSON
? Maps events to visualization states
? Generates valid DOT code
```

### Integration Testing

**Full Pipeline Test** (`tests/test_c2v_pipeline.cpp`):
```cpp
// Given: User's C++ code with graph structure
// When: Running C2V pipeline
// Then: Should produce valid animation sequence

// Step 1: Parse code (Dev 1)
auto structures = extractor.extractStructures();

// Step 2: Instrument and execute
auto logger = executeInstrumented(structures);

// Step 3: Map to visualization (Dev 2)
auto states = mapper.generateVisualizationStates(logger);

// Verify: DOT codes are valid
for (const auto &state : states) {
    EXPECT_FALSE(state.dotCode.empty());
    EXPECT_NE(state.dotCode.find("digraph"), string::npos);
}
```

---

## Error Handling

### Common Issues & Solutions

| Issue | Root Cause | Solution |
|-------|-----------|----------|
| AST context is null | File not parsed | Dev 1: Verify file syntax, compilation flags |
| Structure not found | Extractor not run | Dev 1: Check AST matcher regex |
| Events not logged | Instrumentation failed | Dev 2: Verify method names match extracted ones |
| Invalid DOT code | Node IDs have special chars | Dev 2: Add escaping for IDs |

### Escalation Path
1. Check relevant guide (C2V_DEVELOPER_1/2.md)
2. Review test output and error messages
3. Post in team channel with:
   - Error message
   - Code snippet
   - Steps to reproduce
4. Escalate to tech lead if blocked >2 hours

---

## Timeline & Milestones

```
Week 1
  ?? Dev 1: Implement ASTParser
  ?? Dev 2: Set up environment, review Clang docs
  ?? Handoff 1: ASTParser working

Week 2
  ?? Dev 1: Implement StructureExtractor, finalize extraction
  ?? Dev 2: Implement ExecutionLogger & VisualizationState
  ?? Handoff 2: DataStructureInfo complete

Week 3
  ?? Dev 1: Testing & documentation
  ?? Dev 2: Implement StateMapper & Instrumentation
  ?? Integration: First full pipeline test
  ?? Handoff 3: All components ready

Week 4
  ?? Dev 1: Available for support/debugging
  ?? Dev 2: Finalize instrumentation, performance tuning
  ?? Integration: Full pipeline working
  ?? Handoff 4: C2V complete, ready for V2C team
```

---

## Deliverables Checklist

### Dev 1 - Final Deliverables
- [ ] ASTParser fully functional
- [ ] StructureExtractor with self-referential detection
- [ ] CMakeLists.txt with LLVM/Clang linking
- [ ] 100% unit test coverage
- [ ] Code documentation (inline comments + README)
- [ ] Example C++ files for testing
- [ ] DataStructureInfo documentation

### Dev 2 - Final Deliverables
- [ ] ExecutionLogger with JSON serialization
- [ ] VisualizationState model
- [ ] StateMapper with DOT generation
- [ ] LLVMInstrumentation for logging injection
- [ ] 100% unit test coverage
- [ ] Full C2V pipeline integration test
- [ ] Example execution logs and animation sequences

---

## Next Phase: Handoff to V2C Team

**What V2C Team Receives**:
1. Compiled C2V pipeline (parser + instrumentation + state mapping)
2. Example execution logs and visualization states
3. Detailed documentation of DataStructureInfo and VisualizationState
4. Integration test demonstrating full pipeline

**Interface to V2C**:
- V2C consumes `VisualizationState` objects
- V2C implements code generation from visualization changes
- Bidirectional mapping maintained through consistent data structures

---

## References

- [C2V Developer 1 Guide](./C2V_DEVELOPER_1_FRONTEND.md)
- [C2V Developer 2 Guide](./C2V_DEVELOPER_2_BACKEND.md)
- [Main C2V Guide](./C2V_GUIDE.md)
- [Global Coordination Guide](./GLOBAL_COORDINATION.md)

---

**Last Updated**: [Current Date]  
**Maintained By**: Team Lead  
**Next Review**: End of Week 2
