#pragma once
#include "../core/tree_structure.h"
#include "algorithm_runner.h"

class PreorderRunner : public AlgorithmRunner {
public:
    TreeStructure* tree;

    PreorderRunner(TreeStructure* t) : tree(t) {}

    void run() override;

private:
    void preorder(TreeNode* node);
};
