#ifndef CODE_FORMATTER_H
#define CODE_FORMATTER_H

#include <string>

/**
 * @class CodeFormatter
 * @brief Formats generated code using clang-format style guidelines
 * 
 * Ensures generated code:
 * - Follows consistent indentation
 * - Maintains proper spacing
 * - Aligns with project coding standards
 * - Is readable and professional
 */
class CodeFormatter {
public:
    CodeFormatter();
    
    std::string formatCode(const std::string &code);
    std::string formatCode(const std::string &code, const std::string &styleFile);
    void setStyle(const std::string &styleName);
    
private:
    std::string currentStyle;
};

#endif // CODE_FORMATTER_H
