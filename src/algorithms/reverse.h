#ifndef REVERSE_H
#define REVERSE_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/list_node.h"
#include "../algorithms/frame_recorder.h"
#include "../visualization/animation_frame.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

// Reverse reverses the order of elements in an array or linked list and
// provides an animated trace when requested.
class Reverse : public Algorithm {
private:
 DataStructure* structure;

public:
 Reverse(DataStructure* ds = nullptr) : structure(ds) {}
 ~Reverse() override = default;

 void execute() override {
 if (!structure) {
 qDebug() << "No structure provided";
 return;
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 auto& data = arr->getData();
 std::reverse(data.begin(), data.end());
 qDebug() << "Array reversed";
 QMessageBox::information(nullptr, "Reverse", "Array has been reversed successfully!");
 }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 // Reverse linked list by rebuilding from collected values in reverse order
 std::vector<int> values;
 ListNode* current = const_cast<ListNode*>(list->getHead());
 while (current != nullptr) {
 values.push_back(current->value);
 current = current->next;
 }
 list->clearList();
 for (auto it = values.rbegin(); it != values.rend(); ++it) {
 list->append(*it);
 }
 qDebug() << "List reversed";
 QMessageBox::information(nullptr, "Reverse", "List has been reversed successfully!");
 }
 else {
 qDebug() << "Reverse: Unsupported structure type";
 }
 }

 // Produce animation frames describing the reverse operation.
 std::vector<AnimationFrame> executeWithFrames() override {
 FrameRecorder recorder;
 if (!structure) {
 qDebug() << "No structure provided";
 return {};
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 return executeOnArrayWithFrames(arr);
 }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 return executeOnListWithFrames(list);
 }

 qDebug() << "Reverse: Unsupported structure type";
 return {};
 }

private:
 // Animated reverse for arrays: highlight swaps between symmetric positions.
 std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
 FrameRecorder recorder;
 auto& data = arr->getData();
 int n = data.size();

 // Initial frame showing values
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 std::string nodeId = "node_" + std::to_string(i);
 initialFrame.addHighlightedNode(nodeId, "blue");
 initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
 }
 initialFrame.addAnnotation("Starting Reverse on array of " + std::to_string(n) + " elements");
 recorder.recordFrame(initialFrame);

 int left =0, right = n -1;
 while (left < right) {
 // Frame: select pair to swap
 AnimationFrame selectFrame;
 selectFrame.operationType = "Select Swap";
 selectFrame.addHighlightedNode("node_" + std::to_string(left), "red");
 selectFrame.addHighlightedNode("node_" + std::to_string(right), "orange");
 for (int i =0; i < left; i++) selectFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (int i = right +1; i < n; i++) selectFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (int i =0; i < n; i++) selectFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 selectFrame.addAnnotation("Swapping arr[" + std::to_string(left) + "]=" + std::to_string(data[left]) + " with arr[" + std::to_string(right) + "]=" + std::to_string(data[right]));
 recorder.recordFrame(selectFrame);

 // Perform swap
 std::swap(data[left], data[right]);

 // Frame: after swap
 AnimationFrame afterSwapFrame;
 afterSwapFrame.operationType = "After Swap";
 afterSwapFrame.addHighlightedNode("node_" + std::to_string(left), "cyan");
 afterSwapFrame.addHighlightedNode("node_" + std::to_string(right), "cyan");
 for (int i =0; i <= left; i++) afterSwapFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (int i = right; i < n; i++) afterSwapFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (int i =0; i < n; i++) afterSwapFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 afterSwapFrame.addAnnotation("Swap complete");
 recorder.recordFrame(afterSwapFrame);

 left++;
 right--;
 }

 // Final completion frame
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 finalFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 finalFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 }
 finalFrame.addAnnotation("Array is completely reversed!");
 recorder.recordFrame(finalFrame);

 qDebug() << "Reverse generated" << recorder.getAllFrames().size() << "animation frames";
 return recorder.getAllFrames();
 }

 // Animated reverse for linked lists is not implemented yet.
 std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
 FrameRecorder recorder;
 qDebug() << "Reverse on list (animation not yet implemented)";
 return {};
 }
};

#endif // REVERSE_H
