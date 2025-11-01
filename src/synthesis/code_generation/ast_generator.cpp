#include "ast_generator.h"
#include <sstream>

ASTGenerator::ASTGenerator() = default;

std::string ASTGenerator::generateFunctionCall(const std::string &objectName,
    const std::string &functionName,
    const std::map<std::string, std::string> &parameters)
{
    // TODO: Generate function call in C++ syntax
  // Example: object.function(param1, param2);
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
}

std::string ASTGenerator::generateVariableAssignment(const std::string &varName,
    const std::string &value)
{
    // TODO: Generate variable assignment
    return varName + " = " + value + ";";
}

std::string ASTGenerator::generateNodeCreation(const std::string &nodeId,
    const std::map<std::string, std::string> &properties)
{
    // TODO: Generate node creation code
    std::stringstream ss;
    ss << "// Create node: " << nodeId;
    return ss.str();
}

std::string ASTGenerator::generateEdgeCreation(const std::string &fromNode,
    const std::string &toNode,
    const std::map<std::string, std::string> &properties)
{
    // TODO: Generate edge creation code
    std::stringstream ss;
    ss << "// Create edge: " << fromNode << " -> " << toNode;
    return ss.str();
}

std::string ASTGenerator::formatParameter(const std::string &value)
{
    // TODO: Properly format parameters (strings, numbers, etc.)
    // Add quotes for string literals if needed
    return value;
}
