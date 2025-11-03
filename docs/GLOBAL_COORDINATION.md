# Global Coordination Guide: DataViz Development
**Complete Project Organization & Team Synchronization**

## Project Overview

**DataViz** is a comprehensive C++ data structure visualization system with real-time code editing capabilities. The project consists of 3 sub-teams working across 4 phases to deliver a complete integrated solution.

---

## Teams & Timeline

```
????????????????????????????????????????????????????????????????
? Week 1-3 ? Week 2-4 ? Week 4-6 ? Week 5-7 ? Week 7-8 ?
????????????????????????????????????????????????????????
? C2V Dev1 ? C2V Dev2 ? UI Dev1  ? UI Dev2  ? V2C Team ?
? Frontend ? Backend?Rendering ?Framework ? Integration
????????????????????????????????????????????????????????
```

---

## Team Assignments

### Team 1: C2V (Code-to-Visualization) - Weeks 1-4
**Goal**: Parse C++ code, instrument for execution, capture states

| Developer | Focus | Output |
|-----------|-------|--------|
| **C2V Dev 1** | AST Parsing & Structure Extraction | DataStructureInfo objects |
| **C2V Dev 2** | LLVM Instrumentation & State Mapping | VisualizationState objects |

**Deliverables**:
- ? Parsed C++ code structure inventory
- ? Instrumented executable capturing events
- ? JSON execution logs
- ? Frame-by-frame visualization states

**Coordination**: See [C2V_COORDINATION.md](./C2V_COORDINATION.md)

---

### Team 2: UI (User Interface) - Weeks 4-6
**Goal**: Build interactive visualization application

| Developer | Focus | Output |
|-----------|-------|--------|
| **UI Dev 1** | Graph Rendering & Animation | GraphWidget + AnimationController |
| **UI Dev 2** | Main Window & Framework Integration | Complete Qt Application |

**Deliverables**:
- ? Interactive graph visualization widget
- ? Frame-by-frame animation playback
- ? Qt main application with menus/controls
- ? Properties panel and console logging

**Coordination**: See [UI_COORDINATION.md](./UI_COORDINATION.md)

---

### Team 3: V2C (Visualization-to-Code) - Weeks 5-7
**Goal**: Enable code generation from visualization changes

| Developer | Focus |
|-----------|-------|
| **V2C Dev 1** | Code generation from user interactions |
| **V2C Dev 2** | Code rewriting and source file modification |

**Deliverables**:
- ? Convert graph operations to C++ code
- ? Modify source files using Clang Rewriter
- ? Bidirectional visualization ? code mapping

**Coordination**: See [V2C_COORDINATION.md](./V2C_COORDINATION.md) (if created)

---

## Data Flow Architecture

```
??????????????????????????????????????????????????????????
?            User's C++ Code   ?
?     (e.g., linked_list.cpp)        ?
????????????????????????????????????????????????????????
          ?
 ?
         ???????????????????????
     ?   C2V PIPELINE      ?
       ? (Weeks 1-4)   ?
         ?        ?
    ?????? Dev 1: Parse &      ?
    ?    ?   Extract           ?
    ?    ? Dev 2: Instrument & ?
    ?    ?   Map to States     ?
    ?    ???????????????????????
  ?
    ? DataStructureInfo
    ? + VisualizationState
    ? + ExecutionLog (JSON)
    ?
    ?
????????????????????????????????????????
?     UI PIPELINE (Weeks 4-6)          ?
?             ?
? Dev 1: Render Graphs        ?
? Dev 2: Main Window & Integration ?
?            ?
? ?????????????????????????????????   ?
? ?   Interactive DataViz App     ?   ?
? ?   ?   ?
? ? ???????????????????????????   ?   ?
? ? ?  Graph Visualization    ?   ?   ?
? ? ?  (Zoom, Pan, Select)    ?   ?   ?
? ? ?              ?   ?   ?
? ? ??????????????????????????? ? ?
? ? ? Play  Pause  Speed: 1x  ?   ?   ?
? ? ? Frame Slider: [=====>]  ?   ? ?
? ? ???????????????????????????   ?   ?
? ?????????????????????????????????   ?
????????????????????????????????????????
    ?
    ? User Interaction
    ? (Add Node, Edit Code)
    ?
    ?
????????????????????????????????????????
?     V2C PIPELINE (Weeks 5-7)     ?
?        ?
? Dev 1: Generate C++ Code       ?
? Dev 2: Rewrite Source Files          ?
????????????????????????????????????????
    ?
    ? Modified Source Code
    ?
    ?
        Cycle repeats...
```

---

## Shared Data Structures

### Critical Cross-Team Interfaces

**1. DataStructureInfo** (C2V Dev 1 ? C2V Dev 2, UI Dev 2)
```cpp
struct DataStructureInfo {
    std::string className;
    std::vector<std::string> memberVariables;
    std::vector<std::string> memberFunctions;
    bool isSelfReferential;
    std::string pointerMember;
};
```

**2. ExecutionEvent** (C2V Dev 2 ? UI)
```cpp
struct ExecutionEvent {
    int lineNumber;
    std::string operation;
  std::map<std::string, std::string> variables;
    std::map<std::string, std::string> addresses;
    uint64_t timestamp;
};
```

**3. VisualizationState** (C2V Dev 2 ? UI Dev 1)
```cpp
struct VisualizationState {
    int stepNumber;
    std::string nodeData;        // JSON array
    std::string edgeData;         // JSON array
    std::string dotCode;          // Graphviz DOT
    std::map<std::string, std::string> memoryState;
    std::map<std::string, std::string> metrics;
};
```

**4. RenderedGraph** (UI Dev 1 ? UI Dev 1)
```cpp
struct RenderedGraph {
    std::vector<RenderedNode> nodes;     // With x,y,width,height
    std::vector<RenderedEdge> edges;
    double width, height;
  bool isValid;
};
```

---

## Phase Breakdown

### Phase 1: C2V Analysis (Weeks 1-3)

**C2V Dev 1 Milestones**:
- Week 1: ASTParser working ?
- Week 2: StructureExtractor complete ?
- Week 3: Unit tests, documentation ?

**C2V Dev 2 Milestones**:
- Week 2: Event model and ExecutionLogger ?
- Week 3: StateMapper, instrumentation ?
- Week 4: Integration testing ?

**Phase 1 Deliverables**:
- ? C2V pipeline executable
- ? Test case execution logs (JSON)
- ? Sample visualization states

---

### Phase 2: UI Rendering (Weeks 4-6)

**Dependencies**: C2V pipeline completed

**UI Dev 1 Milestones**:
- Week 4: GraphvizRenderer complete ?
- Week 5: GraphWidget interactive ?
- Week 6: AnimationController working ?

**UI Dev 2 Milestones**:
- Week 4: Main window structure ?
- Week 5: Control panels functional ?
- Week 6: C2V pipeline integration ?

**Phase 2 Deliverables**:
- ? Standalone UI application
- ? Load C++ file ? visualize execution
- ? Export animation to video/frames

---

### Phase 3: V2C Synthesis (Weeks 5-7)

**Dependencies**: C2V completed, UI mostly done

**V2C Dev 1 Milestones**:
- Week 5: Code generation engine ?
- Week 6: API mapping service ?
- Week 7: Integration testing ?

**V2C Dev 2 Milestones**:
- Week 5: Clang Rewriter integration ?
- Week 6: Code modification working ?
- Week 7: Bidirectional sync testing ?

**Phase 3 Deliverables**:
- ? V2C pipeline executable
- ? Code generation from user actions
- ? Source file modification capability

---

### Phase 4: Integration & Polish (Weeks 6-8)

**All Teams**:
- Week 6: Full bidirectional pipeline testing
- Week 7: Performance optimization
- Week 8: Bug fixes, documentation, release

**Final Deliverables**:
- ? Complete DataViz application (executable)
- ? User documentation
- ? Developer documentation
- ? Example projects
- ? Unit test suite

---

## Build & CMake Structure

```
CMakeLists.txt (Root)
?? find_package(LLVM, Clang, Qt6, Graphviz)
?? add_subdirectory(src/analysis)
?  ?? src/analysis/clang_integration (Dev C2V-1)
?  ?? src/analysis/instrumentation (Dev C2V-2)
?  ?? src/analysis/state_mapping (Dev C2V-2)
?? add_subdirectory(src/ui)
?  ?? src/ui/visualization (Dev UI-1)
?  ?? src/ui (Dev UI-2)
?? add_subdirectory(src/synthesis)
?  ?? src/synthesis/code_generation (Dev V2C-1)
?  ?? src/synthesis/api_mapping (Dev V2C-1)
?  ?? src/synthesis/rewriting (Dev V2C-2)
?? add_subdirectory(tests)
   ?? tests/test_c2v (C2V team)
   ?? tests/test_ui (UI team)
   ?? tests/test_v2c (V2C team)
   ?? tests/test_integration (All teams)
```

**Build Order** (CMake handles automatically):
1. `clang_integration` library (C2V-1)
2. `state_mapping` + `instrumentation` (C2V-2, depends on #1)
3. `visualization` library (UI-1, depends on C2V's output)
4. Main UI executable (UI-2, depends on #3)
5. `synthesis` libraries (V2C, depends on #1-2)

---

## Testing Strategy

### Unit Testing (Each Team)
- C2V team: 50+ tests (parsing, extraction, instrumentation, mapping)
- UI team: 30+ tests (rendering, animation, controls)
- V2C team: 40+ tests (code generation, rewriting)

### Integration Testing

**C2V Integration**:
```
Parse ? Extract ? Instrument ? Execute ? Map States
  ?      ?          ?     ?        ?
```

**UI Integration**:
```
Load States ? Render ? Animate ? Display ? Interact
    ? ?        ??       ?
```

**V2C Integration**:
```
Interaction ? Generate Code ? Rewrite File ? Re-parse
   ?            ?        ?   ?
```

**Full System Integration**:
```
C++ File ? C2V Pipeline ? Visualization ? User Action
         ?
   V2C Pipeline ? Modified C++ File
            ?
     [Repeat]
```

### Regression Testing
- Run full test suite on each commit
- Automated testing via CI/CD (GitHub Actions recommended)
- Manual acceptance testing for UI components

---

## Development Workflow

### Git Repository Structure

```
dataviz-uit/
?? .gitignore
?? CMakeLists.txt
?? README.md
?? CONTRIBUTING.md
?? docs/
?  ?? GLOBAL_COORDINATION.md    ? This file
?  ?? C2V_COORDINATION.md
?  ?? C2V_DEVELOPER_1_FRONTEND.md
?  ?? C2V_DEVELOPER_2_BACKEND.md
?  ?? UI_COORDINATION.md
?  ?? UI_DEVELOPER_1_RENDERING.md
?  ?? UI_DEVELOPER_2_FRAMEWORK.md
?  ?? V2C_GUIDE.md
?  ?? API_REFERENCE.md
?? src/
?  ?? analysis/     (C2V team)
?  ?  ?? clang_integration/     (C2V Dev 1)
?  ?  ?? instrumentation/         (C2V Dev 2)
?  ?  ?? state_mapping/         (C2V Dev 2)
?  ?? ui/         (UI team)
?  ?  ?? visualization/      (UI Dev 1)
?  ?  ?? [main window code]     (UI Dev 2)
?  ?? synthesis/         (V2C team)
?  ?? code_generation/     (V2C Dev 1)
?     ?? api_mapping/           (V2C Dev 1)
?     ?? rewriting/               (V2C Dev 2)
?? tests/
?  ?? test_c2v_pipeline.cpp
?  ?? test_ui_integration.cpp
?  ?? test_v2c_pipeline.cpp
?  ?? test_full_system.cpp
?? examples/
?  ?? linked_list.cpp
?  ?? binary_tree.cpp
?  ?? graph.cpp
?? build/           (Generated by CMake)
```

### Branching Strategy

```
main (stable releases)
  ?
  ?
  ?? c2v-dev (C2V team development)
  ?  ?? c2v-dev-1-frontend
  ?  ?? c2v-dev-2-backend
  ?
  ?? ui-dev (UI team development)
  ?  ?? ui-dev-1-rendering
  ?  ?? ui-dev-2-framework
  ?
  ?? v2c-dev (V2C team development)
     ?? v2c-dev-1-codegen
     ?? v2c-dev-2-rewriting
```

### Commit Message Convention

```
[TEAM] Scope: Brief description

More detailed description if needed.

Fixes #issue_number
```

Examples:
- `[C2V] Parser: Handle template specializations`
- `[UI] GraphWidget: Fix memory leak in renderNodes()`
- `[V2C] CodeGen: Generate vector.push_back() calls`

---

## Communication & Meetings

### Daily Standup (Async - Team Channels)
- **What**: Status update (done, doing, blockers)
- **When**: Morning (9 AM your timezone)
- **Where**: Team Slack/Discord channel

### Weekly Sync (Video - All Teams)
- **When**: Fridays 2 PM UTC
- **Duration**: 30 minutes
- **Agenda**:
  1. Each team: 3-min progress update
  2. Cross-team dependencies check
  3. Blockers & help needed
  4. Plan next week

### Bi-Weekly Architecture Review
- **When**: Every other Wednesday 3 PM UTC
- **Duration**: 60 minutes
- **Participants**: Team leads + tech lead
- **Topics**: Design decisions, integration planning, tech debt

---

## Dependency Management

### Critical Dependencies

| Phase | Dependency | Provider | Consumer | Date Due |
|-------|-----------|----------|----------|----------|
| 1 | DataStructureInfo | C2V-1 | C2V-2 | Week 1 EOD |
| 1 | clang_integration lib | C2V-1 | C2V-2, UI-2 | Week 2 EOD |
| 2 | VisualizationState | C2V-2 | UI-1, UI-2 | Week 2 EOD |
| 2 | GraphWidget | UI-1 | UI-2 | Week 5 EOD |
| 3 | UI App | UI-2 | V2C-1, V2C-2 | Week 6 EOD |
| 3 | V2C Pipeline | V2C | Integration | Week 7 EOD |

### Dependency Escalation

If a team is blocked by missing dependency:
1. **1-2 hours**: Try workaround/mock implementation
2. **2-4 hours**: Ping dependency provider in Slack
3. **4+ hours**: Escalate to tech lead
4. **8+ hours**: Tech lead may re-prioritize tasks

---

## Code Quality Standards

### All Teams Must Maintain

- **Language**: C++17 (C++20 optional)
- **Formatting**: clang-format (LLVM style)
- **Linting**: clang-tidy
- **Compilation**: Zero warnings with `-Wall -Wextra -Werror`
- **Tests**: >80% code coverage
- **Documentation**: Doxygen-compatible comments

### Pre-Commit Checklist

- [ ] Code compiles without warnings
- [ ] All tests pass locally
- [ ] clang-format applied
- [ ] No memory leaks (valgrind/asan)
- [ ] Commit message follows convention
- [ ] Related documentation updated

---

## Performance Targets

| Component | Target | Measurement |
|-----------|--------|-------------|
| C2V Parse | <1s | For 1000-line file |
| C2V Instrumentation | <5s | Compile instrumented code |
| Execution Capture | <10% overhead | vs. native execution |
| State Mapping | <100ms | For 10k execution events |
| UI Render | 60 FPS | 60 frames/second |
| Graph Display | <50ms | For 1000 nodes |
| V2C Code Gen | <100ms | Per user action |
| V2C Rewrite | <500ms | Modify source file |

---

## Risk Management

### Technical Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|-----------|
| Clang API changes | Medium | High | Lock to LLVM 14.x LTS |
| Qt MOC issues | Low | Medium | Use Q_OBJECT properly, test early |
| Large graph rendering | Medium | Medium | Implement LOD (Level of Detail) |
| Bidirectional sync issues | High | High | Extensive integration testing |

### Schedule Risks

| Risk | Mitigation |
|------|-----------|
| Dependency delays | Build with mocks, parallel dev |
| Scope creep | Strict feature freeze after Week 5 |
| Integration issues | Daily builds, continuous testing |

---

## Success Criteria

### C2V Team
- ? Parse arbitrary C++ structures
- ? Capture execution with <10% overhead
- ? Generate valid visualization states
- ? All unit tests pass

### UI Team
- ? Display 1000+ node graphs smoothly
- ? Animate at 60 FPS
- ? Responsive controls (<100ms latency)
- ? Export animations in multiple formats

### V2C Team
- ? Generate valid C++ from graph ops
- ? Modify source files correctly
- ? Maintain code formatting
- ? Bidirectional sync working

### System
- ? Complete end-to-end demo working
- ? Handles multiple test cases
- ? Comprehensive documentation
- ? No critical bugs

---

## Resources & Documentation

### External Resources
- [LLVM Project](https://llvm.org/)
- [Clang Documentation](https://clang.llvm.org/)
- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [Graphviz](https://graphviz.org/)
- [CMake Documentation](https://cmake.org/documentation/)

### Internal Documentation
- [C2V Team Coordination](./C2V_COORDINATION.md)
- [UI Team Coordination](./UI_COORDINATION.md)
- [C2V Developer 1 Guide](./C2V_DEVELOPER_1_FRONTEND.md)
- [C2V Developer 2 Guide](./C2V_DEVELOPER_2_BACKEND.md)
- [UI Developer 1 Guide](./UI_DEVELOPER_1_RENDERING.md)
- [UI Developer 2 Guide](./UI_DEVELOPER_2_FRAMEWORK.md)
- [V2C Team Guide](./V2C_GUIDE.md)

---

## Troubleshooting

### "CMake configuration failed"
- [ ] Verify LLVM_DIR is set correctly
- [ ] Check all dependencies installed
- [ ] Try: `cmake -DCMAKE_TOOLCHAIN_FILE=...`

### "Tests failing after merge"
- [ ] Pull latest `main` and rebuild
- [ ] Check new dependencies added
- [ ] Run `ctest --verbose` for details

### "Merge conflicts on docs"
- [ ] Prefer latest version
- [ ] Keep both versions if both teams modified
- [ ] Ping team lead if unsure

---

## Release Checklist

Before each milestone release:

- [ ] All tests passing (unit + integration)
- [ ] Documentation complete and accurate
- [ ] Code reviewed and merged to main
- [ ] Performance benchmarks within targets
- [ ] No critical/high bugs open
- [ ] Build artifacts created
- [ ] Release notes written
- [ ] Version number bumped

---

## Appendix: Key Contacts

| Role | Team | Contact |
|------|------|---------|
| Tech Lead | All | [TBD] |
| C2V Lead | C2V | [Dev 1 or 2] |
| UI Lead | UI | [Dev 1 or 2] |
| V2C Lead | V2C | [Dev 1 or 2] |
| QA Lead | Testing | [TBD] |

---

## Document History

| Date | Author | Change |
|------|--------|--------|
| [Current] | Tech Lead | Initial version |
| - | - | - |

---

**Last Updated**: [Current Date]  
**Maintained By**: Tech Lead  
**Confidentiality**: Internal Use Only  
**Status**: ? Active Development

---

## Quick Start for New Team Members

1. **Clone repository**: `git clone https://github.com/YassineBibrine/dataviz-uit.git`
2. **Read this document** (you're here!)
3. **Read your team's coordination guide** (C2V/UI/V2C)
4. **Read your developer guide** (Developer 1/2)
5. **Set up development environment** (see README.md)
6. **Join team Slack channel**
7. **Attend weekly sync** (Fridays 2 PM UTC)
8. **Start implementing!** ??

---

**Welcome to Team DataViz!** ??
