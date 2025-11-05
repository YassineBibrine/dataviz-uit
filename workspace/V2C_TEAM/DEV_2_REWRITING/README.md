# ?? V2C Dev 2 - Source Rewriting Workspace

Welcome to your isolated development workspace!

---

## ?? YOUR ROLE

**V2C Dev 2 - Source Rewriting**
- Weeks 4-7
- Tasks: 2.1 ? 2.2 ? 2.3 ? 2.4
- Team: V2C (with Dev 1)
- Parallel with Dev 1

---

## ? 5-MINUTE QUICK START

### Your Tasks:
```
Task 2.1: Rewriting Framework (Week 4)
Task 2.2: Source Rewriting Implementation (Week 4-5)
Task 2.3: Bidirectional Sync (Week 5-6)
Task 2.4: V2C Tests & Integration (Week 6-7)
```

### Your Deliverables:
- `clang_rewriter.h/.cpp` - Clang-based rewriting
- `sync_manager.h/.cpp` - Bidirectional sync
- Complete test suite

### Your Dependencies:
- **Input from**: Dev 1 (generated code stream)
- **Input from**: C2V (original AST)
- **Output to**: Modified source files

---

## ?? YOUR WORKSPACE

```
workspace/V2C_TEAM/DEV_2_REWRITING/
??? README.md
??? TASKS.md
??? SCHEDULE.md
??? GETTING_STARTED.md
??? docs/
?   ??? DEVELOPER_GUIDE.md
?   ??? CLANG_REWRITER.md
?   ??? SOURCE_MODIFICATION.md
?   ??? BIDIRECTIONAL_SYNC.md
?   ??? TESTING.md
??? src/
?   ??? source_rewriting/
?   ?   ??? clang_rewriter.h
?   ?   ??? clang_rewriter.cpp
?   ?   ??? tests/
?   ??? bidirectional_sync/
?   ?   ??? sync_manager.h
?   ?   ??? sync_manager.cpp
?   ?   ??? tests/
?   ??? CMakeLists.txt
??? BLOCKERS.md
??? build/
```

---

## ?? KEY FILES

### File 1: `src/source_rewriting/clang_rewriter.h`
```cpp
#pragma once
#include <string>
#include <memory>
#include <clang/Rewrite/Core/Rewriter.h>

class ClangRewriter {
public:
    ClangRewriter();
    ~ClangRewriter();
    
    bool rewriteSource(const std::string& srcFile,
        const std::string& generatedCode,
        const std::string& outputFile);
    
    std::string getRewriteReport() const;
};
```

---

## ?? BUILD

```bash
cd workspace/V2C_TEAM/DEV_2_REWRITING
mkdir -p build && cd build
cmake .. -G Ninja
ninja
ctest
git push origin v2c-dev-2-rewriting
```

---

## ?? TIMELINE

### Week 4-5:
- Clang Rewriter setup
- Source modification impl
- Initial tests

### Week 5-6:
- Bidirectional sync design
- Sync implementation
- Integration with Dev 1

### Week 6-7:
- Full V2C pipeline
- Performance tuning
- Handoff

---

## ?? WORKING WITH DEV 1

### Coordination:
- **Dev 1**: Generates code stream
- **You**: Apply to source files
- **Together**: Full bidirectional sync

### Daily Communication:
- 10 AM: Standup (you + Dev 1)
- Alignment on code generation format
- Sync algorithm collaboration

---

## ? SUCCESS

### Week 4-5:
- [ ] Source rewriting working
- [ ] Clang integration stable
- [ ] Tests 80%+ coverage

### Week 6-7:
- [ ] Bidirectional sync working
- [ ] Full V2C pipeline complete
- [ ] Performance targets met

---

**Status**: ? Ready
**Start**: Week 4 (parallel with Dev 1)
**Questions**: See GETTING_STARTED.md

