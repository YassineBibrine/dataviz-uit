# V2C Team (Visualization-to-Code) Implementation Guide

## Overview

The V2C Team builds the **Synthesis Engine**, responsible for the inverse operation: translating user interactions with visualizations back into syntactically correct C++ source code. This enables the bidirectional code-visualization mapping.

## Team Responsibilities

### Primary Responsibilities
1. **Code Generation** - Generate C++ code from visual operations
2. **AST Manipulation** - Create and manipulate Abstract Syntax Trees
3. **Code Rewriting** - Insert generated code into source files using Clang Rewriter
4. **API Mapping** - Maintain fidelity with user-defined data structure APIs
5. **Code Formatting** - Ensure professional code output

### File Locations
All synthesis files are located in: `src/synthesis/`

```
src/synthesis/
??? CMakeLists.txt
??? code_generation/
?   ??? clang_rewriter.h/.cpp     ? Start here
?   ??? ast_generator.h/.cpp
?   ??? code_formatter.h/.cpp
??? api_mapping/
    ??? api_mapper.h/.cpp
```

## Component Details

### 1. ClangRewriter (Source Code Rewriting)
**File**: `src/synthesis/code_generation/clang_rewriter.h/.cpp`

**Responsibilities**:
- Open and parse existing C++ source files
- Maintain AST representation during modifications
- Insert generated code at specified locations
- Replace existing code
- Write modified code back to file

**Key Methods to Implement**:
```cpp
bool openFile(const std::string &sourceFile);

bool insertCode(const std::string &code, int lineNumber, int columnNumber);

bool replaceCode(int startLine, int startCol, int endLine, int endCol,
    const std::string &replacement);

bool writeModifiedCode(const std::string &outputFile);
```

**Implementation Approach**:

```cpp
// Step 1: Create compiler instance and parse file
clang::CompilerInstance compiler;
clang::SourceManager &SM = compiler.getSourceManager();

// Step 2: Initialize Rewriter with source manager and language options
clang::Rewriter rewriter(SM, compiler.getLangOpts());

// Step 3: Load source file
const clang::FileEntry *pFile = compiler.getFileManager().getFile(sourceFile);
compiler.getSourceManager().createFileID(pFile, clang::SourceLocation(),
    clang::SrcMgr::C_User);

// Step 4: Insert code at location
clang::SourceLocation loc = SM.getLocForStartOfFile(fileID).getLocWithOffset(offset);
rewriter.InsertText(loc, code, true, true);

// Step 5: Write modified file
const clang::RewriteBuffer *RewriteBuf = rewriter.getRewriteBufferFor(fileID);
if (RewriteBuf) {
    RewriteBuf->write(llvm::outs());
}
```

**Clang Rewriter Concepts**:
- **SourceLocation**: Precise file location (file ID + offset)
- **SourceRange**: Range between two SourceLocations
- **RewriteBuffer**: Maintains rewritten content for a file
- **Rewriter**: Main class for source modifications

**Key Headers to Include**:
```cpp
#include "clang/Rewriter/Rewriter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Lexer.h"
```

**Important Methods**:
```cpp
// Insert text at location
rewriter.InsertText(SourceLocation, StringRef, bool InsertAfter = false);

// Replace range with new text
rewriter.ReplaceText(SourceRange, StringRef);

// Get file by ID
const RewriteBuffer *getRewriteBufferFor(FileID FID);

// Remove text in range
rewriter.RemoveText(CharSourceRange);
```

### 2. ASTGenerator (Code Generation)
**File**: `src/synthesis/code_generation/ast_generator.h/.cpp`

**Responsibilities**:
- Generate syntactically correct C++ code
- Create function call statements
- Create variable assignments
- Create node and edge operations
- Return code as strings (ready for Rewriter)

**Key Methods to Implement**:
```cpp
std::string generateFunctionCall(const std::string &objectName,
    const std::string &functionName,
    const std::map<std::string, std::string> &parameters);

std::string generateVariableAssignment(const std::string &varName,
    const std::string &value);

std::string generateNodeCreation(const std::string &nodeId,
  const std::map<std::string, std::string> &properties = {});

std::string generateEdgeCreation(const std::string &fromNode,
    const std::string &toNode,
    const std::map<std::string, std::string> &properties = {});
```

**Implementation Examples**:

```cpp
// Generate function call: graph.addNode("A")
std::string ASTGenerator::generateFunctionCall(
    const std::string &objectName,
    const std::string &functionName,
    const std::map<std::string, std::string> &parameters)
{
    std::stringstream ss;
    ss << objectName << "." << functionName << "(";
    
    bool first = true;
    for (const auto &[key, value] : parameters) {
if (!first) ss << ", ";
        ss << formatParameter(value);
        first = false;
    }
    ss << ");";
    
    return ss.str();
 // Output: "graph.addNode(\"A\");"
}

// Generate variable assignment: int count = 10;
std::string ASTGenerator::generateVariableAssignment(
    const std::string &varName,
    const std::string &value)
{
    return varName + " = " + value + ";";
    // Output: "count = 10;"
}

// Generate node creation with properties
std::string ASTGenerator::generateNodeCreation(
    const std::string &nodeId,
    const std::map<std::string, std::string> &properties)
{
    std::stringstream ss;
    ss << "graph.addNode(\"" << nodeId << "\"";
    
    for (const auto &[key, value] : properties) {
      ss << ", " << key << "=\"" << value << "\"";
    }
    ss << ");";
    
  return ss.str();
    // Output: "graph.addNode(\"A\", label=\"Node A\");"
}

// Generate edge creation
std::string ASTGenerator::generateEdgeCreation(
    const std::string &fromNode,
    const std::string &toNode,
    const std::map<std::string, std::string> &properties)
{
 std::stringstream ss;
  ss << "graph.addEdge(\"" << fromNode << "\", \"" << toNode << "\"";
    
    for (const auto &[key, value] : properties) {
        ss << ", " << key << "=" << value;
    }
    ss << ");";
    
    return ss.str();
    // Output: "graph.addEdge(\"A\", \"B\", 5.0);"
}
```

**Parameter Formatting**:
```cpp
std::string ASTGenerator::formatParameter(const std::string &value)
{
  // If value looks like a number, return as-is
    if (std::all_of(value.begin(), value.end(), ::isdigit)) {
  return value;
    }
    
    // Otherwise, treat as string and add quotes
    // Also escape any internal quotes
  std::string escaped = value;
    // ... escape logic ...
    return "\"" + escaped + "\"";
}
```

### 3. CodeFormatter (Code Beautification)
**File**: `src/synthesis/code_generation/code_formatter.h/.cpp`

**Responsibilities**:
- Format generated code using clang-format
- Apply consistent style
- Handle indentation
- Maintain readability

**Key Methods to Implement**:
```cpp
std::string formatCode(const std::string &code);

std::string formatCode(const std::string &code, 
    const std::string &styleFile);

void setStyle(const std::string &styleName);
```

**Implementation Approach**:

```cpp
std::string CodeFormatter::formatCode(const std::string &code)
{
    // Create temporary file with code
    std::string tempFile = "/tmp/code_to_format.cpp";
    std::ofstream tmp(tempFile);
    tmp << code;
    tmp.close();
    
    // Execute clang-format
    std::string command = "clang-format " + tempFile;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return code;// Fallback to unformatted

    // Read formatted output
    std::string result;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
  }
    pclose(pipe);
    
    // Cleanup
    std::remove(tempFile.c_str());
    
    return result;
}

std::string CodeFormatter::formatCode(const std::string &code,
    const std::string &styleFile)
{
  // Similar to above but with style file argument
    std::string command = "clang-format --style=file:" + styleFile;
    // ... execution ...
}
```

**Style Configuration**:
- Support LLVM, Google, Mozilla, WebKit, and Microsoft styles
- Allow custom `.clang-format` files
- Default to LLVM style

### 4. APIMapper (User Data Structure API Tracking)
**File**: `src/synthesis/api_mapping/api_mapper.h/.cpp`

**Responsibilities**:
- Maintain registry of user-defined data structures
- Map generic operations to specific method signatures
- Ensure generated code uses correct API
- Provide fallback for unmapped operations

**Key Methods to Implement**:
```cpp
void registerDataStructure(const std::string &structureName,
    const std::map<std::string, std::string> &methodSignatures);

std::string getMethodSignature(const std::string &structureName,
    const std::string &methodName);

bool hasMethod(const std::string &structureName,
    const std::string &methodName) const;

std::string mapGraphOperation(const std::string &operation,
    const std::map<std::string, std::string> &params);
```

**Implementation**:

```cpp
void APIMapper::registerDataStructure(const std::string &structureName,
    const std::map<std::string, std::string> &methodSignatures)
{
  // Store mapping of structure name to its methods
    // Example:
    // structureName = "AdjacencyListGraph"
    // methodSignatures = {
    //     "add_node": "void add_node(const std::string &id)",
    //   "add_edge": "void add_edge(const std::string &from, ...)"
    // }
    apiRegistry[structureName] = methodSignatures;
}

std::string APIMapper::getMethodSignature(const std::string &structureName,
    const std::string &methodName)
{
    auto it = apiRegistry.find(structureName);
    if (it != apiRegistry.end()) {
   auto methodIt = it->second.find(methodName);
        if (methodIt != it->second.end()) {
      return methodIt->second;
        }
    }
    return "";  // Not found
}

std::string APIMapper::mapGraphOperation(const std::string &operation,
    const std::map<std::string, std::string> &params)
{
    // Map high-level operation to specific API call
    // operation = "add_node", params = { "id": "A" }
    
    if (operation == "add_node") {
        return generateFunctionCall(graphObjectName, "add_node", params);
    }
    if (operation == "add_edge") {
        return generateFunctionCall(graphObjectName, "add_edge", params);
    }
    // ... more operations ...
    
    return "";  // Unknown operation
}
```

**Registration Flow**:

```cpp
// Step 1: C2V engine identifies user structure
DataStructureInfo info = extractor.extractStructure("MyGraph");

// Step 2: Build method signature map
std::map<std::string, std::string> methods;
for (const auto &func : info.memberFunctions) {
    methods[func] = getSignatureFromAST(func);
}

// Step 3: Register with V2C APIMapper
apiMapper.registerDataStructure("MyGraph", methods);

// Step 4: When generating code, query mapper
std::string sig = apiMapper.getMethodSignature("MyGraph", "insert");
// Returns exact signature used in user's class
```

## V2C Pipeline Overview

```
User Interaction
 (e.g., drag node, add edge)
    ?
    ?
????????????????????????
?  UI Layer Captures  ?  User clicks "Add Node A"
?     Interaction      ?
????????????????????????
    ?
  ?
????????????????????????
?  1. API Mapper       ?  Look up correct method:
?   ?  graph.addNode(const string &)
????????????????????????
    ?
    ?
????????????????????????
? 2. AST Generator  ?  Generate code:
?                   ?  graph.addNode("A");
????????????????????????
    ?
    ?
????????????????????????
? 3. Code Formatter  ?  Apply formatting:
?    ?  graph.addNode("A");
????????????????????????
    ?
    ?
????????????????????????
? 4. Clang Rewriter ?  Insert into source:
?      ?  - Find insertion point
?         ?  - Use Clang Rewriter API
?     ?  - Write modified file
????????????????????????
    ?
    ?
Modified Source Code
 (with new operation)
  ?
    ?
???????????????????????????????
? C2V Pipeline Re-executes    ?
? (parse ? analyze ? execute) ?
???????????????????????????????
    ?
    ?
Updated Visualization
```

## Integration with C2V and UI Teams

### From C2V Team
```cpp
// C2V provides extracted structure information
DataStructureInfo info = extractor.extractStructure("MyGraph");

// V2C uses this to build API mapper
apiMapper.registerDataStructure(info.className, extractMethodSignatures(info));
```

### From UI Team
```cpp
// UI signals user interaction
signals:
    void nodeAdded(const QString &nodeId);
    void edgeAdded(const QString &from, const QString &to);

// V2C handles these signals
void onNodeAdded(const QString &nodeId) {
    std::string code = astGenerator.generateNodeCreation(nodeId.toStdString());
    clangRewriter.insertCode(code, lineNum, colNum);
}
```

## Integration Checklist

- [ ] Clang Rewriter can open and modify source files
- [ ] ASTGenerator creates valid C++ syntax
- [ ] Code formatting produces readable output
- [ ] APIMapper correctly stores and retrieves method signatures
- [ ] Generated code uses user-defined API (not generic)
- [ ] Rewritten code maintains proper indentation
- [ ] Source file can be re-parsed after modification
- [ ] Bidirectional mapping working (visual change ? code change)

## Build Instructions

### Dependencies
```bash
# Ensure LLVM/Clang development libraries installed
# (Same as C2V team requirements)
```

### Build
```bash
cd build
cmake -DLLVM_DIR=/path/to/llvm/cmake ..
cmake --build . --config Release
```

## Testing

### Test Code Generation
```cpp
ASTGenerator gen;
std::string code = gen.generateFunctionCall("graph", "addNode", 
    {{"id", "\"A\""}});
assert(code == "graph.addNode(\"A\");");
```

### Test API Mapping
```cpp
APIMapper mapper;
mapper.registerDataStructure("MyGraph", 
    {{"insert", "void insert(Node n)"}});
assert(mapper.hasMethod("MyGraph", "insert"));
```

### Test Code Rewriting
```cpp
ClangRewriter rewriter;
assert(rewriter.openFile("source.cpp"));
assert(rewriter.insertCode("int x = 5;", 10, 0));
assert(rewriter.writeModifiedCode("output.cpp"));
```

## Useful Resources

- Clang Rewriter: https://clang.llvm.org/docs/LibTooling.html#buildingyourownclang
- Clang AST: https://clang.llvm.org/docs/IntroductionToTheClangAST.html
- Clang-Format: https://clang.llvm.org/docs/ClangFormat/
- LLVM Documentation: https://llvm.org/docs/

## Common Issues

**Issue**: Rewriter modifies wrong location
- Verify SourceLocation calculation
- Use SourceManager to debug locations
- Check file IDs match

**Issue**: Generated code has syntax errors
- Validate parameter escaping
- Check template specializations
- Test with simple cases first

**Issue**: Method signature not found
- Verify API was registered correctly
- Check method name matches exactly
- Use C2V team's extraction output

## Next Steps

1. **Implement ClangRewriter**: Get basic source modification working
2. **Implement ASTGenerator**: Generate syntactically correct code
3. **Implement CodeFormatter**: Apply clang-format styling
4. **Implement APIMapper**: Build user structure API registry
5. **Integration Testing**: Test complete V2C pipeline
6. **Bidirectional Testing**: Verify visual changes sync with code

## Contact

Questions about V2C implementation? Refer to `README.md` or check team communications.
