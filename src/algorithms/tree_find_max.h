#ifndef TREE_FIND_MAX_H
#define TREE_FIND_MAX_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>

class TreeFindMax : public Algorithm {
private:
    TreeStructure* treeStruct;

public:
  TreeFindMax(TreeStructure* tree = nullptr) : treeStruct(tree) {}
  ~TreeFindMax() override = default;
    
  void execute() override {
   if (!treeStruct) {
     qDebug() << "No tree structure provided";
  return;
}
  
     const TreeNode* root = treeStruct->getRoot();
     if (!root) {
    qDebug() << "Tree is empty";
  QMessageBox::information(nullptr, "Find Maximum", "Tree is empty.");
    return;
     }
        
 // In a BST, maximum is the rightmost node
   const TreeNode* current = root;
  while (current->right != nullptr) {
  current = current->right;
     }
 
     QString message = QString("Maximum Value: %1\n\nFound at the rightmost node of the tree.")
    .arg(current->value);
  
        qDebug() << "Maximum value:" << current->value;
      QMessageBox::information(nullptr, "Find Maximum", message);
    }
};

#endif // TREE_FIND_MAX_H
