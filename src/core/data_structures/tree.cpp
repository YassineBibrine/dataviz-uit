#include "tree.h"

Tree::Tree() = default;

void Tree::insertNode(const std::string &id, const std::string &parentId)
{
    Node node;
    node.id = id;
    node.parentId = parentId;
    nodes[id] = node;
    
    if (!parentId.empty() && nodes.find(parentId) != nodes.end()) {
        nodes[parentId].childrenIds.push_back(id);
    } else if (nodes.size() == 1) {
        rootId = id;
    }
}

void Tree::removeNode(const std::string &id)
{
    // TODO: Implement node removal with tree restructuring
}

void Tree::updateNodeProperty(const std::string &id, const std::string &key, 
  const std::string &value)
{
    if (nodes.find(id) != nodes.end()) {
    nodes[id].properties[key] = value;
    }
}

bool Tree::hasNode(const std::string &id) const
{
    return nodes.find(id) != nodes.end();
}

int Tree::getNodeCount() const
{
    return nodes.size();
}
