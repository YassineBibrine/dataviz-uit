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

class TreeInOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    int nodeCounter = 0;
    
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
        initialFrame.addAnnotation("Starting InOrder Traversal");
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
        
 // Visit node
   AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
        visitFrame.addHighlightedNode("node_" + std::to_string(nodeCounter), "cyan");
        visitFrame.addAnnotation("Visiting node with value " + std::to_string(node->value));
        recordFrame(visitFrame);
        
   result.push_back(node->value);
        nodeCounter++;
        
        // Process node frame
  AnimationFrame processFrame;
        processFrame.operationType = "Process Node";
        processFrame.addHighlightedNode("node_" + std::to_string(nodeCounter - 1), "green");
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
