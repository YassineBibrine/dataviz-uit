#ifndef TREE_FIND_MIN_H
#define TREE_FIND_MIN_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>

class TreeFindMin : public Algorithm {
private:
TreeStructure* treeStruct;
    
public:
 TreeFindMin(TreeStructure* tree = nullptr) : treeStruct(tree) {}
  ~TreeFindMin() override = default;
    
  void execute() override {
   if (!treeStruct) {
  qDebug() << "No tree structure provided";
  return;
 }
  
const TreeNode* root = treeStruct->getRoot();
        if (!root) {
       qDebug() << "Tree is empty";
   QMessageBox::information(nullptr, "Find Minimum", "Tree is empty.");
       return;
 }
  
  // In a BST, minimum is the leftmost node
const TreeNode* current = root;
     while (current->left != nullptr) {
  current = current->left;
 }
     
  QString message = QString("Minimum Value: %1\n\nFound at the leftmost node of the tree.")
  .arg(current->value);
        
        qDebug() << "Minimum value:" << current->value;
      QMessageBox::information(nullptr, "Find Minimum", message);
    }
};

#endif // TREE_FIND_MIN_H
