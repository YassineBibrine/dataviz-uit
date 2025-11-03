# ?? DataViz Documentation Quick Reference Card
**Laminate or Pin This - Your One-Page Guide to All Documentation**

---

## ?? ROLE-BASED QUICK START

### ?? **I'm a C2V Frontend Developer (AST Parsing)**
```
READ:   INDEX.md ? GLOBAL ? C2V_COORDINATION ? C2V_DEVELOPER_1_FRONTEND
CODE:   src/analysis/clang_integration/
WEEKS:  1-3
TASKS:Parse C++ ? Extract structures ? Detect linked structures
OUTPUT: DataStructureInfo for Dev 2 + UI team
```

### ?? **I'm a C2V Backend Developer (Instrumentation)**
```
READ:   INDEX.md ? GLOBAL ? C2V_COORDINATION ? C2V_DEVELOPER_2_BACKEND
CODE:   src/analysis/state_mapping/ + src/analysis/instrumentation/
WEEKS:  2-4
TASKS:  Inject logging ? Record events ? Map to visualization frames
OUTPUT: VisualizationState + execution logs for UI team
```

### ?? **I'm a UI Rendering Developer (Graph Display)**
```
READ:   INDEX.md ? GLOBAL ? UI_COORDINATION ? UI_DEVELOPER_1_RENDERING
CODE:   src/ui/visualization/
WEEKS:  4-6
TASKS:  Parse DOT ? Render graphs ? Animate frames
OUTPUT: GraphWidget + AnimationController for framework dev
```

### ?? **I'm a UI Framework Developer (Main App)**
```
READ:   INDEX.md ? GLOBAL ? UI_COORDINATION ? UI_DEVELOPER_2_FRAMEWORK
CODE:   src/ui/
WEEKS:  4-6
TASKS:  Build main window ? Connect controls ? Integrate C2V/V2C
OUTPUT: Complete Qt application
```

### ?? **I'm a V2C Developer**
```
READ:   INDEX.md ? GLOBAL ? V2C_GUIDE ? V2C_MEMBER_*
CODE:   src/synthesis/
WEEKS:  5-7
TASKS:  Generate code ? Rewrite source files ? Maintain bidirectional sync
OUTPUT: Modified C++ source files
```

### ?? **I'm the Tech Lead**
```
READ:   GLOBAL_COORDINATION.md (complete) + team COORDINATION files
TRACK:  Timeline, dependencies, blockers
MEET:   Weekly syncs, bi-weekly architecture reviews
MANAGE: Cross-team coordination, escalations
```

---

## ?? FILE STRUCTURE

```
docs/
?? ?? INDEX.md      ? ENTRY POINT
?? ?? GLOBAL_COORDINATION.md            ? PROJECT LEVEL
?? ?? FILE_MANIFEST.md          ? THIS PACKAGE
?? ?? DOCUMENTATION_SUMMARY.md           ? OVERVIEW
?
?? ?? C2V TEAM
?  ?? C2V_COORDINATION.md
?  ?? C2V_DEVELOPER_1_FRONTEND.md       (Parser Dev)
?  ?? C2V_DEVELOPER_2_BACKEND.md  (Instrumentation Dev)
?
?? ?? UI TEAM
?  ?? UI_COORDINATION.md
?  ?? UI_DEVELOPER_1_RENDERING.md       (Rendering Dev)
?  ?? UI_DEVELOPER_2_FRAMEWORK.md       (Framework Dev)
?
?? ?? V2C TEAM (existing)
   ?? V2C_GUIDE.md
   ?? V2C_MEMBER_1_CODE_GENERATION.md
```

---

## ?? TIMELINE AT A GLANCE

```
Week ? C2V Dev 1   ? C2V Dev 2   ? UI Dev 1    ? UI Dev 2    ? V2C Team
????????????????????????????????????????????????????????????????????????
 1-3 ? FRONTEND    ?             ?     ?          ?
     ? ASTParser   ?   ?        ?             ?
????????????????????????????????????????????????????????????????????????
 2-4 ? (Done)    ? BACKEND     ?             ??
     ?  ? Instrument  ?  ?     ?
????????????????????????????????????????????????????????????????????????
 4-6 ? ? (Done)      ? RENDERING   ? FRAMEWORK   ?
     ?   ?             ? GraphWidget ? MainWindow  ?
????????????????????????????????????????????????????????????????????????
 5-7 ?             ? ? (Done)      ? (Done)      ? SYNTHESIS
 ?             ?  ? ?     ? CodeGen
????????????????????????????????????????????????????????????????????????
 6-8 ?    Support  ?    Support  ?  Support    ?  Support    ? (Done)
     ?  INTEGRATION & RELEASE       ?
????????????????????????????????????????????????????????????????????????
```

---

## ?? KEY DATA STRUCTURES

### DataStructureInfo (C2V-1 ? C2V-2, UI-2)
```cpp
struct DataStructureInfo {
    std::string className;   // "LinkedList"
    std::vector<std::string> memberVariables;  // {"head", "size"}
 std::vector<std::string> memberFunctions;  // {"insert", "remove"}
 bool isSelfReferential;        // true/false
    std::string pointerMember;   // "next" pointer
};
```

### ExecutionEvent (C2V-2 ? UI)
```cpp
struct ExecutionEvent {
    int lineNumber;         // 42
    std::string operation;           // "add_node"
    std::map<std::string, std::string> variables;  // {"id": "5"}
    std::map<std::string, std::string> addresses;  // pointers
    uint64_t timestamp;              // nanoseconds
};
```

### VisualizationState (C2V-2 ? UI-1/2)
```cpp
struct VisualizationState {
    int stepNumber;  // frame number
    std::string dotCode;             // Graphviz DOT
    std::string nodeData;        // JSON array
    std::string edgeData;    // JSON array
    std::string operationDescription;// "Added node 5"
};
```

---

## ? DELIVERABLES BY WEEK

### Week 1
- [x] C2V-1: ASTParser working
- [x] C2V-1: Can parse C++ files

### Week 2
- [x] C2V-1: StructureExtractor complete
- [x] C2V-2: ExecutionLogger + VisualizationState
- [x] UI-1: Start GraphvizRenderer

### Week 3
- [x] C2V-1: All tests passing, docs complete
- [x] C2V-2: StateMapper + Instrumentation working
- [x] UI-1: GraphWidget interactive

### Week 4
- [x] C2V: Pipeline complete and tested
- [x] UI-1: AnimationController working
- [x] UI-2: MainWindow structure built
- [x] V2C: Begin code generation

### Week 5
- [x] UI-1: Full rendering working (60 FPS)
- [x] UI-2: Control panels functional
- [x] V2C: Code generation engine working
- [x] All: Integration testing begins

### Week 6
- [x] UI: Application complete and integrated
- [x] UI + C2V: Load file ? Visualize working
- [x] V2C: Source file rewriting working

### Week 7
- [x] V2C: Bidirectional sync complete
- [x] All: System integration testing

### Week 8
- [x] All: Bug fixes, optimization, release

---

## ?? COMMON QUESTIONS

**"Where do I find my tasks?"**
? Your `DEVELOPER_X_*.md` file, look for "Task N.M"

**"What data do I receive?"**
? Your team's `COORDINATION.md`, see "Integration Points"

**"When do I handoff?"**
? Check `COORDINATION.md`, see "Handoff" sections

**"What tests do I write?"**
? Your `DEVELOPER_X_*.md`, see "Task X.Y: Unit Tests"

**"How do I connect to other teams?"**
? See relevant `COORDINATION.md`, see "Data Exchange"

**"What's my deadline?"**
? `GLOBAL_COORDINATION.md`, see "Phase Breakdown"

**"How do teams communicate?"**
? `GLOBAL_COORDINATION.md`, see "Communication"

**"What if I'm blocked?"**
? `GLOBAL_COORDINATION.md`, see "Dependency Escalation"

---

## ??? TECH STACK

| Component | Library | Version | Used By |
|-----------|---------|---------|---------|
| Parser | LLVM/Clang | 14+ | C2V-1, C2V-2 |
| Instrumentation | LLVM IR | 14+ | C2V-2 |
| Rendering | Graphviz | Latest | UI-1 |
| GUI | Qt | 6.0+ | UI-1, UI-2 |
| Testing | GoogleTest | Any | All |
| JSON | nlohmann_json | 3.11+ | C2V-2, UI-2 |
| Build | CMake | 3.21+ | All |

---

## ?? TESTING CHECKLIST

### Every Developer Must Verify
- [ ] Code compiles without warnings
- [ ] All unit tests pass
- [ ] No memory leaks
- [ ] Integration tests pass
- [ ] Performance meets targets
- [ ] Debugging tips tested
- [ ] Deliverables complete

### Team Lead Must Verify
- [ ] All developers have read guides
- [ ] Build system works
- [ ] Dependencies installed
- [ ] Testing framework set up
- [ ] CI/CD configured
- [ ] Communication channels active

---

## ?? PERFORMANCE TARGETS

| Component | Target | Threshold |
|-----------|--------|-----------|
| Parse Time | <1s | 1000-line file |
| Instrumentation | <5s | Compile |
| Execution Overhead | <10% | vs. native |
| State Mapping | <100ms | 10k events |
| Graph Rendering | 60 FPS | Smooth animation |
| UI Responsiveness | <100ms | User interaction |
| Code Generation | <100ms | Per operation |

---

## ?? BUILD COMMANDS

```bash
# Clone
git clone https://github.com/YassineBibrine/dataviz-uit.git
cd dataviz-uit

# Configure
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..

# Build
ninja

# Test
ctest --verbose

# Run
./dataviz  # Main application
./test_*   # Individual tests
```

---

## ?? DEPENDENCY CHAIN

```
clang_integration (C2V-1)
  ?
    ??? state_mapping (C2V-2) ??? visualization (UI-1)
    ??? llvm_instrumentation (C2V-2)
    ?
              [Execution Log]
       ?
              state_mapping ??? visualization (UI-1)
                   ?
        ui_engine (UI-2)
      ?
  Main Application
        ?
           ??? V2C (synthesis)
       ??? [Code Generation]
```

---

## ?? ESCALATION PATH

If blocked **>2 hours**:

1. Check debugging tips in your guide
2. Search `COORDINATION.md` for similar issue
3. Post in team Slack with:
   - Error message
   - Code snippet
   - Steps to reproduce
4. If still blocked >4 hours ? ping team lead
5. If still blocked >8 hours ? tech lead involved

---

## ?? FIRST DAY CHECKLIST

- [ ] Clone repository
- [ ] Read INDEX.md (15 min)
- [ ] Read GLOBAL_COORDINATION.md (30 min)
- [ ] Read your team's COORDINATION.md (20 min)
- [ ] Set up development environment
- [ ] Verify build works: `cmake && ninja`
- [ ] Run tests: `ctest`
- [ ] Join team Slack channel
- [ ] Schedule 1-on-1 with team lead
- [ ] Review your DEVELOPER_X guide (1 hour)

---

## ?? SUCCESS METRICS

? **You Succeed When**:
- Code compiles without warnings
- All tests pass (unit + integration)
- Performance meets targets
- Other teams receive deliverables on time
- Integration points work seamlessly
- No critical bugs
- Documentation is complete

? **Team Succeeds When**:
- Phase milestone delivered on schedule
- All developers contributing
- Zero major blockers
- Integration with other teams smooth

? **Project Succeeds When**:
- All 3 phases complete
- Full bidirectional pipeline working
- Demo with multiple test cases works
- Performance acceptable
- Ready for release

---

## ?? GO LIVE CHECKLIST

Before release (Week 8):

- [ ] All tests passing (unit + integration + system)
- [ ] No open critical/high bugs
- [ ] Performance benchmarks met
- [ ] Documentation complete
- [ ] Code reviewed and merged
- [ ] Build artifacts created
- [ ] User documentation ready
- [ ] Example projects included
- [ ] Version number bumped
- [ ] Release notes written

---

## ?? DOCUMENT VERSIONS

| Document | Version | Status | Updated |
|----------|---------|--------|---------|
| INDEX.md | 1.0 | ? Active | [Date] |
| GLOBAL_COORDINATION.md | 1.0 | ? Active | [Date] |
| C2V_COORDINATION.md | 1.0 | ? Active | [Date] |
| C2V_DEVELOPER_1_*.md | 1.0 | ? Active | [Date] |
| C2V_DEVELOPER_2_*.md | 1.0 | ? Active | [Date] |
| UI_COORDINATION.md | 1.0 | ? Active | [Date] |
| UI_DEVELOPER_1_*.md | 1.0 | ? Active | [Date] |
| UI_DEVELOPER_2_*.md | 1.0 | ? Active | [Date] |

---

## ?? YOU'RE READY!

You have:
- ? Clear goals
- ? Clear timeline
- ? Clear tasks
- ? Clear examples
- ? Clear testing
- ? Clear integration
- ? Clear support

**Now build amazing things!** ??

---

**Quick Reference Card v1.0**  
**DataViz Project Documentation**  
**Print and Keep Handy!**

---

## ?? BOOKMARK THESE

1. **START**: https://github.com/YassineBibrine/dataviz-uit/blob/main/docs/INDEX.md
2. **GLOBAL**: https://github.com/YassineBibrine/dataviz-uit/blob/main/docs/GLOBAL_COORDINATION.md
3. **YOUR GUIDE**: docs/[YOUR_ROLE]_*.md
4. **TEAM REPO**: https://github.com/YassineBibrine/dataviz-uit

---

**Questions? ? Your team lead**  
**Stuck? ? Check your DEVELOPER guide**  
**Need context? ? Read GLOBAL_COORDINATION.md**  

**Happy coding! ??**
