#ifndef TREE_LEVELORDER_H
#define TREE_LEVELORDER_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <queue>

class TreeLevelOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    
public:
  TreeLevelOrder(TreeStructure* tree = nullptr) : treeStruct(tree) {}
    ~TreeLevelOrder() override = default;
    
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
 oss << "LevelOrder Traversal (BFS - level by level):\n";
     
   std::queue<const TreeNode*> q;
q.push(root);
        
        while (!q.empty()) {
  const TreeNode* node = q.front();
 q.pop();
  
   oss << node->value << " ";
   
  if (node->left) q.push(node->left);
 if (node->right) q.push(node->right);
        }
        
  qDebug() << QString::fromStdString(oss.str());
   QMessageBox::information(nullptr, "LevelOrder Traversal", QString::fromStdString(oss.str()));
  }
};

#endif // TREE_LEVELORDER_H
