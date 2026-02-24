#pragma once

/**
 * @struct ListNode
 * @brief Doubly-linked list node used by ListStructure.
 *
 * The list implementation is simple: callers (ListStructure) are responsible
 * for allocation and deletion of nodes. The destructor is intentionally
 * empty because deletion is handled by the list container.
 */
struct ListNode {
 int value;
 ListNode* next;
 ListNode* prev;

 explicit ListNode(int val);
 ~ListNode();
};
