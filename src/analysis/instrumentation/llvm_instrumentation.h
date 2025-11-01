#ifndef LLVM_INSTRUMENTATION_H
#define LLVM_INSTRUMENTATION_H

#include <string>
#include <vector>

/**
 * @class LLVMInstrumentation
 * @brief LLVM pass for injecting visualization callbacks
 * 
 * Creates LLVM instrumentation passes that:
 * - Insert logging callbacks before/after state-modifying operations
 * - Track variable value changes
 * - Record memory allocation and deallocation
 * - Log pointer assignments and dereferencing
 */
class LLVMInstrumentation {
public:
    LLVMInstrumentation();
    ~LLVMInstrumentation();
    
    bool instrumentCode(const std::string &sourceFile,
        const std::string &outputFile);
    bool createInstrumentationPass();
    
private:
    std::string lastError;
};

#endif // LLVM_INSTRUMENTATION_H
