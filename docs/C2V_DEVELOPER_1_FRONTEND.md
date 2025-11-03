# C2V Developer 1: Frontend & AST Parsing
**Focus Area**: Code Analysis Foundation  
**Timeline**: Weeks 1-3

## Your Role

You are responsible for **parsing C++ source code and extracting data structure information**. Your work feeds into Developer 2's instrumentation and state mapping. Think of you as building the "understanding" layer - analyzing what code exists and what data structures are being used.

---

## Deliverables Overview

### Phase 1: AST Parser (Week 1)
- ? Parse C++ files using Clang
- ? Load compilation databases
- ? Expose AST context to rest of pipeline

### Phase 2: Structure Extraction (Week 2-3)
- ? Identify all data structures in code
- ? Detect linked structures (lists, trees, graphs)
- ? Extract method signatures and properties
- ? Build comprehensive structure inventory

---

## Detailed Tasks

### Task 1.1: Set Up Clang Integration

**File**: `src/analysis/clang_integration/ast_parser.h`

**Create the header with these signatures**:
```cpp
#pragma once

#include "clang/Frontend/CompilerInstance.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <string>
#include <vector>

class ASTParser {
public:
    ASTParser() = default;
    ~ASTParser() = default;

    /// Parse a single C++ file
    /// @param filePath Path to source file
    /// @param compilationFlags Optional compiler flags (e.g., "-std=c++17")
    /// @return true if parsing succeeded
bool parseFile(const std::string &filePath,
    const std::vector<std::string> &compilationFlags = {});

    /// Parse using compilation database (compile_commands.json)
    /// @param databasePath Path to compile_commands.json directory
    /// @return true if database loaded and parsed
    bool parseCompilationDatabase(const std::string &databasePath);

    /// Get the AST context for structure extraction
  /// @return Pointer to ASTContext (valid until parser destroyed)
    clang::ASTContext *getASTContext();

private:
    std::unique_ptr<clang::CompilerInstance> compiler_;
};
```

**File**: `src/analysis/clang_integration/ast_parser.cpp`

**Implement these methods**:

```cpp
#include "ast_parser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Frontend/FrontendActions.h"

bool ASTParser::parseFile(const std::string &filePath,
              const std::vector<std::string> &compilationFlags)
{
    // Step 1: Create compiler instance
    compiler_ = std::make_unique<clang::CompilerInstance>();
    
    // Step 2: Set up diagnostics
    compiler_->createDiagnostics();
    
    // Step 3: Create file manager
    compiler_->createFileManager();
    
    // Step 4: Create source manager
    compiler_->createSourceManager();
  
    // Step 5: Configure language options
    auto &langOpts = compiler_->getLangOpts();
    langOpts.CPlusPlus = 1;
    langOpts.CPlusPlus17 = 1;
    
    // Step 6: Create target info
    auto targetInfo = clang::TargetInfo::CreateTargetInfo(
        compiler_->getDiagnostics(),
        std::make_shared<clang::TargetOptions>());
    
    if (!targetInfo) {
        llvm::errs() << "Failed to create target info\n";
        return false;
    }
    compiler_->setTarget(targetInfo.get());
    
    // Step 7: Parse the file
    try {
        const clang::FileEntry *pFile = 
            compiler_->getFileManager().getFile(filePath);
        
        if (!pFile) {
     llvm::errs() << "File not found: " << filePath << "\n";
            return false;
 }
   
      auto fileID = compiler_->getSourceManager().createFileID(
         pFile, clang::SourceLocation(), clang::SrcMgr::C_User);
     
        compiler_->getSourceManager().setMainFileID(fileID);
        compiler_->createASTContext();
      
        // Create AST consumer and parse
        clang::SyntaxOnlyAction action;
        if (!action.BeginSourceFile(*compiler_, pFile)) {
            llvm::errs() << "Failed to begin source file\n";
            return false;
    }
        
        if (!action.Execute()) {
          llvm::errs() << "Execution failed\n";
   return false;
 }
        
        action.EndSourceFile();
   return true;
        
    } catch (const std::exception &e) {
        llvm::errs() << "Exception during parsing: " << e.what() << "\n";
        return false;
    }
}

bool ASTParser::parseCompilationDatabase(const std::string &databasePath)
{
    // This optional method loads compile_commands.json
    // Advanced implementation - start with parseFile if time is limited
    
    std::string errMsg;
    auto db = clang::tooling::CompilationDatabase::loadFromDirectory(
        databasePath, errMsg);
    
    if (!db) {
        llvm::errs() << "Failed to load compilation database: " << errMsg << "\n";
        return false;
    }
    
    // Process first compilation command if available
    auto commands = db->getAllCompileCommands();
    if (commands.empty()) {
   llvm::errs() << "No compilation commands found\n";
        return false;
    }

    // Use first command - in production, iterate all
    return parseFile(commands[0].Filename, commands[0].CommandLine);
}

clang::ASTContext *ASTParser::getASTContext()
{
    if (compiler_) {
        return &compiler_->getASTContext();
    }
    return nullptr;
}
```

---

### Task 1.2: Implement Structure Extraction

**File**: `src/analysis/clang_integration/structure_extractor.h`

```cpp
#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <string>
#include <vector>
#include <map>

/// Information about an extracted data structure
struct DataStructureInfo {
    std::string className;
    std::vector<std::string> memberVariables;    // e.g., {"head", "tail"}
    std::vector<std::string> memberFunctions;    // e.g., {"insert", "remove"}
    bool isSelfReferential = false;  // true for linked structures
    std::string pointerMember;        // name of self-referential pointer
};

class StructureExtractor : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    explicit StructureExtractor(clang::ASTContext *context);
    
    /// Extract all data structures from the AST
    std::vector<DataStructureInfo> extractStructures();
    
    /// Extract a specific structure by name
    DataStructureInfo extractStructure(const std::string &className);
    
    /// Check if a structure is linked (self-referential)
    static bool isLinkedStructure(const DataStructureInfo &info);
    
    /// Callback for AST matcher results
    void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
    
private:
    clang::ASTContext *context_;
  std::vector<DataStructureInfo> structures_;
    
    /// Helper to extract member variables from a CXX record
    std::vector<std::string> extractMembers(const clang::CXXRecordDecl *decl);
    
    /// Helper to extract member functions
    std::vector<std::string> extractMethods(const clang::CXXRecordDecl *decl);
    
    /// Helper to detect self-referential pointers
    std::string findSelfRefPointer(const clang::CXXRecordDecl *decl);
};
```

**File**: `src/analysis/clang_integration/structure_extractor.cpp`

```cpp
#include "structure_extractor.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"

using namespace clang;
using namespace clang::ast_matchers;

StructureExtractor::StructureExtractor(ASTContext *context)
    : context_(context) {}

std::vector<DataStructureInfo> StructureExtractor::extractStructures()
{
    structures_.clear();
    
    MatchFinder finder;
    
    // Match all C++ record declarations (classes and structs)
    auto recordMatcher = cxxRecordDecl(isDefinition()).bind("record");
    
    finder.addMatcher(recordMatcher, this);
    finder.matchAST(*context_);
    
    return structures_;
}

DataStructureInfo StructureExtractor::extractStructure(const std::string &className)
{
  auto structures = extractStructures();
    for (const auto &info : structures) {
      if (info.className == className) {
            return info;
}
    }
    return {};  // Return empty if not found
}

void StructureExtractor::run(const MatchFinder::MatchResult &result)
{
    const auto *recordDecl = result.Nodes.getNodeAs<CXXRecordDecl>("record");
    if (!recordDecl) return;
    
    // Skip STL classes and built-in types
    auto name = recordDecl->getName();
    if (name.empty() || name.starts_with("std::")) {
        return;
    }
    
    DataStructureInfo info;
    info.className = name.str();
    info.memberVariables = extractMembers(recordDecl);
    info.memberFunctions = extractMethods(recordDecl);
    info.pointerMember = findSelfRefPointer(recordDecl);
    info.isSelfReferential = !info.pointerMember.empty();
    
    structures_.push_back(info);
}

std::vector<std::string> StructureExtractor::extractMembers(const CXXRecordDecl *decl)
{
    std::vector<std::string> members;
    
    for (const auto *field : decl->fields()) {
        members.push_back(field->getName().str());
    }
    
    return members;
}

std::vector<std::string> StructureExtractor::extractMethods(const CXXRecordDecl *decl)
{
    std::vector<std::string> methods;
    
    for (const auto *method : decl->methods()) {
        // Skip constructors, destructors, operators
        if (isa<CXXConstructorDecl>(method) || 
     isa<CXXDestructorDecl>(method)) {
         continue;
        }
        
        methods.push_back(method->getName().str());
    }
  
    return methods;
}

std::string StructureExtractor::findSelfRefPointer(const CXXRecordDecl *decl)
{
    // Look for member pointers that point back to same type
    for (const auto *field : decl->fields()) {
        auto fieldType = field->getType();
        
        // Check if it's a pointer type
     if (const auto *ptrType = fieldType->getAs<PointerType>()) {
     // Check if it points to the same record
 if (const auto *pointedRecord = 
      ptrType->getPointeeType()->getAsCXXRecordDecl()) {
      
      if (pointedRecord == decl || 
      pointedRecord->getName() == decl->getName()) {
           return field->getName().str();
            }
   }
     }
    }
    
    return "";
}

bool StructureExtractor::isLinkedStructure(const DataStructureInfo &info)
{
    return info.isSelfReferential;
}
```

---

### Task 1.3: Create CMake Configuration

**File**: `src/analysis/clang_integration/CMakeLists.txt`

```cmake
# Clang/LLVM integration library

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Add library
add_library(clang_integration
    ast_parser.cpp
    structure_extractor.cpp
)

# Link Clang/LLVM
target_link_libraries(clang_integration
    PUBLIC
     clang
        clang-cpp
        LLVM
)

# Include directories
target_include_directories(clang_integration
    PUBLIC
        ${LLVM_INCLUDE_DIRS}
     ${CLANG_INCLUDE_DIRS}
${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Enable C++17
set_property(TARGET clang_integration PROPERTY CXX_STANDARD 17)
```

---

### Task 1.4: Write Unit Tests

**File**: `tests/test_structure_extractor.cpp`

```cpp
#include <gtest/gtest.h>
#include "analysis/clang_integration/ast_parser.h"
#include "analysis/clang_integration/structure_extractor.h"
#include <fstream>
#include <sstream>

class StructureExtractorTest : public ::testing::Test {
protected:
    void writeTestFile(const std::string &code) {
  std::ofstream file("test_input.cpp");
  file << code;
        file.close();
    }
};

TEST_F(StructureExtractorTest, ExtractsSimpleClass) {
    writeTestFile(R"(
        class MyGraph {
        public:
     void addNode(const std::string &id);
      void addEdge(const std::string &from, const std::string &to);
        private:
     int nodeCount;
        };
    )");
    
    ASTParser parser;
    ASSERT_TRUE(parser.parseFile("test_input.cpp"));
    
    StructureExtractor extractor(parser.getASTContext());
    auto structures = extractor.extractStructures();
    
    EXPECT_EQ(structures.size(), 1);
    EXPECT_EQ(structures[0].className, "MyGraph");
    EXPECT_TRUE(std::find(structures[0].memberFunctions.begin(),
   structures[0].memberFunctions.end(),
        "addNode") != structures[0].memberFunctions.end());
}

TEST_F(StructureExtractorTest, DetectsLinkedStructures) {
    writeTestFile(R"(
      class LinkedList {
        private:
            struct Node {
                int data;
      Node *next;
    };
  Node *head;
        };
    )");
    
    ASTParser parser;
ASSERT_TRUE(parser.parseFile("test_input.cpp"));
    
    StructureExtractor extractor(parser.getASTContext());
    auto structures = extractor.extractStructures();
    
    // Should find LinkedList and Node
    EXPECT_GE(structures.size(), 1);
    
    // Node should be detected as linked (has self-referential next pointer)
 for (const auto &info : structures) {
        if (info.className == "Node") {
      EXPECT_TRUE(info.isSelfReferential);
            EXPECT_EQ(info.pointerMember, "next");
        }
    }
}
```

---

## Testing Checklist

Before moving to Developer 2:

- [ ] `ast_parser.parseFile()` works with valid C++ file
- [ ] `ast_parser.getASTContext()` returns non-null pointer
- [ ] `extractStructures()` finds classes and structs in code
- [ ] `isSelfReferential` correctly identifies linked structures
- [ ] Member functions and variables correctly extracted
- [ ] Unit tests pass (see Task 1.4)
- [ ] No memory leaks in AST parser

---

## Handoff to Developer 2

**What Developer 2 needs from you**:
1. Working `ASTParser` that parses C++ files
2. Working `StructureExtractor` that outputs `DataStructureInfo`
3. Comprehensive list of all data structures in test files
4. Documentation of `DataStructureInfo` structure

**Provide these artifacts**:
- ? Compiled `clang_integration` library
- ? Example `DataStructureInfo` outputs for test cases
- ? README explaining how to use `ASTParser` and `StructureExtractor`

---

## Key Resources

- **Clang AST**: https://clang.llvm.org/docs/IntroductionToTheClangAST.html
- **LibASTMatchers**: https://clang.llvm.org/docs/LibASTMatchers.html
- **Clang Tooling**: https://clang.llvm.org/docs/LibTooling.html

---

## Debugging Tips

**Problem**: AST Context is null
- ? Verify C++ file has valid syntax
- ? Check compilation flags match file's requirements
- ? Try simpler test files first

**Problem**: Structure not found
- ? Add debug output to `run()` callback
- ? Verify AST matcher expression is correct
- ? Check class isn't being filtered out

**Problem**: Build fails with Clang linking errors
- ? Verify LLVM_DIR points to valid LLVM installation
- ? Check LLVM and Clang versions match
- ? See [CMake configuration guide](https://clang.llvm.org/docs/CMakeGuide.html)

---

## Timeline

| Week | Task | Status |
|------|------|--------|
| Week 1 | Task 1.1: AST Parser | ? |
| Week 1-2 | Task 1.2: Structure Extraction | ? |
| Week 2 | Task 1.3: CMake Setup | ? |
| Week 2-3 | Task 1.4: Testing & Documentation | ? |
| Week 3 | Handoff to Developer 2 | ? |

---

**Questions?** Contact your team lead or refer to the main C2V_GUIDE.md.
