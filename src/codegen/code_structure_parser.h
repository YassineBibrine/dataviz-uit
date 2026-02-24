#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

/**
 * @struct ParsedStructure
 * @brief Represents a parsed data-structure extracted from C++ source code.
 *
 * The parser attempts to detect common patterns used to construct Arrays,
 * Linked Lists, Binary Trees and Graphs and returns a normalized representation
 * suitable for creating runtime structures in the application.
 */
struct ParsedStructure {
    enum class Type {
        ARRAY,
        LINKED_LIST,
        BINARY_TREE,
        GRAPH,
        UNKNOWN
    };
    
    Type type = Type::UNKNOWN;
    std::vector<int> values;  // For arrays and simple lists
  std::map<std::string, int> nodeValues;  // For graphs/trees with explicit node IDs
    std::vector<std::pair<std::string, std::string>> edges;  // For graphs/trees
    bool isDirected = false;  // For graphs
    std::string errorMessage;
 bool success = false;
};

/**
 * @class CodeStructureParser
 * @brief Lightweight parser that extracts structure information from C++ code.
 *
 * This parser is intentionally simple and pattern-based (regex). It supports
 * common idioms such as initializer lists for arrays, `new Node(value)` for
 * linked lists, manual `TreeNode* nodeX = new TreeNode(value)` constructions
 * for trees and `addNode`/`addEdge` calls for graphs. It is not a full C++ parser
 * but provides useful heuristics for the code generator dialog.
 */
class CodeStructureParser {
public:
    /**
     * @brief Parse C++ code and extract structure information
     * @param code The C++ source code
* @return Parsed structure information
     */
    static ParsedStructure parse(const std::string& code);
    
private:
 // Detection methods
    static ParsedStructure::Type detectStructureType(const std::string& code);
    
    // Parsing methods for each type
    static ParsedStructure parseArray(const std::string& code);
    static ParsedStructure parseLinkedList(const std::string& code);
    static ParsedStructure parseBinaryTree(const std::string& code);
    static ParsedStructure parseGraph(const std::string& code);
    
    // Helper methods
    static std::vector<int> extractArrayInitializer(const std::string& code);
    static std::vector<int> extractNodeCreations(const std::string& code, const std::string& pattern);
    static std::vector<std::pair<std::string, std::string>> extractEdges(const std::string& code);
    static bool containsKeyword(const std::string& code, const std::string& keyword);
  static std::string trim(const std::string& str);
    static std::vector<std::string> tokenize(const std::string& str, char delimiter);
};
