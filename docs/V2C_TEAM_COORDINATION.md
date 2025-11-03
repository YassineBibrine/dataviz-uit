# V2C Team Coordination Guide

**Purpose**: Enable seamless collaboration between V2C Team Members 1 & 2  
**Last Updated**: $(date)

---

## Team Structure

| Role | Member | Component | Files |
|------|--------|-----------|-------|
| **Code Generation Lead** | Member 1 | AST Generator & Code Formatter | `ast_generator.*`, `code_formatter.*` |
| **Source Integration Lead** | Member 2 | Clang Rewriter & API Mapper | `clang_rewriter.*`, `api_mapper.*` |

---

## Integration Architecture

### The V2C Pipeline Flow

```
???????????????????????????????????????????????????????????????
? USER INTERACTION (from UI Team)        ?
? Example: User clicks "Add Node A"         ?
???????????????????????????????????????????????????????????????
     ?
            ?
        ??????????????????????????????
        ?  MEMBER 2: API Mapper      ?
      ?  ========================= ?
    ?  • Query user structure    ?
        ?  • Find actual method name ?
        ?  • Build parameter map     ?
      ??????????????????????????????
             ?
                ? Parameters: {id: "A"}
        ??????????????????????????????
        ?  MEMBER 1: AST Generator   ?
    ?  ========================= ?
        ?  • Generate: graph.addNode ?
        ?  • Format parameters  ?
        ?  • Return: "graph.add..."  ?
        ??????????????????????????????
      ?
           ? Code: "graph.addNode(\"A\");"
        ??????????????????????????????
        ?  MEMBER 1: Code Formatter  ?
        ?  ========================= ?
?  • Apply clang-format      ?
        ?  • Ensure proper spacing   ?
  ?  • Return formatted code   ?
   ??????????????????????????????
   ?
              ? Formatted code
  ??????????????????????????????
        ?  MEMBER 2: Clang Rewriter  ?
        ?  ========================= ?
        ?  • Parse source file       ?
 ?  • Find insertion point    ?
        ?  • Insert code    ?
    ?  • Write to disk ?
        ??????????????????????????????
           ?
        ?
    ??????????????????????????????
     ?  MODIFIED SOURCE FILE      ?
        ?  (Ready for C2V re-parse)  ?
        ??????????????????????????????
```

---

## Communication Contract

### Member 1 ? Member 2 (Code Generation Output)

**What Member 1 produces:**
- Syntactically valid C++ code strings
- Properly escaped string parameters
- Formatted according to selected style
- Example: `"graph.addNode(\"Node A\", label=\"A\");"`

**Contract guarantees:**
- Code is ready to insert (no compilation needed before insertion)
- Parameters are properly formatted (quotes, escapes handled)
- Code includes proper semicolons and punctuation
- Returned code is a single statement or block

**Error handling:**
- Returns empty string on error
- Logs errors to stderr
- Throws exception only for critical failures

---

### Member 2 ? Member 1 (API Information)

**What Member 2 provides:**
- User-defined data structure APIs
- Actual method names and signatures
- Parameter order and types
- Example: "User's Graph class has method `void insert(const std::string &id, const std::string &label)`"

**Contract guarantees:**
- Correct method signatures from user code
- Accurate parameter ordering
- Valid C++ syntax in signatures
- Thread-safe registry access

**Error handling:**
- Returns empty MethodSignature if method not found
- Returns false from hasMethod() if structure not registered
- Provides debug info for troubleshooting

---

## Integration Points - Detailed

### Point 1: Operation Mapping

**Scenario**: User adds a node through UI

**Member 2 Actions**:
```cpp
// 1. Determine operation type
std::string operation = "add_node";
std::map<std::string, std::string> params = {{"id", "A"}};
std::string structureName = "AdjacencyListGraph";  // From C2V Team

// 2. Get actual method name
std::string actualMethod = mapper.getActualMethodName(
    structureName, 
 operation
);  // Returns "insert" or "add_node" based on user's API

// 3. Call Member 1's generator
std::string code = astGenerator.generateFunctionCall(
    "graph",
  actualMethod,
params
);  // Returns: "graph.insert(\"A\");"
```

**Member 1 Actions**:
```cpp
// Return properly formatted code
return "graph.insert(\"A\");";  // Member 2 will format this next
```

---

### Point 2: Code Formatting

**Member 1 Actions**:
```cpp
// Format the generated code
std::string formatted = formatter.formatCode(unformattedCode);
// Returns: "graph.insert(\"A\");"  (with proper spacing)
```

**Member 2 Actions**:
```cpp
// Receives formatted code and prepares for insertion
// Now ready to pass to Clang Rewriter
rewriter.insertCode(formattedCode, lineNumber, columnNumber);
```

---

### Point 3: Source File Modification

**Member 2 Actions**:
```cpp
// 1. Open source file
rewriter.openFile("user_graph_impl.cpp");

// 2. Locate insertion point (e.g., in main function)
int lineNum = rewriter.findLine("int main()");
int insertLine = lineNum + 3;  // After opening brace

// 3. Insert generated code
rewriter.insertCode(formattedCodeFromMember1, insertLine, 0);

// 4. Write changes
rewriter.writeModifiedCode("user_graph_impl.cpp");
```

---

## Shared Responsibilities

### Code Style Configuration

Both members need to agree on and support these styles:
- **LLVM** (default) - `CodeStyle::LLVM`
- **Google** - `CodeStyle::Google`
- **Mozilla** - `CodeStyle::Mozilla`
- **WebKit** - `CodeStyle::WebKit`
- **Microsoft** - `CodeStyle::Microsoft`

**Implementation**:
- Member 1: Apply via `CodeFormatter::formatCode(code, style)`
- Member 2: Configure via `clang_rewriter` initialization

---

### Parameter Handling

**Agreement on parameter formats**:

| Type | Format | Example |
|------|--------|---------|
| String | Quoted, escaped | `"\"Node A\""` |
| Integer | No quotes | `"42"` |
| Float | No quotes | `"3.14"` |
| Boolean | Lowercase | `"true"` or `"false"` |
| Variable | No quotes | `"count"` |

**Member 1 responsibility**: Format parameters correctly  
**Member 2 responsibility**: Trust Member 1's formatting

---

### Error Recovery

**If Member 1 generates invalid code:**
- Member 2 attempts insertion anyway
- Clang may fail during parsing
- Member 2 reports error back to Member 1 with details

**If Member 2 cannot insert code:**
- Check file is valid C++ (can be parsed)
- Verify line/column numbers are valid
- Check if code is syntactically valid
- Report back to Member 1 with specifics

---

## Testing Strategy

### Unit Tests (Independent)

**Member 1 tests** (in `tests/test_code_generation.cpp`):
```cpp
TEST(ASTGenerator, FunctionCall) {
    ASTGenerator gen;
    std::string result = gen.generateFunctionCall("graph", "addNode", 
    {{"id", "\"A\""}});
    EXPECT_EQ(result, "graph.addNode(\"A\");");
}

TEST(CodeFormatter, FormatWithStyle) {
    CodeFormatter formatter;
  formatter.setStyle(CodeStyle::Google);
    std::string result = formatter.formatCode("int x=5;");
    // Should have proper spacing
    EXPECT_TRUE(result.find("int x = 5") != std::string::npos);
}
```

**Member 2 tests** (in `tests/test_source_integration.cpp`):
```cpp
TEST(ClangRewriter, OpenAndReadFile) {
    ClangRewriter rewriter;
    ASSERT_TRUE(rewriter.openFile("sample.cpp"));
    EXPECT_GT(rewriter.getLineCount(), 0);
}

TEST(APIMapper, RegisterAndQuery) {
    APIMapper mapper;
    std::vector<MethodSignature> methods = {...};
    mapper.registerDataStructure("MyGraph", methods);
    EXPECT_TRUE(mapper.hasMethod("MyGraph", "addNode"));
}
```

### Integration Tests (Together)

**End-to-end test** (in `tests/test_v2c_pipeline.cpp`):
```cpp
TEST(V2CPipeline, GenerateAndInsert) {
    // Setup
    APIMapper mapper;
  ASTGenerator gen;
    CodeFormatter formatter;
    ClangRewriter rewriter;
    
    // Register API
    mapper.registerDataStructure("Graph", {...});
    
    // Generate code
    std::string code = gen.generateFunctionCall("graph", "addNode",
      {{"id", "\"A\""}});
    
    // Format code
    code = formatter.formatCode(code);
    
    // Insert into file
    rewriter.openFile("test_graph.cpp");
    rewriter.insertAfterLine(code, 5);
    rewriter.writeModifiedCode("test_graph_output.cpp");
    
    // Verify file is valid C++
    ClangRewriter verifier;
 ASSERT_TRUE(verifier.openFile("test_graph_output.cpp"));
}
```

### Test Files

Create shared test fixtures in `tests/fixtures/`:
```
tests/fixtures/
??? sample_graph.cpp     # Sample C++ file with Graph class
??? sample_graph.h   # Header file
??? expected_output.cpp      # Expected output after modifications
??? api_registry.json      # Test API registry
??? clang_format_config        # Test clang-format config
```

---

## Data Structures

### Shared Understanding: MethodSignature

Both members must implement this consistently:

```cpp
struct MethodSignature {
    std::string methodName;// e.g., "addNode"
    std::string returnType;    // e.g., "void"
    std::vector<std::pair<std::string, std::string>> parameters;
    //           parameter type            parameter name
    // e.g., {{"const std::string &", "id"}, {"int", "weight"}}
};
```

**Agreed string format** for Member 2 to store:
```
"void addNode(const std::string &id, int weight)"
```

---

## Build Integration

### CMakeLists.txt Structure

```cmake
# src/synthesis/CMakeLists.txt

# Member 1's components
add_library(ast_generator
    code_generation/ast_generator.cpp
)

add_library(code_formatter
    code_generation/code_formatter.cpp
)

# Member 2's components
add_library(clang_rewriter
    code_generation/clang_rewriter.cpp
)
target_link_libraries(clang_rewriter ${LLVM_LIBRARIES} clang::clang)

add_library(api_mapper
    api_mapping/api_mapper.cpp
)

# Combined synthesis library
add_library(synthesis
    ast_generator
    code_formatter
    clang_rewriter
    api_mapper
)

# Tests
add_executable(test_v2c
    ../tests/test_code_generation.cpp
    ../tests/test_source_integration.cpp
    ../tests/test_v2c_pipeline.cpp
)
target_link_libraries(test_v2c synthesis gtest gtest_main)
```

---

## Development Phases

### Week 1: Individual Implementation
- **Member 1**: Implement AST Generator & Code Formatter
- **Member 2**: Implement Clang Rewriter & API Mapper
- Both: Write unit tests

### Week 2: Integration
- **Day 1**: Code review between members
- **Day 2**: Write integration tests
- **Day 3-4**: Debug integration issues
- **Day 5**: Performance tuning & documentation

### Week 3: Polish & Testing
- End-to-end testing with real data structures
- Error handling and edge cases
- Documentation completion
- Demo readiness

---

## Communication Best Practices

### Daily Standup Topics

Each member reports:
1. **Completed**: What was implemented/tested
2. **Blocked**: Any issues preventing progress
3. **Next**: What's planned for next session
4. **Integration**: Any concerns about other member's component

### Code Review Checklist

Before integration testing:
- [ ] Both components compile without warnings
- [ ] Unit tests pass (100% pass rate)
- [ ] Code follows agreed style guidelines
- [ ] Comments document all public APIs
- [ ] Error handling is comprehensive
- [ ] Performance is acceptable
- [ ] No undefined behavior (check with sanitizers)

### Problem Resolution

If integration fails:
1. **Isolate**: Run each component's unit tests
2. **Identify**: Determine which component is problematic
3. **Log**: Document exact error with inputs/outputs
4. **Debug**: Use provided test fixtures
5. **Communicate**: Share findings with other member
6. **Fix**: Implement fix in responsible component
7. **Verify**: Re-run integration tests

---

## Success Criteria

### Member 1 Success
- [ ] All AST Generator methods implemented and tested
- [ ] Code Formatter works with all 5 style types
- [ ] Parameters are properly formatted (100% accuracy)
- [ ] Generated code compiles in sample files
- [ ] Integration tests pass

### Member 2 Success
- [ ] Clang Rewriter can open, parse, and modify files
- [ ] All modification types working (insert, replace, remove)
- [ ] API Mapper registry accurate and thread-safe
- [ ] File writes preserve valid C++ syntax
- [ ] Integration tests pass

### Team Success
- [ ] End-to-end pipeline works: Operation ? Code ? Format ? Insert ? File
- [ ] Generated code matches user APIs exactly
- [ ] Bidirectional mapping verified (visual ? code)
- [ ] Performance acceptable for real-time feedback
- [ ] Comprehensive documentation for future maintenance

---

## Troubleshooting Guide

### Issue: "Code doesn't compile after insertion"
**Possible causes:**
1. Member 1: Generated code has syntax errors
2. Member 2: Inserted at wrong location (inside string literal, comment, etc.)
3. Member 2: Didn't include proper whitespace/indentation

**Resolution:**
- Member 1: Print generated code to verify syntax
- Member 2: Verify line content with `getLine()` before inserting
- Member 2: Check modification history

### Issue: "Method signature mismatch"
**Possible causes:**
1. Member 2: API not registered or incorrectly extracted
2. Member 1: Parameter order doesn't match signature
3. Mismatch between expected vs actual parameter types

**Resolution:**
- Member 2: Verify with `debugInfo()` that API is correct
- Member 1: Check against actual method signature
- Both: Review MethodSignature struct agreement

### Issue: "File not parsing after modification"
**Possible causes:**
1. Invalid C++ syntax in generated code
2. Insertion broke valid syntax
3. Malformed identifier or operator

**Resolution:**
- Member 1: Generate simpler code first (test incrementally)
- Member 2: Use fallback testing - write to disk and verify with compiler
- Both: Use provided test files for validation

---

## Contact & Escalation

### Regular Communication
- **Daily Standup**: [Time/Location]
- **Code Review**: [Schedule]
- **Integration Testing**: [Schedule]

### Escalation Path
1. Try to resolve between members
2. Consult with V2C Team Lead
3. Escalate to Architecture Team if needed

---

## Appendix: Useful Commands

### Build Commands
```bash
# Build synthesis library
cd build
cmake -DLLVM_DIR=/path/to/llvm ..
cmake --build . --target synthesis

# Run tests
cmake --build . --target test_v2c

# Build with verbose output
cmake --build . --verbose
```

### Testing Commands
```bash
# Run only unit tests
ctest --output-on-failure -R "test_code_generation|test_source_integration"

# Run with specific filter
ctest -R "ASTGenerator" --verbose

# Memory leak detection
valgrind --leak-check=full ./build/test_v2c
```

### Debugging Commands
```bash
# Debug with clang-format
clang-format --style=LLVM --output-replacements-xml sample.cpp

# Check file syntax
clang-cc1 -ast-dump sample.cpp

# Verify rewritten file
clang-format -i test_output.cpp  # Format for inspection
```

---

## References

- [Clang Rewriter API](https://clang.llvm.org/docs/LibTooling.html)
- [Clang AST Documentation](https://clang.llvm.org/docs/IntroductionToTheClangAST.html)
- [clang-format Styles](https://clang.llvm.org/docs/ClangFormat/)
- V2C Guide: `docs/V2C_GUIDE.md`
- Member 1 Tasks: `docs/V2C_MEMBER_1_CODE_GENERATION.md`
- Member 2 Tasks: `docs/V2C_MEMBER_2_SOURCE_INTEGRATION.md`

---

**Created**: $(date)  
**Last Modified**: $(date)  
**Status**: Active Development
