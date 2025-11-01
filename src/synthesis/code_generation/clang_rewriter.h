#ifndef CLANG_REWRITER_H
#define CLANG_REWRITER_H

#include <string>
#include <vector>
#include <memory>

namespace clang {
class Rewriter;
class ASTContext;
}

/**
 * @class ClangRewriter
 * @brief Rewrites source code using Clang's code rewriting infrastructure
 * 
 * Implements V2C (Visualization-to-Code) by:
 * - Parsing existing source code into AST
 * - Synthesizing new code statements
 * - Inserting synthesized code into source
 * - Maintaining code formatting and style
 */
class ClangRewriter {
public:
    ClangRewriter();
    ~ClangRewriter();
    
    bool openFile(const std::string &sourceFile);
    bool insertCode(const std::string &code, int lineNumber, int columnNumber);
    bool replaceCode(int startLine, int startCol, int endLine, int endCol,
        const std::string &replacement);
    bool writeModifiedCode(const std::string &outputFile);
    
    const std::string &getLastError() const;

private:
 std::unique_ptr<clang::Rewriter> rewriter;
    std::unique_ptr<clang::ASTContext> astContext;
    std::string lastError;
};

#endif // CLANG_REWRITER_H
