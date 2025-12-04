#include "GraphvizLayoutEngine.h"
#include <cmath>
#include <regex>
#include <sstream>
#include <vector>

// Graphviz headers - conditionally included
#ifdef _WIN32
    #include <gvc.h>
    #include <cgraph.h>
#endif

GraphvizLayoutEngine::GraphvizLayoutEngine()
    : layoutAlgorithm("dot"), graphvizAvailable(false), gvc(nullptr) {
  
#ifdef _WIN32
    try {
        // Try to initialize Graphviz context
        gvc = gvContext();
        if (gvc) {
            graphvizAvailable = true;
        }
    } catch (...) {
        graphvizAvailable = false;
    }
#endif
}

GraphvizLayoutEngine::~GraphvizLayoutEngine() {
#ifdef _WIN32
    if (gvc) {
  gvFreeContext(static_cast<GVC_t*>(gvc));
      gvc = nullptr;
    }
#endif
}

std::map<std::string, std::pair<double, double>> GraphvizLayoutEngine::computeLayout(
    const std::string& dotString) {
    
 if (!graphvizAvailable) {
        return computeFallbackLayout(dotString);
    }

#ifdef _WIN32
    try {
        GVC_t* context = static_cast<GVC_t*>(gvc);
      
      // Parse DOT string into a graph
        Agraph_t* g = agmemread(dotString.c_str());
        if (!g) {
return computeFallbackLayout(dotString);
   }

 // Set layout algorithm
        if (gvLayout(context, g, layoutAlgorithm.c_str()) != 0) {
            agclose(g);
            return computeFallbackLayout(dotString);
        }

    std::map<std::string, std::pair<double, double>> positions;

        // Extract node positions
        for (Agnode_t* n = agfstnode(g); n; n = agnxtnode(g, n)) {
            char* pos = agget(n, (char*)"pos");
 
            if (pos) {
   // Parse position string (format: "x,y")
         std::string posStr(pos);
                size_t commaPos = posStr.find(',');

             if (commaPos != std::string::npos) {
        try {
    double x = std::stod(posStr.substr(0, commaPos));
         double y = std::stod(posStr.substr(commaPos + 1));
   const char* nodeName = agnameof(n);
             if (nodeName) {
        positions[std::string(nodeName)] = {x, y};
      }
              } catch (...) {
      // If parsing fails, use fallback
}
      }
    }
        }

        // If we got some positions, return them
    if (!positions.empty()) {
     gvFreeLayout(context, g);
       agclose(g);
   return positions;
        }

        gvFreeLayout(context, g);
        agclose(g);
  
    } catch (...) {
    // Fall through to fallback layout
    }
#endif

    return computeFallbackLayout(dotString);
}

bool GraphvizLayoutEngine::isAvailable() const {
    return graphvizAvailable;
}

void GraphvizLayoutEngine::setLayoutAlgorithm(const std::string& algorithm) {
    // Validate algorithm name
    static const std::vector<std::string> validAlgorithms = {
        "dot", "neato", "fdp", "circo", "twopi"
    };
    
    for (const auto& valid : validAlgorithms) {
        if (algorithm == valid) {
     layoutAlgorithm = algorithm;
            return;
        }
    }
    
    // Default to "dot" if invalid
    layoutAlgorithm = "dot";
}

std::map<std::string, std::pair<double, double>> GraphvizLayoutEngine::computeFallbackLayout(
    const std::string& dotString) {
    
    std::map<std::string, std::pair<double, double>> positions;
    
    // Extract node names from DOT string using regex
    std::regex nodeRegex(R"(\s*(\w+)\s*\[)");
  std::sregex_iterator iter(dotString.begin(), dotString.end(), nodeRegex);
    std::sregex_iterator end;
    
    std::vector<std::string> nodeNames;
    while (iter != end) {
    nodeNames.push_back(iter->str(1));
        ++iter;
    }

    // If no nodes found, return empty map
    if (nodeNames.empty()) {
        return positions;
    }

    // Arrange nodes in a circle
    const double radius = 300.0;
    const double centerX = 400.0;
    const double centerY = 300.0;
    const double angleStep = 2.0 * M_PI / nodeNames.size();

    for (size_t i = 0; i < nodeNames.size(); ++i) {
        double angle = i * angleStep;
        double x = centerX + radius * std::cos(angle);
      double y = centerY + radius * std::sin(angle);
    positions[nodeNames[i]] = {x, y};
    }

    return positions;
}
