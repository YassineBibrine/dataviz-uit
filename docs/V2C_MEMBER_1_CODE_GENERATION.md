# V2C Team - Member 1: Code Generation Engine

**Team Member Role**: Code Generation Lead  
**Primary Focus**: AST Generation and Code Formatting  
**Files to Implement**: 
- `src/synthesis/code_generation/ast_generator.h/.cpp`
- `src/synthesis/code_generation/code_formatter.h/.cpp`

---

## Overview

Your responsibility is to build the **Code Generation** component of the V2C (Visualization-to-Code) Synthesis Engine. This component transforms user interactions and API operations into syntactically correct, well-formatted C++ code that can be inserted into source files.

### Key Responsibilities

1. **Code Generation** - Generate valid C++ syntax from operation requests
2. **Parameter Handling** - Properly escape and format parameters
3. **Code Formatting** - Apply professional formatting using clang-format
4. **Style Management** - Support multiple coding styles

---

## Component 1: ASTGenerator

**File**: `src/synthesis/code_generation/ast_generator.h/.cpp`

### Purpose
Generate syntactically correct C++ code statements from high-level operation descriptions. This acts as the bridge between visual operations and C++ syntax.

### Header File (ast_generator.h)

```cpp
#pragma once

#include <string>
#include <map>
#include <vector>

class ASTGenerator {
public:
    ASTGenerator();
    ~ASTGenerator();

    /**
     * Generate a function call statement
     * Example: graph.addNode("A")
   * 
     * @param objectName The object to call method on (e.g., "graph")
     * @param functionName The method name (e.g., "addNode")
     * @param parameters Key-value pairs of parameters
   * @return Generated code string
     */
    std::string generateFunctionCall(
        const std::string &objectName,
        const std::string &functionName,
        const std::map<std::string, std::string> &parameters);

    /**
     * Generate a variable assignment statement
     * Example: int count = 10;
     * 
     * @param varName Variable name
     * @param value Value to assign
     * @return Generated code string
     */
    std::string generateVariableAssignment(
        const std::string &varName,
        const std::string &value);

    /**
     * Generate node creation for graph structures
  * Example: graph.addNode("A", label="Node A")
     * 
 * @param nodeId Unique node identifier
     * @param properties Optional node properties (key-value pairs)
     * @return Generated code string
   */
    std::string generateNodeCreation(
        const std::string &nodeId,
        const std::map<std::string, std::string> &properties = {});

    /**
     * Generate edge creation between nodes
     * Example: graph.addEdge("A", "B", weight=5.0)
     * 
     * @param fromNode Source node identifier
     * @param toNode Destination node identifier
     * @param properties Optional edge properties
 * @return Generated code string
     */
    std::string generateEdgeCreation(
        const std::string &fromNode,
const std::string &toNode,
        const std::map<std::string, std::string> &properties = {});

    /**
     * Generate a loop statement
     * Example: for(int i = 0; i < 10; i++)
     * 
     * @param initExpression Loop initialization
     * @param condition Loop condition
* @param increment Loop increment
     * @param body Loop body statements
     * @return Generated code string
     */
    std::string generateLoop(
        const std::string &initExpression,
  const std::string &condition,
        const std::string &increment,
    const std::vector<std::string> &body);

 /**
     * Generate an if statement
     * Example: if(count > 5) { ... }
     * 
     * @param condition Condition expression
     * @param thenBranch Then branch statements
     * @param elseBranch Optional else branch statements
     * @return Generated code string
     */
    std::string generateIfStatement(
        const std::string &condition,
        const std::vector<std::string> &thenBranch,
        const std::vector<std::string> &elseBranch = {});

    /**
     * Format a parameter value for use in code
     * Handles string escaping, quote management, and type detection
     * 
     * @param value Raw parameter value
     * @return Formatted parameter ready for insertion
     */
    std::string formatParameter(const std::string &value);

private:
    /**
     * Check if a string represents a numeric value
     */
    bool isNumeric(const std::string &value) const;

  /**
     * Escape special characters in a string
     */
 std::string escapeString(const std::string &str) const;

    /**
     * Build parameter list string from map
     */
    std::string buildParameterList(
const std::map<std::string, std::string> &parameters,
        bool includeKeyNames = false);
};
```

### Implementation (ast_generator.cpp)

**TODO Items**:
- [ ] Implement `generateFunctionCall()` - generates `object.method(params);`
- [ ] Implement `generateVariableAssignment()` - generates `var = value;`
- [ ] Implement `generateNodeCreation()` - generates graph node operations
- [ ] Implement `generateEdgeCreation()` - generates graph edge operations
- [ ] Implement `generateLoop()` - generates for/while loops
- [ ] Implement `generateIfStatement()` - generates if/else statements
- [ ] Implement `formatParameter()` - handles string escaping and type detection
- [ ] Implement `isNumeric()` - detects numeric values
- [ ] Implement `escapeString()` - escapes quotes and special characters
- [ ] Implement `buildParameterList()` - constructs parameter strings

### Testing Checklist for ASTGenerator

```cpp
// Test 1: Simple function call
ASTGenerator gen;
std::string result = gen.generateFunctionCall("graph", "addNode", {{"id", "\"A\""}});
// Expected: "graph.addNode(\"A\");"
assert(result == "graph.addNode(\"A\");");

// Test 2: Variable assignment
result = gen.generateVariableAssignment("count", "10");
// Expected: "count = 10;"
assert(result == "count = 10;");

// Test 3: Node creation with properties
std::map<std::string, std::string> props = {
    {"label", "\"Node A\""}, 
    {"color", "\"red\""}
};
result = gen.generateNodeCreation("A", props);
// Expected: "graph.addNode(\"A\", label=\"Node A\", color=\"red\");"

// Test 4: Edge creation
std::map<std::string, std::string> edgeProps = {{"weight", "5.0"}};
result = gen.generateEdgeCreation("A", "B", edgeProps);
// Expected: "graph.addEdge(\"A\", \"B\", weight=5.0);"

// Test 5: String escaping
result = gen.formatParameter("Hello \"World\"");
// Should properly escape internal quotes
```

---

## Component 2: CodeFormatter

**File**: `src/synthesis/code_generation/code_formatter.h/.cpp`

### Purpose
Format generated C++ code using clang-format to ensure professional, consistent output that matches project style guidelines.

### Header File (code_formatter.h)

```cpp
#pragma once

#include <string>

/**
 * Code formatting styles
 */
enum class CodeStyle {
    LLVM,    // LLVM style (default)
    Google,     // Google style
    Mozilla,    // Mozilla style
    WebKit,     // WebKit style
    Microsoft   // Microsoft style
};

class CodeFormatter {
public:
CodeFormatter();
  ~CodeFormatter();

    /**
     * Format code using default LLVM style
     * 
     * @param code Unformatted code string
     * @return Formatted code string
     */
    std::string formatCode(const std::string &code);

    /**
     * Format code using specified style
     * 
     * @param code Unformatted code string
     * @param style Style to apply
     * @return Formatted code string
     */
    std::string formatCode(
     const std::string &code,
        CodeStyle style);

    /**
     * Format code using custom clang-format configuration file
 * 
     * @param code Unformatted code string
     * @param styleFilePath Path to .clang-format file
     * @return Formatted code string
     */
    std::string formatCodeWithFile(
        const std::string &code,
        const std::string &styleFilePath);

    /**
     * Set the default style for subsequent formatting calls
     * 
     * @param style Style to set as default
     */
    void setStyle(CodeStyle style);

    /**
     * Get the current default style
     * 
     * @return Current style
     */
 CodeStyle getStyle() const;

    /**
   * Format and indent a single line
     * 
     * @param line The line to format
     * @param indentLevel Indentation level (0-based)
     * @return Indented line
   */
    std::string indentLine(
        const std::string &line,
        int indentLevel = 0);

    /**
     * Check if clang-format is available on system
  * 
     * @return True if clang-format executable found
     */
    static bool isClangFormatAvailable();

  /**
     * Get clang-format version
     * 
   * @return Version string or empty if not available
     */
    static std::string getClangFormatVersion();

private:
    CodeStyle currentStyle;
    static const int INDENT_SIZE = 4;  // Spaces per indentation level

    /**
     * Convert CodeStyle enum to clang-format style string
     */
    std::string styleToString(CodeStyle style) const;

    /**
     * Execute clang-format command and capture output
     */
    std::string executeClangFormat(
        const std::string &code,
    const std::string &styleArg);

    /**
     * Fallback formatting when clang-format unavailable
 */
    std::string fallbackFormat(const std::string &code);
};
```

### Implementation (code_formatter.cpp)

**TODO Items**:
- [ ] Implement `formatCode()` with default style
- [ ] Implement `formatCode()` with specified style
- [ ] Implement `formatCodeWithFile()` for custom .clang-format files
- [ ] Implement `setStyle()` and `getStyle()`
- [ ] Implement `indentLine()` for manual indentation
- [ ] Implement `isClangFormatAvailable()` - check system for clang-format
- [ ] Implement `getClangFormatVersion()` - query clang-format version
- [ ] Implement `styleToString()` - convert enum to style name
- [ ] Implement `executeClangFormat()` - shell out to clang-format binary
- [ ] Implement `fallbackFormat()` - basic formatting if clang-format unavailable

### Testing Checklist for CodeFormatter

```cpp
// Test 1: Check clang-format availability
assert(CodeFormatter::isClangFormatAvailable());

// Test 2: Format unindented code
CodeFormatter formatter;
std::string unformatted = "int x=5;if(x>3){x++;}\n";
std::string formatted = formatter.formatCode(unformatted);
// Should add proper spacing and newlines

// Test 3: Set style and format
formatter.setStyle(CodeStyle::Google);
formatted = formatter.formatCode(unformatted);
// Should follow Google style guidelines

// Test 4: Custom indentation
std::string result = formatter.indentLine("int x = 5;", 2);
// Expected: "        int x = 5;" (8 spaces)

// Test 5: Fallback formatting
if (!CodeFormatter::isClangFormatAvailable()) {
    std::string result = formatter.formatCode(unformatted);
    // Should still produce reasonable output
}
```

---

## Integration Points

### Receiving Input From:
- **Member 2 (Clang Rewriter & API Mapper)**: Will call your generator methods to create code before insertion
- **UI Team**: Signals about user interactions (need to convert to operation parameters)

### Outputs To:
- **Member 2**: Returns formatted C++ code strings ready for file insertion
- **Clang Rewriter**: Code is inserted using the output strings

### Data Flow Example

```
UI: User clicks "Add Node A"
 ?
Member 2 receives signal and determines operation
    ?
Member 2 queries APIMapper for method signature
    ?
Member 2 calls YOUR generateNodeCreation("A", {...})
    ?
YOU return: "graph.addNode(\"A\");"
    ?
YOU format the code
    ?
Member 2 passes formatted code to ClangRewriter
 ?
ClangRewriter inserts into source file
```

---

## Build & Compilation

### CMakeLists.txt Integration

Your implementation should be compiled as part of the synthesis library:

```cmake
# This will be in src/synthesis/code_generation/CMakeLists.txt
add_library(code_generation
    ast_generator.cpp
    code_formatter.cpp
)

target_include_directories(code_generation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
```

### Compiler Requirements
- C++17 or later (for structured bindings in range-based for loops)
- Clang-format executable for system integration
- Standard library support for `<sstream>`, `<map>`, `<vector>`

---

## Development Timeline

### Phase 1: Core Implementation (Days 1-3)
- [ ] Implement ASTGenerator header and basic methods
- [ ] Implement parameter formatting and escaping
- [ ] Write unit tests for code generation

### Phase 2: Formatting Integration (Days 4-5)
- [ ] Implement CodeFormatter header and methods
- [ ] Integrate clang-format command execution
- [ ] Test with various code styles

### Phase 3: Integration Testing (Days 6-7)
- [ ] Test with Member 2's ClangRewriter
- [ ] Verify generated code is valid C++
- [ ] Test edge cases and error handling

---

## Common Patterns & Examples

### Generating Function Calls

```cpp
// Simple call
ASTGenerator gen;
gen.generateFunctionCall("graph", "clear", {});
// Output: "graph.clear();"

// With string parameters
gen.generateFunctionCall("graph", "addNode", {
    {"id", "\"NodeA\""},
    {"label", "\"Node A\""}
});
// Output: "graph.addNode(\"NodeA\", \"Node A\");"

// With numeric parameters
gen.generateFunctionCall("array", "push", {
    {"value", "42"}
});
// Output: "array.push(42);"
```

### Formatting Code

```cpp
CodeFormatter formatter;

// Using default LLVM style
std::string code = "int x=5;if(x>3){x++;}\n";
std::string formatted = formatter.formatCode(code);

// Using specific style
formatter.setStyle(CodeStyle::Microsoft);
formatted = formatter.formatCode(code);

// Using custom file
formatted = formatter.formatCodeWithFile(code, ".clang-format-custom");
```

---

## Documentation & Comments

Ensure your code includes:
- **Doxygen-style comments** for all public methods
- **Inline comments** explaining complex parameter handling
- **Examples** in method documentation showing usage

---

## Related Files (Reference)

- API Mapper (Member 2): `src/synthesis/api_mapping/api_mapper.h`
- Clang Rewriter (Member 2): `src/synthesis/code_generation/clang_rewriter.h`
- CMakeLists: `src/synthesis/CMakeLists.txt`
- V2C Guide: `docs/V2C_GUIDE.md`

---

## Questions & Support

If you have questions about:
- **Parameter handling**: Check `formatParameter()` requirements
- **Style guidelines**: Refer to clang-format documentation
- **Integration**: Contact Member 2 early for API compatibility
- **Testing**: Follow unit test patterns in related modules

Good luck implementing the Code Generation Engine! ??
