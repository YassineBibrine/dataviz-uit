#include "preorder_runner.h"
#include "../core/execution_state.h"
#include "../core/frame_recorder.h"

void PreorderRunner::run() {
    if (!tree || !tree->root) return;
    preorder(tree->root);
}

void PreorderRunner::preorder(TreeNode* node) {
    if (!node) return;

    ExecutionState state;
    state.highlightedNode = node;
    FrameRecorder::getInstance().record(state);

    preorder(node->left);
    preorder(node->right);
}
