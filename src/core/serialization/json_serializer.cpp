#include "json_serializer.h"
#include <sstream>

JSONSerializer::JSONSerializer() = default;

std::string JSONSerializer::serializeExecutionLog(const std::string &logData)
{
    // TODO: Implement JSON serialization of execution logs
    // Format: { "events": [ { "line": 10, "operation": "assign", ... } ] }
    return "";
}

std::string JSONSerializer::serializeVisualizationState(
  const std::map<std::string, std::string> &state)
{
    // TODO: Implement JSON serialization of visualization state
 std::stringstream ss;
    ss << "{\n";
    bool first = true;
    for (const auto &[key, value] : state) {
      if (!first) ss << ",\n";
        ss << "  \"" << escapeJSONString(key) << "\": \""
    << escapeJSONString(value) << "\"";
        first = false;
    }
  ss << "\n}";
    return ss.str();
}

std::string JSONSerializer::serializeMetrics(
    const std::map<std::string, std::string> &metrics)
{
    return serializeVisualizationState(metrics);
}

std::string JSONSerializer::escapeJSONString(const std::string &str)
{
    // TODO: Implement proper JSON string escaping
    return str;
}
