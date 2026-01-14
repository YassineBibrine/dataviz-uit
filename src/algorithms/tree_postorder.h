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

class TreePostOrder : public Algorithm {
private:
    TreeStructure* treeStruct;
    std::vector<AnimationFrame> frames;
    int nodeCounter = 0;
    
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
        initialFrame.addAnnotation("Starting PostOrder Traversal");
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
        
        // THEN visit and process node (post-order)
        AnimationFrame visitFrame;
        visitFrame.operationType = "Visit Node";
        visitFrame.addHighlightedNode("node_" + std::to_string(nodeCounter), "orange");
        visitFrame.addAnnotation("PostOrder visiting node " + std::to_string(node->value));
        recordFrame(visitFrame);
        
        result.push_back(node->value);
        
        AnimationFrame processFrame;
        processFrame.operationType = "Process Node";
        processFrame.addHighlightedNode("node_" + std::to_string(nodeCounter), "green");
        processFrame.addAnnotation("Node " + std::to_string(node->value) + " processed");
        recordFrame(processFrame);
        
        nodeCounter++;
    }
    
    void recordFrame(const AnimationFrame& frame) {
        frames.push_back(frame);
    }
};

#endif // TREE_POSTORDER_H
