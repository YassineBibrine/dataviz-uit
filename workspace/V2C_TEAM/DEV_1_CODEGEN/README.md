# ?? V2C Dev 1 - Code Generation Workspace

Welcome to your isolated development workspace!

---

## ?? YOUR ROLE

**V2C Dev 1 - Code Generation**
- Weeks 4-7
- Tasks: 1.1 ? 1.2 ? 1.3 ? 1.4
- Team: V2C (with Dev 2)
- Starts: After C2V complete (Week 4)

---

## ? 5-MINUTE QUICK START

### Your Tasks:
```
Task 1.1: CodeGen Design & API Mapping (Week 4)
Task 1.2: CodeGen Engine Implementation (Week 4-5)
Task 1.3: API Mapping Service (Week 5)
Task 1.4: Code Generation Tests (Week 5-6)
```

### Your Deliverables:
- `ast_generator.h/.cpp` - Generate code from AST
- `api_mapper.h/.cpp` - Map API calls
- Comprehensive test suite
- Code generation engine

### Your Dependencies:
- **Input from**: C2V Dev 2 (AST information)
- **Works with**: Dev 2 (source rewriting)
- **Output to**: Dev 2 (generated code stream)

---

## ?? YOUR WORKSPACE

```
workspace/V2C_TEAM/DEV_1_CODEGEN/
??? README.md
??? TASKS.md
??? SCHEDULE.md
??? GETTING_STARTED.md
??? docs/
?   ??? DEVELOPER_GUIDE.md
?   ??? LLVM_GUIDE.md
?   ??? AST_TO_CODE.md
?   ??? CODE_GENERATION.md
?   ??? TESTING.md
??? src/
???? code_generation/
?   ?   ??? ast_generator.h
?   ?   ??? ast_generator.cpp
?   ?   ??? tests/
?   ??? api_mapping/
?   ?   ??? api_mapper.h
?   ?   ??? api_mapper.cpp
?   ?   ??? tests/
?   ??? CMakeLists.txt
??? BLOCKERS.md
??? build/
```

---

## ?? KEY FILES

### File 1: `src/code_generation/ast_generator.h`
```cpp
#pragma once
#include <string>
#include <memory>
#include <clang/AST/AST.h>

class ASTInfo;

class ASTGenerator {
public:
    ASTGenerator();
    ~ASTGenerator();
    
    std::string generateCodeFromAST(const ASTInfo& astInfo);
    std::string generateFunction(const std::string& funcName);
    std::string generateClass(const std::string& className);
};
```

---

## ?? BUILD

```bash
cd workspace/V2C_TEAM/DEV_1_CODEGEN
mkdir -p build && cd build
cmake .. -G Ninja
ninja
ctest
git push origin v2c-dev-1-codegen
```

---

## ?? TIMELINE

### Week 4: Design
- Mon-Tue: Task 1.1 - Design API, interfaces
- Wed-Thu: Start Task 1.2 implementation
- Fri: Tests passing

### Week 5: Implementation
- Mon-Tue: Task 1.2 - CodeGen engine
- Wed: Task 1.3 - API mapping
- Thu-Fri: Task 1.4 - Complete tests

### Week 6-7: Integration
- Coordinate with Dev 2
- Fix any issues
- Performance optimization

---

## ?? WORKING WITH DEV 2

### Division:
- **Dev 1**: Generate code from AST
- **Dev 2**: Rewrite source files with generated code
- **Together**: Full V2C pipeline

### Handoff:
```
Week 4: You design ? Dev 2 learns API
Week 5: You implement ? Dev 2 uses in their code
Week 6: Integration ? Combined V2C working
```

---

## ? SUCCESS

### Week 4-5:
- [ ] CodeGen engine working
- [ ] API mapping complete
- [ ] Tests 80%+ coverage

### Week 6-7:
- [ ] Integrated with Dev 2
- [ ] Full V2C working
- [ ] Performance targets met

---

**Status**: ? Ready
**Start**: Week 4
**Questions**: See GETTING_STARTED.md

