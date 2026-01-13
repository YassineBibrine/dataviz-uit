#include "tree_algorithm_factory.h"
#include "../algorithms/tree_inorder.h"
#include "../algorithms/tree_preorder.h"
#include "../algorithms/tree_postorder.h"
#include "../algorithms/tree_levelorder.h"
#include "../algorithms/tree_find_height.h"
#include "../algorithms/tree_find_min.h"
#include "../algorithms/tree_find_max.h"
#include <stdexcept>

std::unique_ptr<Algorithm> TreeAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "InOrder") {
        return std::make_unique<TreeInOrder>();
    }
    else if (type == "PreOrder") {
        return std::make_unique<TreePreOrder>();
    }
    else if (type == "PostOrder") {
        return std::make_unique<TreePostOrder>();
    }
    else if (type == "LevelOrder") {
        return std::make_unique<TreeLevelOrder>();
    }
    else if (type == "FindHeight") {
        return std::make_unique<TreeFindHeight>();
    }
    else if (type == "FindMin") {
        return std::make_unique<TreeFindMin>();
    }
    else if (type == "FindMax") {
        return std::make_unique<TreeFindMax>();
    }
  
    throw std::invalid_argument("Unknown tree algorithm type: " + type);
}
