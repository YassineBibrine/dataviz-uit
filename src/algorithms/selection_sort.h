#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../algorithms/frame_recorder.h"
#include "../visualization/animation_frame.h"
#include <QDebug>
#include <vector>

// SelectionSort selects the minimum element from the unsorted portion and
// swaps it into place. This class supports non-visual execution and
// generation of animation frames for arrays and lists.
class SelectionSort : public Algorithm {
private:
 DataStructure* structure;

public:
 SelectionSort(DataStructure* ds = nullptr) : structure(ds) {}
 ~SelectionSort() override = default;

 void execute() override {
 if (!structure) {
 qDebug() << "No structure provided to SelectionSort";
 return;
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 executeOnArray(arr);
 } else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 executeOnList(list);
 } else {
 qDebug() << "Selection Sort: Unsupported structure type";
 }
 }

 // Generate frames describing the sort steps for visualization.
 std::vector<AnimationFrame> executeWithFrames() override {
 if (!structure) {
 qDebug() << "No structure provided to SelectionSort";
 return {};
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 return executeOnArrayWithFrames(arr);
 } else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 return executeOnListWithFrames(list);
 }

 qDebug() << "Selection Sort: Unsupported structure type";
 return {};
 }

private:
 // Non-visual array implementation.
 void executeOnArray(ArrayStructure* arr) {
 auto& data = arr->getData();
 int n = data.size();
 for (int i =0; i < n -1; i++) {
 int min_idx = i;
 for (int j = i +1; j < n; j++) {
 if (data[j] < data[min_idx]) min_idx = j;
 }
 if (min_idx != i) std::swap(data[i], data[min_idx]);
 }
 qDebug() << "Selection Sort completed on array";
 }

 // Non-visual list implementation (placeholder).
 void executeOnList(ListStructure* list) {
 qDebug() << "Selection Sort completed on list";
 }

 // Animated selection sort for arrays: records search, compare and swap frames.
 std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
 FrameRecorder recorder;
 auto& data = arr->getData();
 int n = data.size();

 // Map structure nodes to canvas IDs
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 for (const auto& node : nodes) canvasIds.push_back(node.id);

 // Initial state frame
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 std::string nodeId = canvasIds[i];
 initialFrame.addHighlightedNode(nodeId, "blue");
 initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
 }
 initialFrame.addAnnotation("Starting Selection Sort on array of " + std::to_string(n) + " elements");
 recorder.recordFrame(initialFrame);

 // Main loop: find minimum, swap, and record relevant frames.
 for (int i =0; i < n -1; i++) {
 AnimationFrame searchFrame;
 searchFrame.operationType = "Search Minimum";
 searchFrame.addHighlightedNode(canvasIds[i], "cyan");
 for (int k =0; k < i; k++) searchFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i; k < n; k++) searchFrame.addHighlightedNode(canvasIds[k], "yellow");
 for (int k =0; k < n; k++) searchFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 searchFrame.addAnnotation("Iteration " + std::to_string(i) + ": Searching for minimum in unsorted portion");
 recorder.recordFrame(searchFrame);

 int min_idx = i;
 for (int j = i +1; j < n; j++) {
 AnimationFrame compareFrame;
 compareFrame.operationType = "Compare";
 compareFrame.addHighlightedNode(canvasIds[min_idx], "red");
 compareFrame.addHighlightedNode(canvasIds[j], "orange");
 for (int k =0; k < i; k++) compareFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) compareFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 compareFrame.addAnnotation("Comparing current minimum and candidate");
 recorder.recordFrame(compareFrame);

 if (data[j] < data[min_idx]) {
 min_idx = j;
 AnimationFrame newMinFrame;
 newMinFrame.operationType = "New Minimum Found";
 newMinFrame.addHighlightedNode(canvasIds[min_idx], "magenta");
 for (int k =0; k < i; k++) newMinFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) newMinFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 newMinFrame.addAnnotation("New minimum found");
 recorder.recordFrame(newMinFrame);
 }
 }

 if (min_idx != i) {
 AnimationFrame beforeSwapFrame;
 beforeSwapFrame.operationType = "Before Swap";
 beforeSwapFrame.addHighlightedNode(canvasIds[i], "red");
 beforeSwapFrame.addHighlightedNode(canvasIds[min_idx], "orange");
 for (int k =0; k < i; k++) beforeSwapFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) beforeSwapFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 beforeSwapFrame.addAnnotation("Swap positions");
 recorder.recordFrame(beforeSwapFrame);

 std::swap(data[i], data[min_idx]);

 AnimationFrame afterSwapFrame;
 afterSwapFrame.operationType = "After Swap";
 for (int k =0; k <= i; k++) afterSwapFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i +1; k < n; k++) afterSwapFrame.addHighlightedNode(canvasIds[k], "lightgray");
 for (int k =0; k < n; k++) afterSwapFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 afterSwapFrame.addAnnotation("Element placed at position " + std::to_string(i));
 recorder.recordFrame(afterSwapFrame);
 } else {
 AnimationFrame alreadyMinFrame;
 alreadyMinFrame.operationType = "Already Minimum";
 for (int k =0; k <= i; k++) alreadyMinFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i +1; k < n; k++) alreadyMinFrame.addHighlightedNode(canvasIds[k], "lightgray");
 for (int k =0; k < n; k++) alreadyMinFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 alreadyMinFrame.addAnnotation("Element already in correct position");
 recorder.recordFrame(alreadyMinFrame);
 }
 }

 // Final completion frame
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 finalFrame.addHighlightedNode(canvasIds[i], "green");
 finalFrame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
 }
 finalFrame.addAnnotation("Array is completely sorted!");
 recorder.recordFrame(finalFrame);

 qDebug() << "SelectionSort generated" << recorder.getAllFrames().size() << "animation frames";
 return recorder.getAllFrames();
 }

 // Animated version for lists: sorts values and applies result back to list
 std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
 FrameRecorder recorder;
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 std::vector<int> values;
 for (const auto& node : nodes) { canvasIds.push_back(node.id); values.push_back(std::stoi(node.value)); }

 int n = values.size();
 if (n ==0) return {};

 // Initial frame
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) { initialFrame.addHighlightedNode(canvasIds[i], "blue"); initialFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]); }
 initialFrame.addAnnotation("Starting Selection Sort on list");
 recorder.recordFrame(initialFrame);

 // Sort values and record frames for each swap
 for (int i =0; i < n -1; i++) {
 int min_idx = i;
 for (int j = i +1; j < n; j++) if (values[j] < values[min_idx]) min_idx = j;
 if (min_idx != i) {
 std::swap(values[i], values[min_idx]);
 AnimationFrame afterSwapFrame;
 afterSwapFrame.operationType = "After Swap";
 for (int k =0; k <= i; k++) afterSwapFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i +1; k < n; k++) afterSwapFrame.addHighlightedNode(canvasIds[k], "lightgray");
 for (int k =0; k < n; k++) afterSwapFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 afterSwapFrame.addAnnotation("Element placed at position " + std::to_string(i));
 recorder.recordFrame(afterSwapFrame);
 }
 }

 // Apply sorted values to the list
 list->clearList();
 for (int val : values) list->append(val);

 // Final frame
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) { finalFrame.addHighlightedNode(canvasIds[i], "green"); finalFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]); }
 finalFrame.addAnnotation("List is completely sorted!");
 recorder.recordFrame(finalFrame);

 qDebug() << "SelectionSort generated" << recorder.getAllFrames().size() << "animation frames for list";
 return recorder.getAllFrames();
 }
};

#endif // SELECTION_SORT_H
