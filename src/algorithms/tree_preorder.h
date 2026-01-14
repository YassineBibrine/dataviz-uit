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

class TreePreOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    int nodeCounter = 0;
    
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
        nodeCounter = 0;
        
        if (!treeStruct) {
   qDebug() << "No tree structure provided";
    return {};
        }
        
        const TreeNode* root = treeStruct->getRoot();
   if (!root) {
   qDebug() << "Tree is empty";
           return {};
     }
        
        // Initial frame
        AnimationFrame initialFrame;
    initialFrame.operationType = "Initial State";
        initialFrame.addAnnotation("Starting PreOrder Traversal");
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
        
       // Visit and process node FIRST (pre-order)
        AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
 visitFrame.addHighlightedNode("node_" + std::to_string(nodeCounter), "yellow");
        visitFrame.addAnnotation("PreOrder visiting node " + std::to_string(node->value));
        recordFrame(visitFrame);
        
result.push_back(node->value);
        
     AnimationFrame processFrame;
   processFrame.operationType = "Process Node";
  processFrame.addHighlightedNode("node_" + std::to_string(nodeCounter), "green");
        processFrame.addAnnotation("Node " + std::to_string(node->value) + " processed");
  recordFrame(processFrame);
        
        nodeCounter++;
        
        // Then traverse left and right
   preOrderTraverseWithFrames(node->left, result);
        preOrderTraverseWithFrames(node->right, result);
    }
    
    void recordFrame(const AnimationFrame& frame) {
        frames.push_back(frame);
    }
};

#endif // TREE_PREORDER_H
