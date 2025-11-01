#ifndef AST_GENERATOR_H
#define AST_GENERATOR_H

#include <string>
#include <map>
#include <memory>

/**
 * @class ASTGenerator
 * @brief Generates AST nodes for code synthesis
 * 
 * Creates Abstract Syntax Tree nodes representing:
 * - Function calls (e.g., graph.addNode())
 * - Variable assignments
 * - Control flow structures
 * - Data structure operations
 */
class ASTGenerator {
public:
    ASTGenerator();
    
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

private:
    std::string formatParameter(const std::string &value);
};

#endif // AST_GENERATOR_H
