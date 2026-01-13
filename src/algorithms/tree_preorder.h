#ifndef TREE_PREORDER_H
#define TREE_PREORDER_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <functional>

class TreePreOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    
public:
 TreePreOrder(TreeStructure* tree = nullptr) : treeStruct(tree) {}
    ~TreePreOrder() override = default;
    
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
 oss << "PreOrder Traversal (Root -> Left -> Right):\n";
   
 std::function<void(const TreeNode*)> preorder = [&](const TreeNode* node) {
     if (!node) return;
     oss << node->value << " ";
  preorder(node->left);
        preorder(node->right);
   };
        
  preorder(root);
        
     qDebug() << QString::fromStdString(oss.str());
   QMessageBox::information(nullptr, "PreOrder Traversal", QString::fromStdString(oss.str()));
    }
};

#endif // TREE_PREORDER_H
