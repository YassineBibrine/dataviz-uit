#ifndef TREE_INORDER_H
#define TREE_INORDER_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <functional>

class TreeInOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    
public:
    TreeInOrder(TreeStructure* tree = nullptr) : treeStruct(tree) {}
    ~TreeInOrder() override = default;
 
    void execute() override {
        if (!treeStruct) {
            qDebug() << "No tree structure provided";
            return;
        }
        
        const TreeNode* root = treeStruct->getRoot();
        if (!root) {
            qDebug() << "Tree is empty";
            return;
        }
        
        std::ostringstream oss;
        oss << "InOrder Traversal (Left -> Root -> Right):\n";
        
        std::function<void(const TreeNode*)> inorder = [&](const TreeNode* node) {
            if (!node) return;
            inorder(node->left);
            oss << node->value << " ";
            inorder(node->right);
        };
        
        inorder(root);
        
        qDebug() << QString::fromStdString(oss.str());
        QMessageBox::information(nullptr, "InOrder Traversal", QString::fromStdString(oss.str()));
    }
};

#endif // TREE_INORDER_H
