#ifndef API_MAPPER_H
#define API_MAPPER_H

#include <string>
#include <map>
#include <memory>

/**
 * @class APIMapper
 * @brief Maps user-defined data structure APIs for code generation
 * 
 * Maintains a catalog of:
 * - User-defined class methods and their signatures
 * - Graph/tree/list manipulation functions
 * - Parameter types and names
 * - Return types and semantics
 * 
 * Ensures generated code uses correct API signatures
 */
class APIMapper {
public:
    APIMapper();
    
 void registerDataStructure(const std::string &structureName,
 const std::map<std::string, std::string> &methodSignatures);
    
    std::string getMethodSignature(const std::string &structureName,
        const std::string &methodName);
    
    bool hasMethod(const std::string &structureName,
        const std::string &methodName) const;
  
    std::string mapGraphOperation(const std::string &operation,
   const std::map<std::string, std::string> &params);

private:
    std::map<std::string, std::map<std::string, std::string>> apiRegistry;
};

#endif // API_MAPPER_H
