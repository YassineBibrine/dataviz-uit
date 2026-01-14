#include "code_structure_parser.h"
#include <regex>
#include <algorithm>
#include <sstream>
#include <cctype>

ParsedStructure CodeStructureParser::parse(const std::string& code) {
    ParsedStructure result;
    
    if (code.empty()) {
        result.errorMessage = "Empty code provided";
 return result;
    }
    
    // Detect structure type
    result.type = detectStructureType(code);
    
    // Parse based on detected type
    switch (result.type) {
        case ParsedStructure::Type::ARRAY:
        result = parseArray(code);
            break;
        case ParsedStructure::Type::LINKED_LIST:
   result = parseLinkedList(code);
       break;
 case ParsedStructure::Type::BINARY_TREE:
         result = parseBinaryTree(code);
break;
        case ParsedStructure::Type::GRAPH:
            result = parseGraph(code);
         break;
        default:
         result.errorMessage = "Could not detect structure type. Supported types: Array, LinkedList, BinaryTree, Graph";
            break;
    }
    
    return result;
}

ParsedStructure::Type CodeStructureParser::detectStructureType(const std::string& code) {
    // Check for graph (most specific)
    if (containsKeyword(code, "addEdge") || containsKeyword(code, "adjacency")) {
        return ParsedStructure::Type::GRAPH;
    }
    
    // Check for tree
    if ((containsKeyword(code, "TreeNode") || containsKeyword(code, "BinaryTree")) &&
        (containsKeyword(code, "left") && containsKeyword(code, "right"))) {
        return ParsedStructure::Type::BINARY_TREE;
    }
    
    // Check for linked list
    if ((containsKeyword(code, "Node") && containsKeyword(code, "next")) ||
        containsKeyword(code, "LinkedList")) {
    return ParsedStructure::Type::LINKED_LIST;
    }
    
    // Check for array/vector
    if (containsKeyword(code, "vector<int>") || containsKeyword(code, "int arr[") ||
        containsKeyword(code, "std::vector")) {
      return ParsedStructure::Type::ARRAY;
    }
    
    return ParsedStructure::Type::UNKNOWN;
}

ParsedStructure CodeStructureParser::parseArray(const std::string& code) {
    ParsedStructure result;
    result.type = ParsedStructure::Type::ARRAY;
    
    // Extract array initializer: { 1, 2, 3, ... }
    result.values = extractArrayInitializer(code);
    
    if (result.values.empty()) {
        result.errorMessage = "No array values found. Expected format: vector<int> arr = {1, 2, 3};";
        result.success = false;
    } else {
        result.success = true;
    }
    
    return result;
}

ParsedStructure CodeStructureParser::parseLinkedList(const std::string& code) {
    ParsedStructure result;
    result.type = ParsedStructure::Type::LINKED_LIST;
    
    // Pattern: new Node(value)
    result.values = extractNodeCreations(code, R"(new\s+Node\s*\(\s*(\d+)\s*\))");

    if (result.values.empty()) {
        result.errorMessage = "No linked list nodes found. Expected: new Node(value);";
        result.success = false;
    } else {
        result.success = true;
    }
  
    return result;
}

ParsedStructure CodeStructureParser::parseBinaryTree(const std::string& code) {
    ParsedStructure result;
    result.type = ParsedStructure::Type::BINARY_TREE;
    
    // Try to parse manual construction first (new format)
    // Pattern: TreeNode* nodeX = new TreeNode(value);
    std::vector<int> manualValues = extractNodeCreations(code, R"(TreeNode\s*\*\s*node\d+\s*=\s*new\s+TreeNode\s*\(\s*(\d+)\s*\))");
    
    if (!manualValues.empty()) {
  // Manual construction detected
    result.values = manualValues;
        
        // Try to extract edge connections: nodeX->left = nodeY; or nodeX->right = nodeY;
        std::regex leftEdgeRegex(R"(node(\d+)\s*->\s*left\s*=\s*node(\d+))");
     std::regex rightEdgeRegex(R"(node(\d+)\s*->\s*right\s*=\s*node(\d+))");
        
   std::sregex_iterator leftIter(code.begin(), code.end(), leftEdgeRegex);
  std::sregex_iterator rightIter(code.begin(), code.end(), rightEdgeRegex);
        std::sregex_iterator end;
        
        // Build node ID map for edges
        std::map<std::string, int> nodeIdToValue;
        for (size_t i = 0; i < manualValues.size(); ++i) {
            std::string nodeId = "node" + std::to_string(i);
       nodeIdToValue[nodeId] = manualValues[i];
       result.nodeValues[nodeId] = manualValues[i];
        }
        
        // Extract left edges
     while (leftIter != end) {
   std::string parentId = "node" + (*leftIter)[1].str();
 std::string childId = "node" + (*leftIter)[2].str();
      result.edges.emplace_back(parentId, childId);
   ++leftIter;
  }
  
 // Extract right edges
   while (rightIter != end) {
  std::string parentId = "node" + (*rightIter)[1].str();
            std::string childId = "node" + (*rightIter)[2].str();
       result.edges.emplace_back(parentId, childId);
     ++rightIter;
        }
  
        result.success = true;
        return result;
    }
    
    // Fall back to old formats (BST insertion or simple new TreeNode)
    // Pattern 1: insert(root, value)
    std::vector<int> insertValues = extractNodeCreations(code, R"(insert\s*\(\s*\w+\s*,\s*(\d+)\s*\))");
    
    // Pattern 2: new TreeNode(value) without explicit node variable
    if (insertValues.empty()) {
   insertValues = extractNodeCreations(code, R"(new\s+TreeNode\s*\(\s*(\d+)\s*\))");
}
    
    result.values = insertValues;
    
    if (result.values.empty()) {
     result.errorMessage = "No tree nodes found. Expected formats:\n"
      "  - Manual: TreeNode* node0 = new TreeNode(value); node0->left = node1;\n"
  "  - BST Insert: insert(root, value);\n"
     "  - Simple: new TreeNode(value);";
        result.success = false;
    } else {
      result.success = true;
    }
    
    return result;
}

ParsedStructure CodeStructureParser::parseGraph(const std::string& code) {
    ParsedStructure result;
    result.type = ParsedStructure::Type::GRAPH;
    
    // Check if directed
    result.isDirected = containsKeyword(code, "Graph(true)") || 
            containsKeyword(code, "directed = true");
    
  // Extract nodes with values: addNode("nodeId", value) or addNode("nodeId")
    std::regex nodeWithValueRegex(R"(addNode\s*\(\s*\"([^\"]+)\"\s*,\s*(\d+)\s*\))");
    std::regex nodeOnlyRegex(R"(addNode\s*\(\s*\"([^\"]+)\"\s*\))");
    
    std::sregex_iterator nodeValIter(code.begin(), code.end(), nodeWithValueRegex);
    std::sregex_iterator nodeOnlyIter(code.begin(), code.end(), nodeOnlyRegex);
    std::sregex_iterator end;
    
    // First, try to parse nodes with values
    while (nodeValIter != end) {
        std::string nodeId = (*nodeValIter)[1].str();
        int value = std::stoi((*nodeValIter)[2].str());
        result.nodeValues[nodeId] = value;
        ++nodeValIter;
    }
 
    // Then parse nodes without explicit values (assign sequential index)
    int nodeIndex = static_cast<int>(result.nodeValues.size());
    while (nodeOnlyIter != end) {
        std::string nodeId = (*nodeOnlyIter)[1].str();
  // Only add if not already added with a value
        if (result.nodeValues.find(nodeId) == result.nodeValues.end()) {
        result.nodeValues[nodeId] = nodeIndex++;
    }
        ++nodeOnlyIter;
    }
    
    // Extract edges: addEdge("from", "to")
    result.edges = extractEdges(code);
    
    if (result.nodeValues.empty() && result.edges.empty()) {
      result.errorMessage = "No graph structure found. Expected: addNode(\"id\", value) and addEdge(\"from\", \"to\");";
     result.success = false;
    } else {
      result.success = true;
    }

    return result;
}

std::vector<int> CodeStructureParser::extractArrayInitializer(const std::string& code) {
    std::vector<int> values;
    
    // Pattern: = { number, number, ... }
    std::regex initRegex(R"(=\s*\{\s*([0-9\s,]+)\s*\})");
std::smatch match;
    
    if (std::regex_search(code, match, initRegex)) {
 std::string numberList = match[1].str();
 
     // Extract all numbers
std::regex numRegex(R"(\d+)");
        std::sregex_iterator numIter(numberList.begin(), numberList.end(), numRegex);
      std::sregex_iterator end;
    
   while (numIter != end) {
            values.push_back(std::stoi((*numIter).str()));
            ++numIter;
        }
 }
    
    return values;
}

std::vector<int> CodeStructureParser::extractNodeCreations(const std::string& code, const std::string& pattern) {
    std::vector<int> values;
    
    std::regex regex(pattern);
    std::sregex_iterator iter(code.begin(), code.end(), regex);
    std::sregex_iterator end;
    
    while (iter != end) {
        values.push_back(std::stoi((*iter)[1].str()));
        ++iter;
    }
    
    return values;
}

std::vector<std::pair<std::string, std::string>> CodeStructureParser::extractEdges(const std::string& code) {
    std::vector<std::pair<std::string, std::string>> edges;
    
    // Pattern: addEdge("from", "to") or addEdge('from', 'to')
    std::regex edgeRegex(R"(addEdge\s*\(\s*[\"']([^\"']+)[\"']\s*,\s*[\"']([^\"']+)[\"']\s*\))");
    std::sregex_iterator iter(code.begin(), code.end(), edgeRegex);
    std::sregex_iterator end;
    
    while (iter != end) {
        std::string from = (*iter)[1].str();
        std::string to = (*iter)[2].str();
        edges.emplace_back(from, to);
        ++iter;
    }
    
    return edges;
}

bool CodeStructureParser::containsKeyword(const std::string& code, const std::string& keyword) {
    return code.find(keyword) != std::string::npos;
}

std::string CodeStructureParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::vector<std::string> CodeStructureParser::tokenize(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    
    return tokens;
}
