# ?? C2V Dev 1 - Frontend/AST Parsing Workspace

Welcome to your isolated development workspace!

---

## ?? YOUR ROLE

**C2V Dev 1 - Frontend/AST Parsing**
- Weeks 1-3
- Tasks: 1.1 ? 1.2 ? 1.3 ? 1.4
- Team: C2V (with Dev 2)

---

## ? 5-MINUTE QUICK START

### Step 1: Understand Your Tasks
```
Task 1.1: ASTParser Header (Week 1, Days 1-2)
Task 1.2: ASTParser Implementation (Week 1, Days 2-4)
Task 1.3: ASTParser Unit Tests (Week 1, Days 4-5)
Task 1.4: StructureExtractor (Week 2-3)
```

### Step 2: Know Your Deliverables
- `ast_parser.h` - Header with AST parsing interface
- `ast_parser.cpp` - Implementation using LLVM
- `ast_parser_tests.cpp` - Comprehensive unit tests
- `structure_extractor.h/.cpp` - Data structure analysis

### Step 3: Know Your Dependencies
- **Input from**: C++ source code files
- **Output to**: C2V Dev 2 (DataStructureInfo structure)
- **Blocks**: UI Team (needs your DataStructureInfo), V2C Team
- **External**: LLVM/Clang libraries

### Step 4: Read These Files (In Order)
1. `./README.md` (this file)
2. `TASKS.md`
3. `GETTING_STARTED.md`
4. `docs/DEVELOPER_GUIDE.md`

### Step 5: Start Coding
```bash
# 1. Set up environment
source GETTING_STARTED.md

# 2. Create first file
touch src/ast_parser/ast_parser.h

# 3. Begin Task 1.1
# See TASKS.md for details
```

---

## ?? YOUR TIMELINE

### Week 1: Foundation (5 days)
```
Mon-Tue:  Task 1.1a - AST Parser Header design
        ?? Create header with all function signatures
          ?? Define data structures
      ?? Document interfaces

Wed-Thu:  Task 1.1b - AST Parser Implementation
   ?? Implement header in CPP file
          ?? Integrate LLVM/Clang
   ?? Complete all functions

Fri:      Task 1.1c - Unit Tests
          ?? Write comprehensive tests (gtest)
        ?? Verify all functionality
   ?? Fix any bugs
          ?? Ready for handoff
```

### Week 2: Expansion (5 days)
```
Mon-Tue:  Task 1.2a - StructureExtractor Design
          ?? Header file and interfaces
          ?? Use AST from Task 1.1

Wed-Thu:  Task 1.2b - StructureExtractor Implementation
          ?? Extract data structures from AST
       ?? Build DataStructureInfo objects
     ?? Complete all required analysis

Fri:  Task 1.2c - Tests & Documentation
          ?? Unit tests for extractor
        ?? API documentation
  ?? Ready for Dev 2
```

### Week 3: Finalization (3 days)
```
Mon-Wed:  Task 1.3 - CMake Integration & Final Tests
    ?? Integrate with build system
          ?? Run full test suite
          ?? Performance benchmarking
          ?? Handoff to Dev 2

Thu-Fri:  Support Phase
          ?? Dev 2 may need clarifications
          ?? Fix any integration issues
          ?? Prepare for UI handoff
```

---

## ?? YOUR WORKSPACE STRUCTURE

```
workspace/C2V_TEAM/DEV_1_FRONTEND/
??? README.md   ? You are here
??? TASKS.md  ? Detailed task list
??? SCHEDULE.md  ? Week-by-week details
??? GETTING_STARTED.md         ? Setup instructions
??? docs/
?   ??? DEVELOPER_GUIDE.md    ? Full guide from shared docs
?   ??? MY_TASKS_DETAILED.md      ? Your task breakdown
?   ??? DEPENDENCIES.md       ? Who needs your output
?   ??? INTERFACES.md    ? APIs you must create
?   ??? LLVM_GUIDE.md         ? LLVM integration tips
?   ??? AST_PARSING.md       ? AST parsing concepts
?   ??? CODE_EXAMPLES.md   ? Reference implementations
?   ??? TESTING.md ? Unit test examples
??? src/
?   ??? CMakeLists.txt          ? Local build config
?   ??? include/
?   ?   ??? ast_parser.h     ? Your header
?   ??? ast_parser/
?       ??? ast_parser.h               ? Parser header
?       ??? ast_parser.cpp       ? Implementation
?    ??? ast_parser_tests.cpp ? Tests
?       ??? structure_extractor.h   ? Extractor header
?       ??? structure_extractor.cpp    ? Implementation
?  ??? structure_extractor_tests.cpp
??? BLOCKERS.md    ? Track any issues
??? build/       ? Build output (.gitignored)
```

---

## ?? YOUR KEY FILES TO CREATE

### File 1: `src/ast_parser/ast_parser.h`
```cpp
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <clang/AST/AST.h>

// Your AST parser interface
class ASTParser {
public:
    ASTParser();
    virtual ~ASTParser();
    
    // Parse C++ source file
    std::unique_ptr<clang::TranslationUnit> parseFile(
const std::string& filename
    );
    
    // Extract all functions
    std::vector<std::string> extractFunctionNames(
     clang::TranslationUnit* tu
    );
    
    // Extract all data structures
    std::vector<std::string> extractDataStructures(
        clang::TranslationUnit* tu
    );
};
```

### File 2: `src/ast_parser/ast_parser.cpp`
```cpp
#include "ast_parser.h"
// ... implementation using LLVM/Clang ...
```

### File 3: `src/ast_parser/ast_parser_tests.cpp`
```cpp
#include <gtest/gtest.h>
#include "ast_parser.h"

TEST(ASTParserTest, ParseSimpleFile) {
    // Test implementation
}

TEST(ASTParserTest, ExtractFunctions) {
    // Test implementation
}

// ... more tests ...
```

---

## ?? QUICK BUILD COMMANDS

```bash
# 1. Enter your workspace
cd workspace/C2V_TEAM/DEV_1_FRONTEND

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
git commit -m "Task 1.1: AST Parser Header and Implementation"
git push origin c2v-dev-1-frontend
```

---

## ?? YOUR DEPENDENCIES

### You Need Input From:
- **LLVM/Clang libraries** (pre-installed)
- **C++ standard library**
- **gtest framework** (for testing)

### You Provide to:
- **C2V Dev 2**: `DataStructureInfo` structure
- **UI Team**: AST analysis results
- **V2C Team**: Source code structure information

### Your Handoff Timeline
```
End of Week 1:    Task 1.1 complete ? Dev 2 can start
End of Week 2:    Task 1.2 complete ? UI team can start prep
End of Week 3:    Task 1.3 complete ? Full C2V handoff to UI/V2C
```

---

## ?? YOUR TESTING STRATEGY

### Unit Tests (Your Responsibility)
```
? AST Parser Creation
? File Parsing (with sample files)
? Function Extraction
? Data Structure Extraction
? Error Handling
? Edge Cases
```

### Integration Tests (Dev 2's Responsibility)
```
Dev 2 will test:
- Your output ? Their input compatibility
- Full C2V pipeline with your code
```

---

## ?? IF YOU GET STUCK

### Check These In Order:
1. **Your TASKS.md** - See exact requirements
2. **docs/CODE_EXAMPLES.md** - See example implementations
3. **docs/LLVM_GUIDE.md** - See LLVM integration tips
4. **docs/TESTING.md** - See test examples
5. **BLOCKERS.md** - Record the blocker
6. **Slack message** to teammate
7. **Escalate to Tech Lead** if 4+ hours blocked

### Track Blockers Here:
```markdown
# BLOCKERS.md

## Blocker: [Title]
- **When**: [Date/Time blocked]
- **Why**: [What's the issue]
- **Depends on**: [Who has the answer]
- **Expected**: [When should resolve]
- **Impact**: [What it blocks]
- **Action**: [What I'm doing]
```

---

## ? YOUR SUCCESS CRITERIA

### Week 1 Success:
- [ ] AST parser header complete
- [ ] AST parser implementation works
- [ ] All unit tests passing
- [ ] Code compiles without warnings
- [ ] Ready for Dev 2

### Week 2 Success:
- [ ] StructureExtractor header complete
- [ ] StructureExtractor implementation works
- [ ] All unit tests passing
- [ ] DataStructureInfo documented
- [ ] Ready for UI/V2C teams

### Week 3 Success:
- [ ] Full C2V pipeline integration
- [ ] Performance targets met
- [ ] All bugs fixed
- [ ] Documentation complete
- [ ] Handoff verified

---

## ?? WORKING WITH DEV 2

### Daily Communication:
- **10 AM**: 15-min standup (you + Dev 2)
- **Throughout day**: Slack for quick questions
- **Friday**: Review your work together

### Handoff Process (End of Week 1):
1. Send Dev 2 mock `DataStructureInfo` structure
2. Dev 2 starts with mocked version
3. End of Week 2: Real version ready
4. Integration happens seamlessly

---

## ?? CONTACT & ESCALATION

### Your Team: C2V Team
- **Dev 1**: You (AST Parsing)
- **Dev 2**: [Name] (Instrumentation)
- **Team Lead**: [Name]
- **Tech Lead**: [Name]

### Daily Standup: 10 AM (you + Dev 2)
### Weekly Sync: Friday 2 PM (all teams)

### Need Help?
1. Check docs in your workspace
2. Ask Dev 2 on Slack
3. Post in #c2v-team channel
4. Escalate to Tech Lead if urgent

---

## ?? LEARNING RESOURCES

### In Your docs/:
- `LLVM_GUIDE.md` - LLVM/Clang tutorial
- `AST_PARSING.md` - AST concepts
- `CODE_EXAMPLES.md` - Real implementations
- `TESTING.md` - Unit test patterns

### External:
- [LLVM Documentation](https://llvm.org/docs/)
- [Clang API Docs](https://clang.llvm.org/docs/)
- [gtest Tutorial](https://github.com/google/googletest)

---

## ?? READY TO START?

### Your Action Plan:

1. **Right now**: Read these files in order:
   - TASKS.md (5 min)
   - GETTING_STARTED.md (10 min)
   - docs/DEVELOPER_GUIDE.md (30 min)

2. **Next (Tomorrow morning)**:
   - Set up your environment
   - Create your first file: `ast_parser.h`
   - Start Task 1.1

3. **Daily**:
- 10 AM: Standup with Dev 2
   - Work on your tasks
   - End of day: Commit your progress

4. **Friday**:
   - Review your work
 - Prepare for next week
   - Attend weekly sync

---

## ?? YOUR METRICS

### Track These Weekly:
- ? Tests passing: [#]
- ?? Hours spent: [hours]
- ?? Bugs fixed: [#]
- ?? Documentation: [% complete]
- ?? Blockers: [#]

### Target for Week 1:
- 100% of Task 1.1 complete
- 80+ test coverage
- 0 critical bugs
- Ready for Dev 2 input

---

## ?? PRO TIPS

1. **Commit often** - Multiple small commits daily
2. **Test early** - Write tests as you code
3. **Document as you go** - Don't leave it for end
4. **Ask questions** - Don't get stuck for hours
5. **Keep BLOCKERS.md updated** - Track everything
6. **Review with Dev 2** - Get feedback early

---

## ? YOU'RE READY!

Everything is set up. Your workspace is isolated. You have all the guidance you need.

**Next action**: Open `TASKS.md` and start with Task 1.1.

**Timeline**: 3 weeks to complete the C2V frontend.

**Support**: Full - you have a team.

**Let's build!** ??

---

**Status**: ? Your workspace is ready
**Next**: Read TASKS.md
**Questions**: See GETTING_STARTED.md

