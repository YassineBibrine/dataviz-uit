#include "list_node.h"

ListNode::ListNode(int val) : value(val), next(nullptr), prev(nullptr) {}

ListNode::~ListNode() {
    // Note: Caller is responsible for managing list deletion
}
