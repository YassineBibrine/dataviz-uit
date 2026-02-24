#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/list_node.h"
#include "../algorithms/frame_recorder.h"
#include <vector>
#include <algorithm>

/**
 * @class BubbleSort
 * @brief Bubble Sort algorithm implementation that produces animation frames.
 *
 * The algorithm repeatedly steps through the sequence, compares adjacent
 * elements and swaps them if they are in the wrong order. Passes are
 * repeated until no swaps occur.
 *
 * Time Complexity: O(n^2)
 * Space Complexity: O(1)
 */
class BubbleSort : public Algorithm {
private:
 DataStructure* structure;

public:
 BubbleSort(DataStructure* ds = nullptr) : structure(ds) {}
 ~BubbleSort() override = default;

 void execute() override {
 if (!structure) return;

 // Execute and ignore returned frames
 executeWithFrames();
 }

 std::vector<AnimationFrame> executeWithFrames() override {
 FrameRecorder recorder;

 if (!structure) {
 return recorder.getAllFrames();
 }

 // Handle ArrayStructure case
 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 std::vector<int>& data = arr->getData();
 int n = static_cast<int>(data.size());

 // Obtain canvas node IDs corresponding to structure nodes
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 for (const auto& node : nodes) {
 canvasIds.push_back(node.id); // e.g. arr_0, arr_1, ...
 }

 if (n <=1) {
 // Already sorted - record a single completion frame
 AnimationFrame frame;
 frame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 frame.addHighlightedNode(canvasIds[i], "green");
 frame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
 }
 frame.addAnnotation("Array is already sorted");
 recorder.recordFrame(frame);
 return recorder.getAllFrames();
 }

 // Record initial state frame
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 initialFrame.addHighlightedNode(canvasIds[i], "blue");
 initialFrame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
 }
 initialFrame.addAnnotation("Starting Bubble Sort");
 recorder.recordFrame(initialFrame);

 // Bubble Sort main loop, producing frames for compare, swap, and pass completion
 bool swapped;
 for (int i =0; i < n -1; ++i) {
 swapped = false;

 AnimationFrame passFrame;
 passFrame.operationType = "Pass Start";
 for (int k =0; k < n; k++) {
 if (k >= n - i) {
 passFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 passFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 passFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 }
 passFrame.addAnnotation("Pass " + std::to_string(i +1) + ": Scanning unsorted portion");
 recorder.recordFrame(passFrame);

 for (int j =0; j < n - i -1; ++j) {
 // Produce a compare frame for the adjacent pair
 AnimationFrame compareFrame;
 compareFrame.operationType = "Compare";
 for (int k =0; k < n; k++) {
 if (k == j || k == j +1) {
 compareFrame.addHighlightedNode(canvasIds[k], "yellow");
 } else if (k >= n - i) {
 compareFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 compareFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 compareFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 }
 compareFrame.addAnnotation("Comparing: " + std::to_string(data[j]) + " and " + std::to_string(data[j +1]));
 recorder.recordFrame(compareFrame);

 if (data[j] > data[j +1]) {
 // Swap elements and record swap frame
 std::swap(data[j], data[j +1]);
 swapped = true;

 AnimationFrame swapFrame;
 swapFrame.operationType = "Swap";
 for (int k =0; k < n; k++) {
 if (k == j || k == j +1) {
 swapFrame.addHighlightedNode(canvasIds[k], "orange");
 } else if (k >= n - i) {
 swapFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 swapFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 swapFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 }
 swapFrame.addAnnotation("Swapped: " + std::to_string(data[j]) + " and " + std::to_string(data[j +1]));
 recorder.recordFrame(swapFrame);
 }
 }

 // Mark the last element of this pass as sorted
 AnimationFrame sortedFrame;
 sortedFrame.operationType = "Element Sorted";
 for (int k =0; k < n; k++) {
 if (k >= n - i -1) {
 sortedFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 sortedFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 sortedFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 }
 sortedFrame.addAnnotation("Element " + std::to_string(data[n - i -1]) + " is now in final position");
 recorder.recordFrame(sortedFrame);

 // Early exit optimization: if no swaps, array is sorted
 if (!swapped) {
 AnimationFrame earlyExitFrame;
 earlyExitFrame.operationType = "Early Exit";
 for (int k =0; k < n; k++) {
 earlyExitFrame.addHighlightedNode(canvasIds[k], "green");
 earlyExitFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 }
 earlyExitFrame.addAnnotation("No swaps needed - array is sorted!");
 recorder.recordFrame(earlyExitFrame);
 break;
 }
 }

 // Final sorted state frame
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 finalFrame.addHighlightedNode(canvasIds[i], "green");
 finalFrame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
 }
 finalFrame.addAnnotation("Bubble Sort Complete!");
 recorder.recordFrame(finalFrame);
 }
 // Handle ListStructure case
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 // Obtain canvas node IDs and extract integer values
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 std::vector<int> values;
 for (const auto& node : nodes) {
 canvasIds.push_back(node.id);
 values.push_back(std::stoi(node.value));
 }

 int n = static_cast<int>(values.size());
 if (n <=1) {
 // Already sorted - record a single completion frame
 AnimationFrame frame;
 frame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 frame.addHighlightedNode(canvasIds[i], "green");
 frame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
 }
 frame.addAnnotation("List is already sorted");
 recorder.recordFrame(frame);
 return recorder.getAllFrames();
 }

 // Record initial state for list
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 initialFrame.addHighlightedNode(canvasIds[i], "blue");
 initialFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
 }
 initialFrame.addAnnotation("Starting Bubble Sort on List");
 recorder.recordFrame(initialFrame);

 // Bubble Sort main loop for list values, recording frames
 bool swapped;
 for (int i =0; i < n -1; ++i) {
 swapped = false;

 AnimationFrame passFrame;
 passFrame.operationType = "Pass Start";
 for (int k =0; k < n; k++) {
 if (k >= n - i) {
 passFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 passFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 passFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 }
 passFrame.addAnnotation("Pass " + std::to_string(i +1) + ": Scanning unsorted portion");
 recorder.recordFrame(passFrame);

 for (int j =0; j < n - i -1; ++j) {
 AnimationFrame compareFrame;
 compareFrame.operationType = "Compare";
 for (int k =0; k < n; k++) {
 if (k == j || k == j +1) {
 compareFrame.addHighlightedNode(canvasIds[k], "yellow");
 } else if (k >= n - i) {
 compareFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 compareFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 compareFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 }
 compareFrame.addAnnotation("Comparing: " + std::to_string(values[j]) + " and " + std::to_string(values[j +1]));
 recorder.recordFrame(compareFrame);

 if (values[j] > values[j +1]) {
 std::swap(values[j], values[j +1]);
 swapped = true;

 AnimationFrame swapFrame;
 swapFrame.operationType = "Swap";
 for (int k =0; k < n; k++) {
 if (k == j || k == j +1) {
 swapFrame.addHighlightedNode(canvasIds[k], "orange");
 } else if (k >= n - i) {
 swapFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 swapFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 swapFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 }
 swapFrame.addAnnotation("Swapped: " + std::to_string(values[j]) + " and " + std::to_string(values[j +1]));
 recorder.recordFrame(swapFrame);
 }
 }

 AnimationFrame sortedFrame;
 sortedFrame.operationType = "Element Sorted";
 for (int k =0; k < n; k++) {
 if (k >= n - i -1) {
 sortedFrame.addHighlightedNode(canvasIds[k], "green");
 } else {
 sortedFrame.addHighlightedNode(canvasIds[k], "lightgray");
 }
 sortedFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 }
 sortedFrame.addAnnotation("Element " + std::to_string(values[n - i -1]) + " is now in final position");
 recorder.recordFrame(sortedFrame);

 if (!swapped) {
 AnimationFrame earlyExitFrame;
 earlyExitFrame.operationType = "Early Exit";
 for (int k =0; k < n; k++) {
 earlyExitFrame.addHighlightedNode(canvasIds[k], "green");
 earlyExitFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 }
 earlyExitFrame.addAnnotation("No swaps needed - list is sorted!");
 recorder.recordFrame(earlyExitFrame);
 break;
 }
 }

 // Write sorted values back into the list structure
 list->clearList();
 for (int val : values) {
 list->append(val);
 }

 // Final sorted state for list
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 finalFrame.addHighlightedNode(canvasIds[i], "green");
 finalFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
 }
 finalFrame.addAnnotation("Bubble Sort Complete!");
 recorder.recordFrame(finalFrame);
 }

 return recorder.getAllFrames();
 }
};

#endif // BUBBLE_SORT_H
