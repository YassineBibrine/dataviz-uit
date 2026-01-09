#pragma once
#include "../core/tree_structure.h"
#include "algorithm_runner.h"

class PostorderRunner : public AlgorithmRunner {
public:
    TreeStructure* tree;

    PostorderRunner(TreeStructure* t) : tree(t) {}

    void run() override;

private:
    void postorder(TreeNode* node);
};
