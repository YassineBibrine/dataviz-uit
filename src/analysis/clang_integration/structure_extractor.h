#ifndef STRUCTURE_EXTRACTOR_H
#define STRUCTURE_EXTRACTOR_H

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace clang {
class ASTContext;
}

struct DataStructureInfo {
    std::string className;
    std::vector<std::string> memberVariables;
    std::vector<std::string> memberFunctions;
    bool isSelfReferential = false; // For linked structures
  std::string pointerMember;      // Name of pointer member
};

/**
 * @class StructureExtractor
 * @brief Extracts data structure definitions from AST
 * 
 * Uses LibASTMatchers to identify:
 * - Custom class/struct definitions
 * - Linked structures (linked lists, trees, graphs)
 * - Member variables and their types
 * - Relevant member functions
 */
class StructureExtractor {
public:
    explicit StructureExtractor(clang::ASTContext *context);
    
    std::vector<DataStructureInfo> extractStructures();
    DataStructureInfo extractStructure(const std::string &className);

private:
    clang::ASTContext *astContext;
    std::vector<DataStructureInfo> cachedStructures;
    
    bool isLinkedStructure(const DataStructureInfo &info);
};

#endif // STRUCTURE_EXTRACTOR_H
