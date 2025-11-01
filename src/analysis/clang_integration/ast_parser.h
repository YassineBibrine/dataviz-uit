#ifndef AST_PARSER_H
#define AST_PARSER_H

#include <string>
#include <vector>
#include <memory>

namespace clang {
class ASTContext;
class CompilationDatabase;
}

/**
 * @class ASTParser
 * @brief Parses C++ source files using Clang/LLVM
 * 
 * Responsible for:
 * - Loading compilation database
 * - Parsing C++ source files
 * - Generating Abstract Syntax Tree (AST)
 * - Providing AST context for analysis
 */
class ASTParser {
public:
    ASTParser();
    ~ASTParser();
    
  bool parseFile(const std::string &filePath,
  const std::vector<std::string> &compilationFlags = {});
    bool parseCompilationDatabase(const std::string &databasePath);
    
    clang::ASTContext *getASTContext();
    const std::string &getLastError() const;
    
private:
    std::unique_ptr<clang::CompilationDatabase> compilationDatabase;
    std::string lastError;
    // TODO: Store parsed AST information
};

#endif // AST_PARSER_H
