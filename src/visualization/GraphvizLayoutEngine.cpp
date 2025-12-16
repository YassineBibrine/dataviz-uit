#include "GraphvizLayoutEngine.h"
#include <cmath>
#include <regex>
#include <sstream>
#include <vector>

#if defined(__has_include)
#    if __has_include(<gvc.h>) && __has_include(<cgraph.h>)
#        define DATAVIZ_HAS_GRAPHVIZ_HEADERS 1
#    else
#        define DATAVIZ_HAS_GRAPHVIZ_HEADERS 0
#    endif
#else
#    define DATAVIZ_HAS_GRAPHVIZ_HEADERS 1
#endif

#if defined(DATAVIZ_HAS_GRAPHVIZ) && DATAVIZ_HAS_GRAPHVIZ_HEADERS
#    include <gvc.h>
#    include <cgraph.h>
#elif defined(DATAVIZ_HAS_GRAPHVIZ)
#    undef DATAVIZ_HAS_GRAPHVIZ
#endif

GraphvizLayoutEngine::GraphvizLayoutEngine()
    : layoutAlgorithm("dot"), graphvizAvailable(false), gvc(nullptr) {
#ifdef DATAVIZ_HAS_GRAPHVIZ
    try {
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
#ifdef DATAVIZ_HAS_GRAPHVIZ
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

#ifdef DATAVIZ_HAS_GRAPHVIZ
    try {
        GVC_t* context = static_cast<GVC_t*>(gvc);

        Agraph_t* g = agmemread(dotString.c_str());
        if (!g) {
            return computeFallbackLayout(dotString);
        }

        if (gvLayout(context, g, layoutAlgorithm.c_str()) != 0) {
            agclose(g);
            return computeFallbackLayout(dotString);
        }

        std::map<std::string, std::pair<double, double>> positions;
        for (Agnode_t* n = agfstnode(g); n; n = agnxtnode(g, n)) {
            char* pos = agget(n, (char*)"pos");
            if (pos) {
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
                        // Parsing failed; keep fallback values for this node.
                    }
                }
            }
        }

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
    static const std::vector<std::string> validAlgorithms = {
        "dot", "neato", "fdp", "circo", "twopi"
    };

    for (const auto& valid : validAlgorithms) {
        if (algorithm == valid) {
            layoutAlgorithm = algorithm;
            return;
        }
    }

    layoutAlgorithm = "dot";
}

std::map<std::string, std::pair<double, double>> GraphvizLayoutEngine::computeFallbackLayout(
    const std::string& dotString) {
    std::map<std::string, std::pair<double, double>> positions;

    std::regex nodeRegex(R"(\s*(\w+)\s*\[)");
    std::sregex_iterator iter(dotString.begin(), dotString.end(), nodeRegex);
    std::sregex_iterator end;

    std::vector<std::string> nodeNames;
    while (iter != end) {
        nodeNames.push_back(iter->str(1));
        ++iter;
    }

    if (nodeNames.empty()) {
        return positions;
    }

    constexpr double kPi = 3.14159265358979323846;
    const double radius = 300.0;
    const double centerX = 400.0;
    const double centerY = 300.0;
    const double angleStep = 2.0 * kPi / nodeNames.size();

    for (size_t i = 0; i < nodeNames.size(); ++i) {
        double angle = i * angleStep;
        double x = centerX + radius * std::cos(angle);
        double y = centerY + radius * std::sin(angle);
        positions[nodeNames[i]] = {x, y};
    }

    return positions;
}
