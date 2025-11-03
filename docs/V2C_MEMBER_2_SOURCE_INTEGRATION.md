# V2C Team - Member 2: Source Rewriting & API Mapping

**Team Member Role**: Source Integration Lead  
**Primary Focus**: Clang Rewriter and API Mapper Components  
**Files to Implement**: 
- `src/synthesis/code_generation/clang_rewriter.h/.cpp`
- `src/synthesis/api_mapping/api_mapper.h/.cpp`

---

## Overview

Your responsibility is to build the **Source Code Rewriting** and **API Mapping** components of the V2C (Visualization-to-Code) Synthesis Engine. These components handle inserting generated code into existing source files and maintaining fidelity with user-defined data structure APIs.

### Key Responsibilities

1. **Clang Rewriter** - Insert generated code into source files using Clang infrastructure
2. **AST Navigation** - Parse and manipulate Abstract Syntax Trees
3. **API Mapping** - Track and manage user-defined data structure method signatures
4. **File I/O** - Read and write modified C++ source files

---

## Component 1: ClangRewriter

**File**: `src/synthesis/code_generation/clang_rewriter.h/.cpp`

### Purpose
Use LLVM/Clang infrastructure to safely insert generated C++ code into existing source files while maintaining proper AST structure and formatting.

### Header File (clang_rewriter.h)

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>

// Forward declarations for Clang types
namespace clang {
    class CompilerInstance;
    class Rewriter;
    class SourceManager;
    class FileID;
}

/**
 * Manages source code rewriting using Clang infrastructure
 * Enables insertion, replacement, and removal of code at precise locations
 */
class ClangRewriter {
public:
    ClangRewriter();
    ~ClangRewriter();

 /**
  * Initialize and open a C++ source file for modification
     * 
   * @param sourceFile Path to the C++ file to open
     * @return True if file opened successfully, false otherwise
     * 
     * Usage: Must be called before any insert/replace/remove operations
     */
  bool openFile(const std::string &sourceFile);

    /**
     * Close the currently open file
     * Cleans up Clang resources
     * 
     * @return True if closed successfully
     */
    bool closeFile();

    /**
     * Get currently open file path
     * 
     * @return Path to open file, or empty string if no file open
     */
    std::string getCurrentFile() const;

    /**
     * Insert code at a specific line and column location
     * 
     * @param code Code string to insert
     * @param lineNumber Line number (1-based indexing)
     * @param columnNumber Column number (0-based indexing)
     * @return True if insertion successful, false otherwise
     * 
     * Example: insertCode("int x = 5;", 10, 0) inserts at line 10, column 0
     * 
     * Note: Insertion point can be in the middle of a line
     */
    bool insertCode(
        const std::string &code,
        int lineNumber,
        int columnNumber);

    /**
   * Insert code at end of a function
 * 
   * @param code Code to insert
  * @param functionName Name of function to target
     * @return True if insertion successful
     * 
     * Example: insertAtFunctionEnd("logger.log(\"Done\");", "processGraph")
     * Finds the closing brace of the named function
     */
    bool insertAtFunctionEnd(
  const std::string &code,
 const std::string &functionName);

    /**
     * Insert code after a specific line
     * 
     * @param code Code to insert
     * @param lineNumber Line number after which to insert (1-based)
     * @return True if insertion successful
     */
    bool insertAfterLine(
  const std::string &code,
 int lineNumber);

    /**
     * Insert code before a specific line
     * 
     * @param code Code to insert
     * @param lineNumber Line number before which to insert (1-based)
     * @return True if insertion successful
     */
  bool insertBeforeLine(
        const std::string &code,
        int lineNumber);

    /**
     * Replace a range of code with new code
     * 
     * @param startLine Start line of range (1-based)
     * @param startCol Start column of range (0-based)
     * @param endLine End line of range (1-based)
     * @param endCol End column of range (0-based)
   * @param replacement Replacement code
 * @return True if replacement successful
     * 
   * Example: replaceCode(5, 0, 5, 20, "new_code");
     * Replaces first 20 characters of line 5
     */
    bool replaceCode(
        int startLine,
      int startCol,
     int endLine,
        int endCol,
  const std::string &replacement);

    /**
     * Remove code in a specified range
     * 
     * @param startLine Start line (1-based)
     * @param startCol Start column (0-based)
     * @param endLine End line (1-based)
   * @param endCol End column (0-based)
     * @return True if removal successful
     */
    bool removeCode(
        int startLine,
        int startCol,
        int endLine,
   int endCol);

    /**
     * Write modified code to output file
     * 
     * @param outputFile Output file path (can be same as source to overwrite)
     * @return True if write successful
     * 
     * Note: Call this after all insertions/replacements to save changes
     */
    bool writeModifiedCode(const std::string &outputFile);

    /**
     * Get the modified content as a string without writing to file
     * 
     * @return Modified code as string, or empty string if error
     */
    std::string getModifiedContent() const;

    /**
     * Get line count of currently open file
     * 
     * @return Number of lines, or 0 if no file open
     */
    int getLineCount() const;

    /**
     * Get content of a specific line
     * 
     * @param lineNumber Line number (1-based)
   * @return Line content, or empty string if line doesn't exist
     */
    std::string getLine(int lineNumber) const;

    /**
     * Find line number by searching for text pattern
   * 
     * @param pattern Text to search for
   * @param startLine Line to start search from (default 1)
     * @return Line number if found (1-based), or -1 if not found
     * 
     * Example: findLine("void processGraph()")
     */
    int findLine(
        const std::string &pattern,
        int startLine = 1) const;

    /**
     * Check if file has unsaved modifications
     * 
     * @return True if file has been modified but not written
     */
    bool hasModifications() const;

    /**
  * Discard all modifications and reload from disk
     * 
     * @return True if successful
     */
    bool discardModifications();

    /**
     * Get list of all modifications made so far
     * 
     * @return Vector of modification descriptions
     */
    std::vector<std::string> getModificationHistory() const;

private:
    std::unique_ptr<clang::CompilerInstance> compiler;
    std::unique_ptr<clang::Rewriter> rewriter;
    std::string currentFilePath;
    std::vector<std::string> modificationHistory;
    bool isModified;

    /**
     * Initialize Clang compiler instance
     */
    bool initializeCompiler();

/**
     * Parse the source file with Clang
     */
    bool parseSourceFile(const std::string &sourceFile);

    /**
     * Convert line/column to SourceLocation
     */
 clang::SourceLocation getSourceLocation(
        int lineNumber,
        int columnNumber) const;

    /**
     * Log a modification to history
     */
void logModification(const std::string &description);
};
```

### Implementation (clang_rewriter.cpp)

**TODO Items**:
- [ ] Implement `openFile()` - initialize Clang compiler and parse file
- [ ] Implement `closeFile()` - cleanup and release Clang resources
- [ ] Implement `getCurrentFile()` - return current file path
- [ ] Implement `insertCode()` - use Clang Rewriter API to insert at location
- [ ] Implement `insertAtFunctionEnd()` - find function and insert at closing brace
- [ ] Implement `insertAfterLine()` - insert after specified line
- [ ] Implement `insertBeforeLine()` - insert before specified line
- [ ] Implement `replaceCode()` - replace code in range
- [ ] Implement `removeCode()` - remove code in range
- [ ] Implement `writeModifiedCode()` - write RewriteBuffer to file
- [ ] Implement `getModifiedContent()` - get modified content as string
- [ ] Implement `getLineCount()` - return total line count
- [ ] Implement `getLine()` - retrieve specific line content
- [ ] Implement `findLine()` - search for pattern and return line number
- [ ] Implement `hasModifications()` - check modification flag
- [ ] Implement `discardModifications()` - reload from disk
- [ ] Implement `getModificationHistory()` - return modification log
- [ ] Helper: `initializeCompiler()` - setup CompilerInstance
- [ ] Helper: `parseSourceFile()` - parse with Clang
- [ ] Helper: `getSourceLocation()` - convert line/col to SourceLocation
- [ ] Helper: `logModification()` - track changes

### Dependencies

Add to your CMakeLists.txt:
```cmake
find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

target_link_libraries(clang_rewriter
    clang::clang
    ${LLVM_LIBRARIES}
)

target_include_directories(clang_rewriter PRIVATE
    ${LLVM_INCLUDE_DIRS}
${CLANG_INCLUDE_DIRS}
)
```

### Testing Checklist for ClangRewriter

```cpp
// Test 1: Open and read file
ClangRewriter rewriter;
assert(rewriter.openFile("test_source.cpp"));
assert(rewriter.getLineCount() > 0);

// Test 2: Insert code at location
assert(rewriter.insertCode("int x = 5;", 5, 0));

// Test 3: Insert after line
assert(rewriter.insertAfterLine("int y = 10;", 5));

// Test 4: Replace code
assert(rewriter.replaceCode(5, 0, 5, 10, "new code"));

// Test 5: Write modifications
assert(rewriter.writeModifiedCode("test_output.cpp"));

// Test 6: Find line by pattern
int lineNum = rewriter.findLine("void processGraph()");
assert(lineNum > 0);

// Test 7: Modification tracking
assert(rewriter.hasModifications());
std::vector<std::string> history = rewriter.getModificationHistory();
assert(history.size() > 0);
```

---

## Component 2: APIMapper

**File**: `src/synthesis/api_mapping/api_mapper.h/.cpp`

### Purpose
Maintain a registry of user-defined data structures and their method signatures. When generating code, the mapper ensures generated calls match the actual API of the user's classes.

### Header File (api_mapper.h)

```cpp
#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

/**
 * Represents a method signature for a data structure
 */
struct MethodSignature {
    std::string methodName;
    std::string returnType;
    std::vector<std::pair<std::string, std::string>> parameters;  // (type, name)
    
    /**
   * Convert to string representation
     * Example: "void addNode(const std::string &id)"
     */
    std::string toString() const;
};

/**
 * Represents a registered user-defined data structure
 */
struct DataStructureInfo {
    std::string structureName;
    std::string namespaceName;
    std::vector<MethodSignature> methods;
    
    /**
     * Check if structure has a specific method
     */
  bool hasMethod(const std::string &methodName) const;
    
    /**
     * Get method signature by name
 */
    MethodSignature getMethod(const std::string &methodName) const;
};

/**
 * Maps high-level graph operations to user-defined APIs
 */
class APIMapper {
public:
    APIMapper();
    ~APIMapper();

    /**
 * Register a user-defined data structure and its methods
     * 
     * @param structureName Name of the structure (e.g., "AdjacencyListGraph")
     * @param methods Vector of method signatures
     * 
     * Example:
     * std::vector<MethodSignature> methods = {
     *     {"addNode", "void", {{"const std::string &", "id"}}},
     *     {"addEdge", "void", {{"const std::string &", "from"}, ...}}
     * };
     * mapper.registerDataStructure("MyGraph", methods);
 */
    void registerDataStructure(
        const std::string &structureName,
        const std::vector<MethodSignature> &methods);

    /**
     * Register a data structure with alternative method names
     * Maps generic operations to user-specific method names
     * 
     * @param structureName Name of the structure
     * @param operationMappings Map of generic operation -> actual method name
     * 
     * Example:
     * mapper.registerOperationMappings("MyGraph", {
     *     {"add_node", "insert"},           // add_node() generic -> insert() actual
   *     {"add_edge", "connect"},      // add_edge() generic -> connect() actual
     *     {"remove_node", "erase"}
     * });
     */
    void registerOperationMappings(
        const std::string &structureName,
const std::map<std::string, std::string> &operationMappings);

/**
     * Get method signature for a registered data structure
     * 
     * @param structureName Name of the structure
   * @param methodName Name of the method
     * @return MethodSignature if found, empty signature if not found
     */
    MethodSignature getMethodSignature(
        const std::string &structureName,
        const std::string &methodName) const;

    /**
     * Check if a data structure has been registered
     * 
     * @param structureName Name to check
     * @return True if registered
     */
    bool hasDataStructure(const std::string &structureName) const;

    /**
     * Check if a data structure has a specific method
 * 
     * @param structureName Name of the structure
     * @param methodName Name of the method
     * @return True if method exists
     */
    bool hasMethod(
        const std::string &structureName,
    const std::string &methodName) const;

    /**
     * Map a generic graph operation to user-defined method call code
     * 
     * @param structureName User-defined data structure name
     * @param operation Generic operation (e.g., "add_node", "add_edge")
     * @param parameters Operation parameters
     * @param objectName Name of object instance to call method on
     * @return Generated method call code (e.g., "graph.insert(\"A\");")
     * 
     * Example:
     * mapper.mapGraphOperation("MyGraph", "add_node", 
     *     {{"id", "\"A\"}}, "myGraph")
     * // Returns: "myGraph.insert(\"A\");"
   */
    std::string mapGraphOperation(
        const std::string &structureName,
        const std::string &operation,
   const std::map<std::string, std::string> &parameters,
        const std::string &objectName = "graph");

    /**
     * Get all registered data structures
     * 
     * @return Vector of structure info
     */
    std::vector<DataStructureInfo> getAllDataStructures() const;

    /**
     * Get registry as debug string
     * 
     * @return Formatted string showing all registered structures and methods
     */
    std::string debugInfo() const;

    /**
     * Clear all registered structures
     */
    void clear();

    /**
     * Save registry to JSON file
     * 
     * @param filePath Path to save to
     * @return True if successful
     */
    bool saveRegistry(const std::string &filePath) const;

    /**
     * Load registry from JSON file
     * 
     * @param filePath Path to load from
     * @return True if successful
     */
    bool loadRegistry(const std::string &filePath);

private:
    // Map: structure name -> DataStructureInfo
    std::map<std::string, DataStructureInfo> registry;
    
    // Map: structure name -> operation mappings (generic -> actual method name)
    std::map<std::string, std::map<std::string, std::string>> operationMappings;

    /**
     * Find actual method name for an operation
     * Returns operation name if no mapping found
     */
    std::string getActualMethodName(
   const std::string &structureName,
        const std::string &operation) const;

    /**
     * Match and reorder parameters to method signature
  */
    std::map<std::string, std::string> matchParameters(
        const MethodSignature &signature,
        const std::map<std::string, std::string> &providedParams) const;
};
```

### Implementation (api_mapper.cpp)

**TODO Items**:
- [ ] Implement `MethodSignature::toString()` - format method signature as string
- [ ] Implement `DataStructureInfo::hasMethod()` - check for method existence
- [ ] Implement `DataStructureInfo::getMethod()` - retrieve method signature
- [ ] Implement `registerDataStructure()` - add structure to registry
- [ ] Implement `registerOperationMappings()` - map generic operations to actual methods
- [ ] Implement `getMethodSignature()` - lookup method by structure and name
- [ ] Implement `hasDataStructure()` - check if structure registered
- [ ] Implement `hasMethod()` - check if method exists in structure
- [ ] Implement `mapGraphOperation()` - generate method call for operation
- [ ] Implement `getAllDataStructures()` - return all registered structures
- [ ] Implement `debugInfo()` - format registry as debug string
- [ ] Implement `clear()` - empty all registries
- [ ] Implement `saveRegistry()` - serialize to JSON
- [ ] Implement `loadRegistry()` - deserialize from JSON
- [ ] Helper: `getActualMethodName()` - resolve operation to method name
- [ ] Helper: `matchParameters()` - align parameters to signature

### Testing Checklist for APIMapper

```cpp
// Test 1: Register data structure
APIMapper mapper;
std::vector<MethodSignature> methods = {
    {"addNode", "void", {{"const std::string &", "id"}}},
    {"addEdge", "void", {{"const std::string &", "from"}, {"const std::string &", "to"}}}
};
mapper.registerDataStructure("MyGraph", methods);
assert(mapper.hasDataStructure("MyGraph"));

// Test 2: Check method existence
assert(mapper.hasMethod("MyGraph", "addNode"));
assert(!mapper.hasMethod("MyGraph", "nonexistent"));

// Test 3: Get method signature
MethodSignature sig = mapper.getMethodSignature("MyGraph", "addNode");
assert(sig.methodName == "addNode");

// Test 4: Operation mapping
mapper.registerOperationMappings("MyGraph", {
    {"add_node", "addNode"},
    {"add_edge", "addEdge"}
});

// Test 5: Map operation to code
std::string code = mapper.mapGraphOperation("MyGraph", "add_node",
    {{"id", "\"A\"}}, "graph");
// Should return: "graph.addNode(\"A\");"

// Test 6: Debug info
std::string debug = mapper.debugInfo();
assert(!debug.empty());

// Test 7: Save/Load registry
mapper.saveRegistry("api_registry.json");
APIMapper mapper2;
mapper2.loadRegistry("api_registry.json");
assert(mapper2.hasDataStructure("MyGraph"));
```

---

## Integration Points

### Receiving Input From:
- **Member 1 (Code Generator)**: Receives generated code strings to insert
- **C2V Team**: Data structure information extracted from user code
- **UI Team**: User interactions trigger operations

### Outputs To:
- **Source Files**: Modified C++ code written back to disk
- **Member 1**: Returns formatted operation-specific method calls

### Data Flow Example

```
UI: User adds node "A"
 ?
APIMapper: Look up user's graph structure
 ?
APIMapper: Find actual method name (e.g., "insert" not "addNode")
 ?
Member 1 (Code Generator): Generate "graph.insert(\"A\");"
 ?
Code Formatter: Format the generated code
 ?
YOU (ClangRewriter): Insert at correct location in source
 ?
ClangRewriter: Write modified file to disk
```

---

## Build & Compilation

### CMakeLists.txt Integration

```cmake
# src/synthesis/CMakeLists.txt

# Clang Rewriter library
add_library(clang_rewriter
    code_generation/clang_rewriter.cpp
)
target_link_libraries(clang_rewriter PUBLIC clang::clang ${LLVM_LIBRARIES})

# API Mapper library  
add_library(api_mapper
    api_mapping/api_mapper.cpp
)
# May need JSON library for serialization (e.g., nlohmann/json)

# Synthesis library combines both
add_library(synthesis
    clang_rewriter
    api_mapper
    code_generation
)
```

### External Dependencies

Consider using:
- **nlohmann/json**: For JSON serialization in APIMapper
  ```bash
  # Add to project
  find_package(nlohmann_json REQUIRED)
  target_link_libraries(api_mapper nlohmann_json::nlohmann_json)
  ```

---

## Development Timeline

### Phase 1: ClangRewriter Basics (Days 1-3)
- [ ] Setup Clang compiler initialization
- [ ] Implement basic file open/parse
- [ ] Implement insertCode() at line/column
- [ ] Test with simple file modifications

### Phase 2: Advanced Rewriting (Days 4-5)
- [ ] Implement function-aware insertion
- [ ] Implement replace and remove operations
- [ ] Add modification history tracking
- [ ] Write comprehensive tests

### Phase 3: APIMapper Implementation (Days 6-7)
- [ ] Implement registration and lookup
- [ ] Add operation mapping support
- [ ] Implement JSON serialization
- [ ] Integration testing with Member 1

### Phase 4: Full Integration (Days 8-9)
- [ ] End-to-end testing with Code Generator
- [ ] Test with realistic data structures
- [ ] Performance optimization
- [ ] Error handling and edge cases

---

## Common Patterns & Examples

### Registering a Data Structure

```cpp
APIMapper mapper;

// Step 1: Create method signatures
std::vector<MethodSignature> methods;

MethodSignature addNode;
addNode.methodName = "addNode";
addNode.returnType = "void";
addNode.parameters = {{"const std::string &", "id"}};
methods.push_back(addNode);

MethodSignature addEdge;
addEdge.methodName = "addEdge";
addEdge.returnType = "void";
addEdge.parameters = {
    {"const std::string &", "from"},
    {"const std::string &", "to"},
    {"double", "weight"}
};
methods.push_back(addEdge);

// Step 2: Register structure
mapper.registerDataStructure("MyGraph", methods);

// Step 3: Map generic operations if different
mapper.registerOperationMappings("MyGraph", {
    {"add_node", "addNode"},
    {"add_edge", "addEdge"}
});
```

### Using ClangRewriter

```cpp
ClangRewriter rewriter;

// Open source file
if (!rewriter.openFile("graph_impl.cpp")) {
    std::cerr << "Failed to open file\n";
    return;
}

// Find function to modify
int funcLine = rewriter.findLine("void processGraph()");

// Insert code at function end
rewriter.insertAtFunctionEnd("logger.log(\"Done\");", "processGraph");

// Write changes back
rewriter.writeModifiedCode("graph_impl.cpp");

// Check what changed
std::vector<std::string> history = rewriter.getModificationHistory();
for (const auto &change : history) {
    std::cout << "Modified: " << change << "\n";
}
```

---

## Handling Clang Errors

When working with Clang rewriter:

```cpp
// Always check return values
if (!rewriter.openFile(sourceFile)) {
  // Handle error - file may not compile, missing dependencies, etc.
return false;
}

// Validate line/column exist before modification
int lineCount = rewriter.getLineCount();
if (lineNumber > lineCount) {
    // Line doesn't exist in file
    return false;
}

// Check modifications before writing
if (rewriter.hasModifications()) {
    if (!rewriter.writeModifiedCode(outputFile)) {
        // Write failed - disk full, permissions, etc.
        return false;
    }
}
```

---

## Documentation & Comments

Ensure your code includes:
- **Doxygen-style comments** for public APIs
- **Error documentation** explaining failure modes
- **Usage examples** in class documentation
- **Parameter validation** with clear error messages

---

## Related Files (Reference)

- Code Generator (Member 1): `src/synthesis/code_generation/ast_generator.h`
- Code Formatter (Member 1): `src/synthesis/code_generation/code_formatter.h`
- C2V Structure Extractor: `src/analysis/clang_integration/structure_extractor.h`
- CMakeLists: `src/synthesis/CMakeLists.txt`
- V2C Guide: `docs/V2C_GUIDE.md`

---

## Questions & Support

If you have questions about:
- **Clang API**: Check LLVM/Clang documentation links
- **API Mapping logic**: Coordinate with Member 1 on generated code format
- **File handling**: Ensure thread-safety for concurrent modifications
- **Testing**: Provide test files for reproducible testing

Good luck implementing the Source Integration Engine! ??
