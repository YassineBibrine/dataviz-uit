#pragma once

/**
 * @struct ListNode
 * @brief Doubly-linked list node
 */
struct ListNode {
    int value;
    ListNode* next;
    ListNode* prev;
    
    explicit ListNode(int val);
    ~ListNode();
};
