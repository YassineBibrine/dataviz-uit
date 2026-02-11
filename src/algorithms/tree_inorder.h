#ifndef TREE_INORDER_H
#define TREE_INORDER_H

#include "../orchestration/algorithm.h"
#include "../core/tree_structure.h"
#include "../core/tree_node.h"
#include "../algorithms/frame_recorder.h"
#include "../visualization/animation_frame.h"
#include <QDebug>
#include <QMessageBox>
#include <functional>
#include <vector>
#include <map>
#include <queue>

class TreeInOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    std::map<const TreeNode*, std::string> nodeToId;  // ? Map tree nodes to canvas IDs
 
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
            QMessageBox::information(nullptr, "InOrder Traversal", "Tree is empty - no nodes to traverse");
            return;
   }
  
        std::vector<int> result;
        inOrderTraverse(root, result);
     
        QString message = "InOrder Traversal: ";
        for (size_t i = 0; i < result.size(); ++i) {
    if (i > 0) message += ", ";
message += QString::number(result[i]);
   }
      
        qDebug() << message;
      QMessageBox::information(nullptr, "InOrder Traversal", message);
    }
    
  // NEW: Execute with step-by-step animation frames
 std::vector<AnimationFrame> executeWithFrames() override {
        frames.clear();
        nodeToId.clear();
     
 if (!treeStruct) {
qDebug() << "No tree structure provided";
   return {};
    }
     
 const TreeNode* root = treeStruct->getRoot();
 if (!root) {
       qDebug() << "Tree is empty";
    return {};
      }
      
    // ? FIX: Build mapping using BFS order (matches canvas layout)
   // The ROOT node will be assigned to "n1" (first canvas node)
 
        // Collect all tree nodes in BFS order starting from ROOT
        std::vector<const TreeNode*> bfsNodes;
    std::queue<const TreeNode*> bfsQueue;
  bfsQueue.push(root);  // ? Start from actual root
     
 while (!bfsQueue.empty()) {
      const TreeNode* node = bfsQueue.front();
 bfsQueue.pop();
    if (!node) continue;
          
    bfsNodes.push_back(node);
  qDebug() << "InOrder: Tree node with value" << node->value;
  
  if (node->left) bfsQueue.push(node->left);
  if (node->right) bfsQueue.push(node->right);
        }
  
        // ? KEY FIX: Map tree nodes to canvas IDs in BFS order
    // The first node in BFS (the root) gets "n1"
        std::string rootCanvasId = "";
for (size_t i = 0; i < bfsNodes.size(); ++i) {
 std::string canvasId = "n" + std::to_string(i + 1);  // n1, n2, n3, ...
   nodeToId[bfsNodes[i]] = canvasId;
          if (i == 0) {
       rootCanvasId = canvasId;  // ? First BFS node (root) ? "n1"
          qDebug() << "InOrder: ROOT node" << root->value << "mapped to" << QString::fromStdString(canvasId);
        } else {
      qDebug() << "InOrder: Mapped node" << bfsNodes[i]->value << "to" << QString::fromStdString(canvasId);
        }
     }

  // Initial frame - ? Highlight the root
AnimationFrame initialFrame;
   initialFrame.operationType = "Initial State";
   initialFrame.addHighlightedNode(rootCanvasId, "purple");  // ? Purple for root
     initialFrame.addAnnotation("Starting InOrder Traversal (Left ? Root ? Right)\n? Root node (value " + 
    std::to_string(root->value) + ") is at the TOP");
        recordFrame(initialFrame);
        
   // Perform traversal with frame recording
  std::vector<int> result;
      inOrderTraverseWithFrames(root, result);
    
        // Final frame
 AnimationFrame finalFrame;
    finalFrame.operationType = "Complete";
  QString traversalOrder = "InOrder: ";
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) traversalOrder += ", ";
  traversalOrder += QString::number(result[i]);
    }
        finalFrame.addAnnotation(traversalOrder.toStdString());
        recordFrame(finalFrame);
    
    qDebug() << "TreeInOrder generated" << frames.size() << "animation frames";
        return frames;
}
    
private:
    void inOrderTraverse(const TreeNode* node, std::vector<int>& result) {
        if (!node) return;
      inOrderTraverse(node->left, result);
        result.push_back(node->value);
  inOrderTraverse(node->right, result);
    }
    
 void inOrderTraverseWithFrames(const TreeNode* node, std::vector<int>& result) {
 if (!node) return;
        
     // Traverse left
  inOrderTraverseWithFrames(node->left, result);
   
 // Visit node - ? Use actual canvas ID
        std::string canvasId = nodeToId[node];
   AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
    visitFrame.addHighlightedNode(canvasId, "cyan");
     visitFrame.addAnnotation("InOrder visiting node " + std::to_string(node->value) + " (Left ? Root ? Right)");
        recordFrame(visitFrame);
     
   result.push_back(node->value);
        
        // Process node frame
  AnimationFrame processFrame;
        processFrame.operationType = "Process Node";
        processFrame.addHighlightedNode(canvasId, "green");
     processFrame.addAnnotation("Node " + std::to_string(node->value) + " processed");
        recordFrame(processFrame);
        
        // Traverse right
        inOrderTraverseWithFrames(node->right, result);
    }
    
    void recordFrame(const AnimationFrame& frame) {
     frames.push_back(frame);
    }
};

#endif // TREE_INORDER_H
