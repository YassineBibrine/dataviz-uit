#include "postorder_runner.h"
#include "../core/execution_state.h"
#include "../core/frame_recorder.h"

void PostorderRunner::run() {
    if (!tree || !tree->root) return;
    postorder(tree->root);
}

void PostorderRunner::postorder(TreeNode* node) {
    if (!node) return;

    postorder(node->left);
    postorder(node->right);

    ExecutionState state;
    state.highlightedNode = node;
    FrameRecorder::getInstance().record(state);
}
