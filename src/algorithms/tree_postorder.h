#ifndef TREE_POSTORDER_H
#define TREE_POSTORDER_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <functional>

class TreePostOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    
public:
    TreePostOrder(TreeStructure* tree = nullptr) : treeStruct(tree) {}
    ~TreePostOrder() override = default;
    
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
        oss << "PostOrder Traversal (Left -> Right -> Root):\n";
    
        std::function<void(const TreeNode*)> postorder = [&](const TreeNode* node) {
            if (!node) return;
            postorder(node->left);
            postorder(node->right);
            oss << node->value << " ";
        };
        
        postorder(root);
  
        qDebug() << QString::fromStdString(oss.str());
        QMessageBox::information(nullptr, "PostOrder Traversal", QString::fromStdString(oss.str()));
    }
};

#endif // TREE_POSTORDER_H
