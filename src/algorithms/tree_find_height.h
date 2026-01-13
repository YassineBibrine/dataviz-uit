#ifndef TREE_FIND_HEIGHT_H
#define TREE_FIND_HEIGHT_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>
#include <functional>

class TreeFindHeight : public Algorithm {
private:
    TreeStructure* treeStruct;
    
public:
    TreeFindHeight(TreeStructure* tree = nullptr) : treeStruct(tree) {}
 ~TreeFindHeight() override = default;
    
  void execute() override {
  if (!treeStruct) {
  qDebug() << "No tree structure provided";
       return;
  }
   
        const TreeNode* root = treeStruct->getRoot();
     if (!root) {
    qDebug() << "Tree is empty - height is 0";
  QMessageBox::information(nullptr, "Tree Height", "Tree is empty. Height = 0");
       return;
        }
 
 std::function<int(const TreeNode*)> getHeight = [&](const TreeNode* node) -> int {
 if (!node) return 0;
       return 1 + std::max(getHeight(node->left), getHeight(node->right));
   };
        
  int height = getHeight(root);
        
  QString message = QString("Tree Height: %1\n\nThe longest path from root to leaf has %1 edges.")
    .arg(height - 1);  // Height is typically counted as edges, not nodes
   
        qDebug() << "Tree height:" << height;
   QMessageBox::information(nullptr, "Tree Height", message);
    }
};

#endif // TREE_FIND_HEIGHT_H
