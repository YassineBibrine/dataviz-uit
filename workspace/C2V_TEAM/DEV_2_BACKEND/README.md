# ?? C2V Dev 2 - Backend/Instrumentation Workspace

Welcome to your isolated development workspace!

---

## ?? YOUR ROLE

**C2V Dev 2 - Backend/Instrumentation**
- Weeks 2-4
- Tasks: 2.1 ? 2.2 ? 2.3 ? 2.4 ? 2.5 ? 2.6 ? 2.7
- Team: C2V (with Dev 1)
- Starts: After Dev 1 provides AST Parser (Week 2)

---

## ? 5-MINUTE QUICK START

### Step 1: Understand Your Tasks
```
Task 2.1: ExecutionEvent Model (Week 2, Day 2)
Task 2.2: ExecutionLogger Implementation (Week 2)
Task 2.3: VisualizationState Model (Week 2-3)
Task 2.4: StateMapper Implementation (Week 3)
Task 2.5: LLVM Instrumentation (Week 3-4)
Task 2.6: Integration Tests (Week 4)
Task 2.7: C2V Finalization (Week 4)
```

### Step 2: Know Your Deliverables
- `execution_logger.h` - Execution logging interface
- `execution_logger.cpp` - Implementation
- `visualization_state.h` - State management interface
- `state_mapper.h/.cpp` - Data structure mapping
- `llvm_instrumentation.h/.cpp` - LLVM instrumentation
- Complete test suite

### Step 3: Know Your Dependencies
- **Input from**: C2V Dev 1 (ASTParser, DataStructureInfo)
- **Output to**: UI Team (VisualizationState), V2C Team (state data)
- **Blocks**: UI and V2C teams
- **External**: LLVM, clang libraries

### Step 4: Read These Files (In Order)
1. `./README.md` (this file)
2. `TASKS.md`
3. `GETTING_STARTED.md`
4. `docs/DEVELOPER_GUIDE.md`

### Step 5: Prepare to Start
```bash
# Week 1: Wait for Dev 1 to provide mock DataStructureInfo
# Watch their progress in shared BLOCKERS.md

# Week 2: Dev 1 handoff + you start
# Begin with mock, swap to real later
```

---

## ?? YOUR TIMELINE

### Week 1: Preparation (No coding yet)
```
Mon-Fri:  Dev 1 works on ASTParser
          ?? You review their code
          ?? Understand DataStructureInfo interface
   ?? Prepare your mock version
          ?? Write design doc for Tasks 2.1-2.3
```

### Week 2: Start with Mocks (5 days)
```
Mon:Dev 1 provides mock DataStructureInfo
          ?? You start Task 2.1 with mock API

Tue-Wed:  Task 2.1 - ExecutionEvent Model
   ?? Design event data structures
     ?? Create ExecutionEvent class

Thu-Fri:  Task 2.2 - ExecutionLogger
          ?? Implement logger using mock data
     ?? Unit tests
```

### Week 3: Full Implementation (5 days)
```
Mon-Tue:  Dev 1 provides real DataStructureInfo
       ?? Swap mock for real (should be seamless!)

Wed:      Task 2.3 - VisualizationState
      ?? State management class
          ?? Data binding

Thu-Fri:  Task 2.4 - StateMapper
          ?? Map AST ? VisualizationState
   ?? Complete unit tests
          ?? Ready for UI team
```

### Week 4: Finalization (5 days)
```
Mon-Tue:  Task 2.5 - LLVM Instrumentation
          ?? Instrument source code
          ?? Collect execution data

Wed-Thu:  Task 2.6 - Integration Tests
          ?? Full C2V pipeline tests
          ?? Performance benchmarking

Fri:  Task 2.7 - Finalization
          ?? Bug fixes
   ?? Complete handoff to UI/V2C
   ?? Ready for integration phase
```

---

## ?? YOUR WORKSPACE STRUCTURE

```
workspace/C2V_TEAM/DEV_2_BACKEND/
??? README.md   ? You are here
??? TASKS.md  ? Your detailed tasks
??? SCHEDULE.md  ? Week-by-week breakdown
??? GETTING_STARTED.md         ? Setup instructions
??? docs/
?   ??? DEVELOPER_GUIDE.md    ? Full guide
?   ??? MY_TASKS_DETAILED.md      ? Task breakdown
?   ??? DEPENDENCIES.md       ? Dev 1 outputs
?   ??? INTERFACES.md    ? APIs you need/create
?   ??? LLVM_INSTRUMENTATION.md    ? Instrumentation guide
?   ??? STATE_MANAGEMENT.md  ? State concepts
?   ??? CODE_EXAMPLES.md   ? Reference
?   ??? TESTING.md ? Unit test patterns
??? src/
?   ??? CMakeLists.txt          ? Local build config
?   ??? include/
?   ?   ??? *.h  ? Your headers
?   ??? execution_logger/
?   ?   ??? execution_logger.h
?   ?   ??? execution_logger.cpp
?   ?   ??? execution_logger_tests.cpp
?   ??? instrumentation/
?   ?   ??? llvm_instrumentation.h
?   ?   ??? llvm_instrumentation.cpp
?   ? ??? instrumentation_tests.cpp
?   ??? visualization_state/
?       ??? visualization_state.h
?       ??? visualization_state.cpp
?       ??? state_tests.cpp
??? BLOCKERS.md    ? Track any issues
??? MOCK_DATA.md  ? Your mock data for Week 2
??? build/   ? Build output
```

---

## ?? YOUR KEY FILES TO CREATE

### File 1: `src/execution_logger/execution_logger.h`
```cpp
#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <chrono>

struct ExecutionEvent {
    uint64_t timestamp;
    std::string eventType;
    std::string data;
    // ... other fields
};

class ExecutionLogger {
public:
ExecutionLogger();
    ~ExecutionLogger();
    
    void recordEvent(const ExecutionEvent& event);
    const std::vector<ExecutionEvent>& getEvents() const;
    void clearEvents();
};
```

### File 2: `src/visualization_state/visualization_state.h`
```cpp
#pragma once
#include <string>
#include <map>

class VisualizationState {
public:
    VisualizationState();
    ~VisualizationState();
    
    void updateStructureData(const std::string& key, 
          const std::string& value);
    std::string getDotRepresentation() const;
    std::map<std::string, std::string> getStateData() const;
};
```

### File 3: `src/instrumentation/llvm_instrumentation.h`
```cpp
#pragma once
#include <string>

class LLVMInstrumenter {
public:
    LLVMInstrumenter();
    ~LLVMInstrumenter();
    
    bool instrumentSource(const std::string& inputFile,
         const std::string& outputFile);
    std::vector<std::string> getInstrumentationReport() const;
};
```

---

## ?? QUICK BUILD COMMANDS

```bash
# 1. Enter your workspace
cd workspace/C2V_TEAM/DEV_2_BACKEND

# 2. Create build directory
mkdir -p build && cd build

# 3. Configure CMake
cmake .. -G Ninja

# 4. Build your code
ninja

# 5. Run your tests
ctest

# 6. Commit your work
cd ..
git add -A
git commit -m "Task 2.1-2.2: ExecutionLogger and VisualizationState"
git push origin c2v-dev-2-backend
```

---

## ?? YOUR DEPENDENCIES

### You Need Input From:
- **Dev 1**: `DataStructureInfo` structure (mock by Tuesday Week 2, real by Tuesday Week 3)
- **LLVM/Clang libraries**
- **gtest framework**

### You Provide to:
- **UI Team**: `VisualizationState` class
- **V2C Team**: Execution state information

### Timeline:
```
End of Week 2:    Task 2.1-2.2 complete (with mocks)
End of Week 3:    Task 2.3-2.4 complete (with real data)
End of Week 4:    Task 2.5-2.7 complete (full C2V ready)
```

---

## ?? YOUR TESTING STRATEGY

### Unit Tests (Your Responsibility)
```
? Event Creation
? Event Logging
? State Management
? Data Mapping
? Error Handling
? Edge Cases
```

### Integration Tests
```
Dev 1 + Dev 2 combined:
- Full C2V pipeline
- Input source ? Output visualization state
```

---

## ?? WORKING WITH DEV 1

### Week 1: Preparation
- Review Dev 1's AST Parser design
- Understand `DataStructureInfo` interface
- Prepare your mock for Week 2 start

### Week 2-3: Collaboration
- Use mock data (provided by Dev 1)
- Swap to real data mid-week
- Daily communication: questions about their API
- Friday: Review together

### Handoff Process:
1. Dev 1 finalizes AST Parser (Friday Week 1)
2. You get mock `DataStructureInfo` (Monday Week 2)
3. You build with mock all of Week 2
4. Dev 1 finishes real version (Tuesday Week 3)
5. You integrate real version seamlessly
6. Continue with Week 3 tasks

---

## ?? IF YOU GET STUCK

### Check These In Order:
1. **Your TASKS.md** - See exact requirements
2. **docs/CODE_EXAMPLES.md** - See example implementations
3. **docs/INSTRUMENTATION.md** - See LLVM tips
4. **BLOCKERS.md** - Record the blocker
5. **Slack message** to Dev 1
6. **Escalate to Tech Lead** if 4+ hours blocked

### Common Issues & Solutions:

**Issue**: Mock data doesn't match real data
- **Solution**: Keep mock interface loose, swap implementation easily
- **Prevention**: Communicate API changes with Dev 1 daily

**Issue**: LLVM instrumentation crashes
- **Solution**: Start with simple test case, build up complexity
- **Prevention**: Review LLVM_INSTRUMENTATION.md guide

**Issue**: Tests fail after swapping mock ? real data
- **Solution**: Likely means API changed, coordinate with Dev 1
- **Prevention**: Detailed BLOCKERS.md + daily communication

---

## ? YOUR SUCCESS CRITERIA

### Week 2 Success:
- [ ] ExecutionEvent model complete
- [ ] ExecutionLogger working with mock data
- [ ] All unit tests passing
- [ ] Mock/real data interface documented
- [ ] Ready for real data swap

### Week 3 Success:
- [ ] Real data integration seamless
- [ ] VisualizationState complete
- [ ] StateMapper working
- [ ] All tests passing
- [ ] Ready for UI team

### Week 4 Success:
- [ ] LLVM instrumentation working
- [ ] Full C2V pipeline complete
- [ ] Performance targets met
- [ ] All bugs fixed
- [ ] Handoff to UI/V2C verified

---

## ?? CONTACT & ESCALATION

### Your Team: C2V Team
- **Dev 1**: [Name] (AST Parsing)
- **Dev 2**: You (Instrumentation)
- **Team Lead**: [Name]
- **Tech Lead**: [Name]

### Daily Standup: 10 AM (you + Dev 1)
### Weekly Sync: Friday 2 PM (all teams)

### Contact Protocol:
1. Check docs in your workspace
2. Ask Dev 1 on Slack
3. Escalate to Team Lead if urgent

---

## ?? PRO TIPS

1. **Start early with mocks** - Don't wait for Dev 1
2. **Build swap-friendly code** - Easy to replace mock with real
3. **Test with both mock and real** - Catch integration issues early
4. **Document your interfaces** - So UI/V2C team can use your code
5. **Keep BLOCKERS.md updated** - Let Tech Lead help you unblock
6. **Review code with Dev 1** - Get feedback on architecture

---

## ? YOU'RE READY!

You start in Week 2, so use Week 1 to prepare. Your workspace is isolated and ready.

**Week 1 Action**: Review Dev 1's code, prepare mock data
**Week 2 Action**: Start Tasks 2.1-2.2 with mock
**Week 3 Action**: Integrate real data, continue tasks
**Week 4 Action**: Finalize and handoff

**Let's build!** ??

---

**Status**: ? Your workspace is ready (waiting for Dev 1)
**Start**: Week 2 (after Dev 1 Week 1 complete)
**Questions**: See GETTING_STARTED.md

