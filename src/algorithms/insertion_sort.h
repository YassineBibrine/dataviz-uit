#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../algorithms/frame_recorder.h"
#include "../visualization/animation_frame.h"
#include <QDebug>
#include <QMessageBox>
#include <vector>

// Insertion sort algorithm that supports arrays and linked lists and
// can generate animation frames describing each step for the renderer.
class InsertionSort : public Algorithm {
private:
 DataStructure* structure;

public:
 InsertionSort(DataStructure* ds = nullptr) : structure(ds) {}
 ~InsertionSort() override = default;

 void execute() override {
 if (!structure) {
 qDebug() << "No structure provided to InsertionSort";
 return;
 }

 // Try array first, then list. If neither matches, report unsupported type.
 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 executeOnArray(arr);
 }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 executeOnList(list);
 }
 else {
 qDebug() << "Insertion Sort: Unsupported structure type";
 }
 }

 // Produce frames that describe the algorithm execution for visualization.
 std::vector<AnimationFrame> executeWithFrames() override {
 if (!structure) {
 qDebug() << "No structure provided to InsertionSort";
 return {};
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 return executeOnArrayWithFrames(arr);
 }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 return executeOnListWithFrames(list);
 }

 qDebug() << "Insertion Sort: Unsupported structure type";
 return {};
 }

private:
 // Non-visual in-place execution on raw array data.
 void executeOnArray(ArrayStructure* arr) {
 auto& data = arr->getData();
 int n = data.size();
 for (int i =1; i < n; i++) {
 int key = data[i];
 int j = i -1;
 while (j >=0 && data[j] > key) {
 data[j +1] = data[j];
 j = j -1;
 }
 data[j +1] = key;
 }
 qDebug() << "Insertion Sort completed on array";
 }

 // Non-visual in-place execution on list (placeholder).
 void executeOnList(ListStructure* list) {
 // For linked list, the algorithm can be implemented similarly by
 // extracting values, sorting, and rebuilding the list.
 qDebug() << "Insertion Sort completed on list";
 }

 // Visual version for arrays: generates frames showing selection, comparisons,
 // shifts and insertions using the structure node IDs (canvas IDs).
 std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
 FrameRecorder recorder;
 auto& data = arr->getData();
 int n = data.size();

 // Obtain structure node IDs to map labels/highlights to canvas elements.
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 for (const auto& node : nodes) {
 canvasIds.push_back(node.id); // e.g. arr_0, arr_1, ...
 }

 // Initial frame: show all elements in blue with values.
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 std::string nodeId = canvasIds[i];
 initialFrame.addHighlightedNode(nodeId, "blue");
 initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
 }
 initialFrame.addAnnotation("Starting Insertion Sort on array of " + std::to_string(n) + " elements");
 recorder.recordFrame(initialFrame);

 // Main insertion sort loop with frames for selection, comparisons, shifts and insertion.
 for (int i =1; i < n; i++) {
 int key = data[i];
 // Frame: highlight key
 AnimationFrame selectKeyFrame;
 selectKeyFrame.operationType = "Select Key";
 selectKeyFrame.addHighlightedNode(canvasIds[i], "yellow");
 for (int k =0; k < i; k++) selectKeyFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) selectKeyFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 selectKeyFrame.addAnnotation("Iteration " + std::to_string(i) + ": Selected key = " + std::to_string(key));
 recorder.recordFrame(selectKeyFrame);

 int j = i -1;
 // While shifting is needed, produce compare and shift frames.
 while (j >=0 && data[j] > key) {
 AnimationFrame compareFrame;
 compareFrame.operationType = "Compare";
 compareFrame.addHighlightedNode(canvasIds[j], "red");
 compareFrame.addHighlightedNode(canvasIds[j +1], "orange");
 for (int k =0; k < j; k++) compareFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) compareFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 compareFrame.addAnnotation("Comparing: arr[" + std::to_string(j) + "]=" + std::to_string(data[j]) + " > key=" + std::to_string(key) + " ? YES - Shift needed");
 recorder.recordFrame(compareFrame);

 // Shift frame and update data
 AnimationFrame shiftFrame;
 shiftFrame.operationType = "Shift Right";
 shiftFrame.addHighlightedNode(canvasIds[j], "orange");
 shiftFrame.addHighlightedNode(canvasIds[j +1], "orange");
 for (int k =0; k < j; k++) shiftFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) shiftFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 shiftFrame.addAnnotation("Shifting arr[" + std::to_string(j) + "]=" + std::to_string(data[j]) + " to position " + std::to_string(j +1));
 recorder.recordFrame(shiftFrame);

 data[j +1] = data[j];
 j = j -1;

 AnimationFrame afterShiftFrame;
 afterShiftFrame.operationType = "After Shift";
 afterShiftFrame.addHighlightedNode(canvasIds[j +1], "orange");
 for (int k =0; k < j; k++) afterShiftFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) afterShiftFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 afterShiftFrame.addAnnotation("Element shifted to position " + std::to_string(j +1));
 recorder.recordFrame(afterShiftFrame);
 }

 // Insert key and record frames for insertion and after-insert state.
 AnimationFrame insertFrame;
 insertFrame.operationType = "Insert";
 insertFrame.addHighlightedNode(canvasIds[j +1], "cyan");
 for (int k =0; k <= i; k++) if (k != j +1) insertFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) insertFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 insertFrame.addAnnotation("Inserting key=" + std::to_string(key) + " at position " + std::to_string(j +1));
 recorder.recordFrame(insertFrame);

 data[j +1] = key;

 AnimationFrame afterInsertFrame;
 afterInsertFrame.operationType = "After Insert";
 for (int k =0; k <= i; k++) afterInsertFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i +1; k < n; k++) afterInsertFrame.addHighlightedNode(canvasIds[k], "lightgray");
 for (int k =0; k < n; k++) afterInsertFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
 afterInsertFrame.addAnnotation("Elements0-" + std::to_string(i) + " are now sorted");
 recorder.recordFrame(afterInsertFrame);
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

 qDebug() << "InsertionSort generated" << recorder.getAllFrames().size() << "animation frames";
 return recorder.getAllFrames();
 }

 // Visual version for linked lists: generates frames and applies sorted values back
 std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
 FrameRecorder recorder;
 auto nodes = structure->getNodes();
 std::vector<std::string> canvasIds;
 std::vector<int> values;
 for (const auto& node : nodes) {
 canvasIds.push_back(node.id);
 values.push_back(std::stoi(node.value));
 }

 int n = values.size();
 if (n ==0) return {};

 // Initial frame
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (int i =0; i < n; i++) {
 initialFrame.addHighlightedNode(canvasIds[i], "blue");
 initialFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
 }
 initialFrame.addAnnotation("Starting Insertion Sort on list of " + std::to_string(n) + " elements");
 recorder.recordFrame(initialFrame);

 // Sort values with frames, then write back to list
 for (int i =1; i < n; i++) {
 int key = values[i];
 AnimationFrame selectKeyFrame;
 selectKeyFrame.operationType = "Select Key";
 selectKeyFrame.addHighlightedNode(canvasIds[i], "yellow");
 for (int k =0; k < i; k++) selectKeyFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k =0; k < n; k++) selectKeyFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 selectKeyFrame.addAnnotation("Iteration " + std::to_string(i) + ": Selected key = " + std::to_string(key));
 recorder.recordFrame(selectKeyFrame);

 int j = i -1;
 while (j >=0 && values[j] > key) {
 values[j +1] = values[j];
 j--;
 }
 values[j +1] = key;

 AnimationFrame afterInsertFrame;
 afterInsertFrame.operationType = "After Insert";
 for (int k =0; k <= i; k++) afterInsertFrame.addHighlightedNode(canvasIds[k], "green");
 for (int k = i +1; k < n; k++) afterInsertFrame.addHighlightedNode(canvasIds[k], "lightgray");
 for (int k =0; k < n; k++) afterInsertFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
 afterInsertFrame.addAnnotation("Elements0-" + std::to_string(i) + " are now sorted");
 recorder.recordFrame(afterInsertFrame);
 }

 // Apply sorted values to the underlying list structure
 list->clearList();
 for (int val : values) list->append(val);

 // Final frame
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (int i =0; i < n; i++) {
 finalFrame.addHighlightedNode(canvasIds[i], "green");
 finalFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
 }
 finalFrame.addAnnotation("List is completely sorted!");
 recorder.recordFrame(finalFrame);

 qDebug() << "InsertionSort generated" << recorder.getAllFrames().size() << "animation frames for list";
 return recorder.getAllFrames();
 }
};

#endif // INSERTION_SORT_H
