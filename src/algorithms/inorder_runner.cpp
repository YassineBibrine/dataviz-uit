#include "inorder_runner.h"
#include "../core/execution_state.h"
#include "../core/frame_recorder.h"

void InorderRunner::run() {
    if (!tree || !tree->root) return;
    inorder(tree->root);
}

// Parcours inorder avec enregistrement des étapes
void InorderRunner::inorder(TreeNode* node) {
    if (!node) return;

    inorder(node->left);

    // Ici on enregistre l’état pour l’animation
    ExecutionState state;
    state.highlightedNode = node;  // surligne le noeud courant
    FrameRecorder::getInstance().record(state);

    inorder(node->right);
}
