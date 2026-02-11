#ifndef TREE_PREORDER_H
#define TREE_PREORDER_H

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

class TreePreOrder : public Algorithm {
private:
  TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    std::map<const TreeNode*, std::string> nodeToId;  // ? Map tree nodes to canvas IDs
    
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
         QMessageBox::information(nullptr, "PreOrder Traversal", "Tree is empty - no nodes to traverse");
    return;
        }
        
   std::vector<int> result;
        preOrderTraverse(root, result);
        
        QString message = "PreOrder Traversal: ";
     for (size_t i = 0; i < result.size(); ++i) {
     if (i > 0) message += ", ";
         message += QString::number(result[i]);
     }
  
 qDebug() << message;
        QMessageBox::information(nullptr, "PreOrder Traversal", message);
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
       qDebug() << "PreOrder: Tree node with value" << node->value;
 
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
              qDebug() << "PreOrder: ROOT node" << root->value << "mapped to" << QString::fromStdString(canvasId);
        } else {
       qDebug() << "PreOrder: Mapped node" << bfsNodes[i]->value << "to" << QString::fromStdString(canvasId);
        }
     }

        // Initial frame - ? Highlight the root
  AnimationFrame initialFrame;
    initialFrame.operationType = "Initial State";
        initialFrame.addHighlightedNode(rootCanvasId, "purple");// ? Purple for root
 initialFrame.addAnnotation("Starting PreOrder Traversal (Root ? Left ? Right)\n? Root node (value " + 
   std::to_string(root->value) + ") is at the TOP");
  recordFrame(initialFrame);
  
 // Perform traversal with frame recording
 std::vector<int> result;
 preOrderTraverseWithFrames(root, result);
        
  // Final frame
 AnimationFrame finalFrame;
  finalFrame.operationType = "Complete";
  QString traversalOrder = "PreOrder: ";
        for (size_t i = 0; i < result.size(); ++i) {
   if (i > 0) traversalOrder += ", ";
traversalOrder += QString::number(result[i]);
     }
  finalFrame.addAnnotation(traversalOrder.toStdString());
     recordFrame(finalFrame);
  
qDebug() << "TreePreOrder generated" << frames.size() << "animation frames";
    return frames;
    }
    
private:
    void preOrderTraverse(const TreeNode* node, std::vector<int>& result) {
 if (!node) return;
        result.push_back(node->value);
        preOrderTraverse(node->left, result);
     preOrderTraverse(node->right, result);
    }
    
    void preOrderTraverseWithFrames(const TreeNode* node, std::vector<int>& result) {
   if (!node) return;
      
        // Visit and process node FIRST (pre-order) - ? Use actual canvas ID
        std::string canvasId = nodeToId[node];
        
        AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
 visitFrame.addHighlightedNode(canvasId, "yellow");
        visitFrame.addAnnotation("PreOrder visiting node " + std::to_string(node->value) + " (Root ? Left ? Right)");
   recordFrame(visitFrame);
        
result.push_back(node->value);
        
     AnimationFrame processFrame;
   processFrame.operationType = "Process Node";
  processFrame.addHighlightedNode(canvasId, "green");
     processFrame.addAnnotation("Node " + std::to_string(node->value) + " processed");
  recordFrame(processFrame);
        
        // Then traverse left and right
   preOrderTraverseWithFrames(node->left, result);
        preOrderTraverseWithFrames(node->right, result);
    }
    
    void recordFrame(const AnimationFrame& frame) {
        frames.push_back(frame);
    }
};

#endif // TREE_PREORDER_H
