#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <string>
#include <map>

/**
 * @class JSONSerializer
 * @brief Serializes execution logs and visualization states to JSON
 * 
 * Converts:
 * - Execution events to JSON format
 * - Visualization states to JSON
 * - Metric data to JSON
 */
class JSONSerializer {
public:
  JSONSerializer();
    
    std::string serializeExecutionLog(const std::string &logData);
    std::string serializeVisualizationState(
  const std::map<std::string, std::string> &state);
 std::string serializeMetrics(
   const std::map<std::string, std::string> &metrics);

private:
    std::string escapeJSONString(const std::string &str);
};

#endif // JSON_SERIALIZER_H
