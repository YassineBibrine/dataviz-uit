#ifndef TREE_POSTORDER_H
#define TREE_POSTORDER_H

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

class TreePostOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    std::map<const TreeNode*, std::string> nodeToId;  // ? Map tree nodes to canvas IDs
    
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
            QMessageBox::information(nullptr, "PostOrder Traversal", "Tree is empty - no nodes to traverse");
  return;
        }
        
   std::vector<int> result;
        postOrderTraverse(root, result);
  
        QString message = "PostOrder Traversal: ";
     for (size_t i = 0; i < result.size(); ++i) {
         if (i > 0) message += ", ";
        message += QString::number(result[i]);
     }
   
        qDebug() << message;
      QMessageBox::information(nullptr, "PostOrder Traversal", message);
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
  qDebug() << "PostOrder: Tree node with value" << node->value;
         
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
          qDebug() << "PostOrder: ROOT node" << root->value << "mapped to" << QString::fromStdString(canvasId);
      } else {
   qDebug() << "PostOrder: Mapped node" << bfsNodes[i]->value << "to" << QString::fromStdString(canvasId);
        }
     }

  // Initial frame - ? Highlight the root
   AnimationFrame initialFrame;
     initialFrame.operationType = "Initial State";
  initialFrame.addHighlightedNode(rootCanvasId, "purple");  // ? Purple for root
      initialFrame.addAnnotation("Starting PostOrder Traversal (Left ? Right ? Root)\n? Root node (value " + 
 std::to_string(root->value) + ") is at the TOP");
   recordFrame(initialFrame);
     
 // Perform traversal with frame recording
     std::vector<int> result;
   postOrderTraverseWithFrames(root, result);
  
        // Final frame
      AnimationFrame finalFrame;
     finalFrame.operationType = "Complete";
      QString traversalOrder = "PostOrder: ";
        for (size_t i = 0; i < result.size(); ++i) {
       if (i > 0) traversalOrder += ", ";
    traversalOrder += QString::number(result[i]);
    }
   finalFrame.addAnnotation(traversalOrder.toStdString());
    recordFrame(finalFrame);

 qDebug() << "TreePostOrder generated" << frames.size() << "animation frames";
  return frames;
    }
    
private:
    void postOrderTraverse(const TreeNode* node, std::vector<int>& result) {
        if (!node) return;
   postOrderTraverse(node->left, result);
        postOrderTraverse(node->right, result);
        result.push_back(node->value);
    }
    
    void postOrderTraverseWithFrames(const TreeNode* node, std::vector<int>& result) {
     if (!node) return;
        
        // Traverse left first
    postOrderTraverseWithFrames(node->left, result);
 
   // Traverse right
      postOrderTraverseWithFrames(node->right, result);
    
        // THEN visit and process node (post-order) - ? Use actual canvas ID
        std::string canvasId = nodeToId[node];
        
        AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
        visitFrame.addHighlightedNode(canvasId, "orange");
        visitFrame.addAnnotation("PostOrder visiting node " + std::to_string(node->value) + " (Left ? Right ? Root)");
        recordFrame(visitFrame);
 
   result.push_back(node->value);
        
  AnimationFrame processFrame;
  processFrame.operationType = "Process Node";
        processFrame.addHighlightedNode(canvasId, "green");
        processFrame.addAnnotation("Node " + std::to_string(node->value) + " processed");
      recordFrame(processFrame);
    }
    
 void recordFrame(const AnimationFrame& frame) {
        frames.push_back(frame);
    }
};

#endif // TREE_POSTORDER_H
