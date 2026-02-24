#include "GraphvizLayoutEngine.h"
#include <cmath>
#include <regex>
#include <sstream>
#include <vector>

// Graphviz is optional - only include if explicitly enabled via CMake
// CMake sets GRAPHVIZ_AVAILABLE=1 when found, or GRAPHVIZ_AVAILABLE=0 when not found
// If neither is set, assume Graphviz is not available
#if defined(GRAPHVIZ_AVAILABLE) && (GRAPHVIZ_AVAILABLE ==1)
# define USE_GRAPHVIZ1
#else
# define USE_GRAPHVIZ0
#endif

#if USE_GRAPHVIZ
# if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4996)
# endif
# include <graphviz/gvc.h>
# include <graphviz/cgraph.h>
# if defined(_MSC_VER)
# pragma warning(pop)
# endif
#endif

GraphvizLayoutEngine::GraphvizLayoutEngine()
 : layoutAlgorithm("dot"), graphvizAvailable(false), gvc(nullptr) {
#if USE_GRAPHVIZ
 try {
 gvc = gvContext();
 graphvizAvailable = (gvc != nullptr);
 } catch (...) {
 graphvizAvailable = false;
 if (gvc) {
 try { gvFreeContext(static_cast<GVC_t*>(gvc)); } catch (...) {}
 gvc = nullptr;
 }
 }
#endif
}

GraphvizLayoutEngine::~GraphvizLayoutEngine() {
#if USE_GRAPHVIZ
 if (gvc) {
 try { gvFreeContext(static_cast<GVC_t*>(gvc)); } catch (...) {}
 gvc = nullptr;
 }
#endif
}

std::map<std::string, std::pair<double, double>> GraphvizLayoutEngine::computeLayout(
 const std::string& dotString) {
 if (!graphvizAvailable || !gvc) {
 return computeFallbackLayout(dotString);
 }

#if USE_GRAPHVIZ
 try {
 GVC_t* ctx = static_cast<GVC_t*>(gvc);
 Agraph_t* g = agmemread(dotString.c_str());
 if (!g) return computeFallbackLayout(dotString);

 // Attempt layout - if it fails, silently fall back
 // Note: Graphviz may print errors to stderr if plugins are not loaded
 int layoutResult = gvLayout(ctx, g, layoutAlgorithm.c_str());
 if (layoutResult !=0) {
 agclose(g);
 return computeFallbackLayout(dotString);
 }

 std::map<std::string, std::pair<double, double>> positions;
 for (Agnode_t* n = agfstnode(g); n; n = agnxtnode(g, n)) {
 char* pos = agget(n, const_cast<char*>("pos"));
 if (pos) {
 std::string posStr(pos);
 size_t comma = posStr.find(',');
 if (comma != std::string::npos) {
 try {
 double x = std::stod(posStr.substr(0, comma));
 double y = std::stod(posStr.substr(comma +1));
 positions[agnameof(n)] = {x, y};
 } catch (...) {}
 }
 }
 }
 gvFreeLayout(ctx, g);
 agclose(g);
 if (!positions.empty()) return positions;
 else
 return computeFallbackLayout(dotString);
 } catch (...) {
 // Silently fall back to fallback layout on any exception
 }
#endif
 return computeFallbackLayout(dotString);
}

bool GraphvizLayoutEngine::isAvailable() const {
 return graphvizAvailable;
}

void GraphvizLayoutEngine::setLayoutAlgorithm(const std::string& algo) {
 if (algo == "dot" || algo == "neato" || algo == "fdp" || algo == "circo" || algo == "twopi")
 layoutAlgorithm = algo;
 else
 layoutAlgorithm = "dot";
}

std::map<std::string, std::pair<double, double>> GraphvizLayoutEngine::computeFallbackLayout(
 const std::string& dotString) {
 std::map<std::string, std::pair<double, double>> positions;
 std::regex nodeRegex(R"(\s*(\w+)\s*\[)");
 std::sregex_iterator it(dotString.begin(), dotString.end(), nodeRegex);
 std::sregex_iterator end;

 std::vector<std::string> nodes;
 while (it != end) {
 std::string name = it->str(1);
 if (name != "digraph" && name != "graph" && name != "node" &&
 name != "edge" && name != "subgraph" && name != "strict")
 nodes.push_back(name);
 ++it;
 }
 if (nodes.empty()) return positions;

 const double PI =3.14159265358979;
 double r =150.0 + static_cast<double>(nodes.size()) *15.0;
 for (size_t i =0; i < nodes.size(); ++i) {
 double a = (2.0 * PI * static_cast<double>(i) / static_cast<double>(nodes.size())) - PI /2.0;
 positions[nodes[i]] = {400.0 + r * std::cos(a),300.0 + r * std::sin(a)};
 }
 return positions;
}
