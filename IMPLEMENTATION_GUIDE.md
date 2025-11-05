# ?? IMPLEMENTATION GUIDE - Setting Up Isolated Workspaces
**Step-by-Step Instructions to Implement the New Structure**

---

## ?? PRE-IMPLEMENTATION CHECKLIST

Before you start, make sure you have:

- [ ] Access to repository
- [ ] Git configured locally
- [ ] CMake 3.21+ installed
- [ ] Ninja build system installed
- [ ] Team members ready
- [ ] 2-4 hours available

---

## ?? STEP-BY-STEP IMPLEMENTATION

### Step 1: Create Workspace Directory Structure

```bash
# Navigate to repo root
cd dataviz-uit

# Create main workspace directory
mkdir -p workspace/{C2V_TEAM,UI_TEAM,V2C_TEAM,TECH_LEAD}

# Create team member subdirectories
mkdir -p workspace/C2V_TEAM/{DEV_1_FRONTEND,DEV_2_BACKEND}
mkdir -p workspace/UI_TEAM/{DEV_1_RENDERING,DEV_2_FRAMEWORK}
mkdir -p workspace/V2C_TEAM/{DEV_1_CODEGEN,DEV_2_REWRITING}

# Create subdirectories in each workspace
for team in C2V_TEAM UI_TEAM V2C_TEAM TECH_LEAD; do
  for role in workspace/$team/*; do
    if [ -d "$role" ]; then
   mkdir -p "$role/{docs,src,build}"
 fi
  done
done
```

### Step 2: Copy Workspace README Files

```bash
# Copy the README files created earlier
cp workspace/C2V_TEAM/DEV_1_FRONTEND/README.md \
   workspace/C2V_TEAM/DEV_1_FRONTEND/README.md

cp workspace/C2V_TEAM/DEV_2_BACKEND/README.md \
   workspace/C2V_TEAM/DEV_2_BACKEND/README.md

# ... repeat for all other teams ...

cp workspace/TECH_LEAD/README.md \
   workspace/TECH_LEAD/README.md
```

### Step 3: Create Template Files in Each Workspace

```bash
# For each workspace, create these files:

# TASKS.md - Task breakdown
cat > workspace/C2V_TEAM/DEV_1_FRONTEND/TASKS.md << 'EOF'
# C2V Dev 1 - Frontend/AST Parsing Tasks

## Task 1.1: AST Parser Header
- **Duration**: 2 days (Mon-Tue Week 1)
- **Files**: src/ast_parser/ast_parser.h
- **Dependencies**: LLVM/Clang libraries
- **Testing**: Unit tests in ast_parser_tests.cpp

## Task 1.2: AST Parser Implementation
- **Duration**: 3 days (Wed-Thu-Fri Week 1)
- **Files**: src/ast_parser/ast_parser.cpp
- **Dependencies**: Task 1.1 complete
- **Testing**: Complete unit tests

## Task 1.3: StructureExtractor
- **Duration**: 4-5 days (Mon-Fri Week 2-3)
- **Files**: src/structure_extractor/*
- **Dependencies**: Task 1.1-1.2 complete
- **Testing**: Complete unit tests

## Task 1.4: CMake Integration
- **Duration**: 2 days (Mon-Tue Week 3)
- **Files**: src/CMakeLists.txt
- **Dependencies**: All previous tasks
- **Testing**: Full build & test suite
EOF

# SCHEDULE.md - Week-by-week breakdown
cat > workspace/C2V_TEAM/DEV_1_FRONTEND/SCHEDULE.md << 'EOF'
# C2V Dev 1 - Development Schedule

## Week 1: AST Parser Foundation
### Monday-Tuesday
- Task 1.1a: Design AST Parser header
- Files: ast_parser.h
- Time: 16 hours

### Wednesday-Thursday  
- Task 1.1b: Implement parser
- Files: ast_parser.cpp
- Time: 20 hours

### Friday
- Task 1.1c: Unit tests & debugging
- Files: ast_parser_tests.cpp
- Time: 8 hours

## Week 2-3: Data Structure Analysis
### Weeks 2-3: Task 1.2
- StructureExtractor implementation
- Unit tests
- Ready for handoff

EOF

# GETTING_STARTED.md - Setup instructions
cat > workspace/C2V_TEAM/DEV_1_FRONTEND/GETTING_STARTED.md << 'EOF'
# Getting Started - C2V Dev 1

## Prerequisites
- C++ 17 or later
- LLVM/Clang libraries
- CMake 3.21+
- Ninja build system
- gtest framework

## Installation

### 1. Install LLVM/Clang
```bash
# On Ubuntu/Debian
sudo apt-get install llvm-dev clang-dev

# On macOS
brew install llvm clang
```

### 2. Install build tools
```bash
sudo apt-get install cmake ninja-build
```

### 3. Install gtest
```bash
sudo apt-get install libgtest-dev
```

## Building Your Code

```bash
cd workspace/C2V_TEAM/DEV_1_FRONTEND
mkdir -p build
cd build
cmake .. -G Ninja
ninja
ctest
```

## First File

Create `src/ast_parser/ast_parser.h`:
```cpp
#pragma once
#include <string>
#include <memory>
#include <clang/AST/AST.h>

class ASTParser {
public:
    ASTParser();
    ~ASTParser();
    std::unique_ptr<clang::TranslationUnit> parseFile(
const std::string& filename);
};
```

## Troubleshooting

If CMake can't find LLVM:
```bash
cmake .. -G Ninja -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm
```

EOF

# BLOCKERS.md - Track issues
cat > workspace/C2V_TEAM/DEV_1_FRONTEND/BLOCKERS.md << 'EOF'
# Current Blockers

## None yet!

As you encounter blockers, record them here:

## Blocker Template:
```
## Blocker: [Title]
- **When**: [Date/Time]
- **Why**: [What's the issue]
- **Depends on**: [Who has the answer]
- **Expected**: [When should resolve]
- **Impact**: [What it blocks]
- **Action**: [What I'm doing]
- **Status**: [Blocked/In Progress/Resolved]
```
EOF
```

### Step 4: Update Main CMakeLists.txt

```cmake
# In dataviz-uit/CMakeLists.txt, add at root level:

cmake_minimum_required(VERSION 3.21)
project(DataViz)

# Enable testing
enable_testing()

# Include all team workspaces
add_subdirectory(workspace/C2V_TEAM/DEV_1_FRONTEND)
add_subdirectory(workspace/C2V_TEAM/DEV_2_BACKEND)
add_subdirectory(workspace/UI_TEAM/DEV_1_RENDERING)
add_subdirectory(workspace/UI_TEAM/DEV_2_FRAMEWORK)
add_subdirectory(workspace/V2C_TEAM/DEV_1_CODEGEN)
add_subdirectory(workspace/V2C_TEAM/DEV_2_REWRITING)

# Create a master test target
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
)
```

### Step 5: Create CMakeLists.txt Templates

```bash
# Create a template CMakeLists.txt for each workspace

cat > workspace/C2V_TEAM/DEV_1_FRONTEND/src/CMakeLists.txt << 'EOF'
# C2V Dev 1 - Frontend/AST Parsing

# Find required packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
find_package(GTest REQUIRED)

# Include directories
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

# Source files
set(SOURCES
    ast_parser/ast_parser.cpp
    structure_extractor/structure_extractor.cpp
)

# Header files
set(HEADERS
    ast_parser/ast_parser.h
    structure_extractor/structure_extractor.h
)

# Create library
add_library(c2v_frontend ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(c2v_frontend
    ${LLVM_LIBRARIES}
    ${CLANG_LIBRARIES}
)

# Tests
add_executable(c2v_frontend_tests
    ast_parser/ast_parser_tests.cpp
    structure_extractor/structure_extractor_tests.cpp
)

target_link_libraries(c2v_frontend_tests
    c2v_frontend
    GTest::GTest
 GTest::Main
)

enable_testing()
add_test(NAME C2V_Frontend_Tests COMMAND c2v_frontend_tests)
EOF

# Repeat for other teams with appropriate changes
```

### Step 6: Set Up Git Branches

```bash
# Create individual development branches
git checkout -b c2v-dev-1-frontend
git checkout -b c2v-dev-2-backend
git checkout -b ui-dev-1-rendering
git checkout -b ui-dev-2-framework
git checkout -b v2c-dev-1-codegen
git checkout -b v2c-dev-2-rewriting
git checkout -b tech-lead-integration

# Create team aggregate branches
git checkout -b c2v-dev
git checkout -b ui-dev
git checkout -b v2c-dev

# Return to main
git checkout main
```

### Step 7: Share Documentation

```bash
# Copy main documentation files to docs/SHARED/
mkdir -p docs/SHARED
cp WORKSPACE_ORGANIZATION.md docs/SHARED/
cp README_WORKSPACES.md docs/SHARED/

# Copy planning docs
mkdir -p docs/PLANNING
cp docs/PARALLEL_TASK_PLANNING.md docs/PLANNING/
cp docs/GANTT_AND_SCHEDULES.md docs/PLANNING/
cp docs/TASK_DEPENDENCY_MATRIX.md docs/PLANNING/
```

### Step 8: Initial Commit

```bash
# Stage everything
git add workspace/ README_WORKSPACES.md WORKSPACE_ORGANIZATION.md

# Commit
git commit -m "feat: Implement isolated team workspaces for parallel development

- Create workspace for each team member (6 total)
- Each workspace has own docs, src, build directories
- Add README.md, TASKS.md, SCHEDULE.md to each
- Update main CMakeLists.txt to include all workspaces
- Set up Git branches for individual and team development
- Enable maximum parallelization (4 independent tracks weeks 4-5)"

# Push to main
git push origin main

# Create and push team branches
git push origin c2v-dev c2v-dev-1-frontend c2v-dev-2-backend
git push origin ui-dev ui-dev-1-rendering ui-dev-2-framework
git push origin v2c-dev v2c-dev-1-codegen v2c-dev-2-rewriting
git push origin tech-lead-integration
```

---

## ? VERIFICATION CHECKLIST

After implementation, verify everything works:

### Directory Structure
- [ ] workspace/C2V_TEAM/DEV_1_FRONTEND exists
- [ ] workspace/C2V_TEAM/DEV_2_BACKEND exists
- [ ] workspace/UI_TEAM/DEV_1_RENDERING exists
- [ ] workspace/UI_TEAM/DEV_2_FRAMEWORK exists
- [ ] workspace/V2C_TEAM/DEV_1_CODEGEN exists
- [ ] workspace/V2C_TEAM/DEV_2_REWRITING exists
- [ ] workspace/TECH_LEAD exists

### Files in Each Workspace
- [ ] README.md exists
- [ ] TASKS.md exists
- [ ] SCHEDULE.md exists
- [ ] GETTING_STARTED.md exists
- [ ] BLOCKERS.md exists
- [ ] docs/ directory created
- [ ] src/ directory created
- [ ] src/CMakeLists.txt exists

### Git Branches
- [ ] c2v-dev branch exists
- [ ] c2v-dev-1-frontend branch exists
- [ ] c2v-dev-2-backend branch exists
- [ ] ui-dev branch exists
- [ ] ui-dev-1-rendering branch exists
- [ ] ui-dev-2-framework branch exists
- [ ] v2c-dev branch exists
- [ ] v2c-dev-1-codegen branch exists
- [ ] v2c-dev-2-rewriting branch exists
- [ ] tech-lead-integration branch exists

### CMake Build
```bash
cd dataviz-uit
mkdir build && cd build
cmake .. -G Ninja
ninja

# Should complete without errors
```

---

## ?? NEXT STEPS AFTER IMPLEMENTATION

### Immediately After (Day 1):
```bash
# 1. Share workspace links with team
# 2. Each person clones their workspace branch
# 3. Each person reads their README.md
# 4. Schedule first standup
```

### Monday Week 1 (First Standup):
```bash
# 10:00 AM - C2V Team
# 10:20 AM - UI Team
# 10:40 AM - V2C Team
# 2:00 PM - Friday All-Hands (planning)
```

### Week 1 Execution:
```bash
# C2V Dev 1 starts Task 1.1
# Others setup environments and prepare
# Daily standups track progress
```

---

## ?? TROUBLESHOOTING IMPLEMENTATION

### Problem: CMake can't find workspace subdirectories
**Solution**: Make sure CMakeLists.txt files exist in each workspace src/ directory

### Problem: Git branches not created
**Solution**: 
```bash
git branch c2v-dev-1-frontend
git push -u origin c2v-dev-1-frontend
```

### Problem: Build fails in workspace
**Solution**: Verify CMakeLists.txt is correct and dependencies are installed

### Problem: Tests don't compile
**Solution**: Make sure gtest is installed and linked in CMakeLists.txt

---

## ? SUCCESS INDICATORS

After implementation is complete, you'll have:

? 7 isolated workspace directories (one per team member + tech lead)
? Each has complete documentation (README, TASKS, SCHEDULE, etc.)
? Each has separate src/ and build/ directories
? CMake configured to build all workspaces
? Git branches set up for all team members
? Tech lead has coordination workspace

? **Team ready to begin development on Monday morning!**

---

## ?? SUPPORT

If you get stuck during implementation:

1. Check this guide step-by-step
2. Verify directory structure matches
3. Check CMakeLists.txt syntax
4. Ensure Git branches are created
5. Run `cmake --verbose` for build errors

---

**Status**: Ready to implement
**Time required**: 2-4 hours
**Complexity**: Medium
**Result**: Professional isolated workspaces for 6 developers

**Let's implement!** ??

