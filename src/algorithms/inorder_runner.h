#pragma once
#include "../core/tree_structure.h"
#include "algorithm_runner.h" // base class pour tous les algos

class InorderRunner : public AlgorithmRunner {
public:
    TreeStructure* tree;

    InorderRunner(TreeStructure* t) : tree(t) {}

    void run() override; // méthode principale
private:
    void inorder(TreeNode* node);
};
