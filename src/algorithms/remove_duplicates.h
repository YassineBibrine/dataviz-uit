#ifndef REMOVE_DUPLICATES_H
#define REMOVE_DUPLICATES_H

#include "../orchestration/algorithm.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../algorithms/frame_recorder.h"
#include "../visualization/animation_frame.h"
#include <QDebug>
#include <QMessageBox>
#include <set>
#include <algorithm>

// RemoveDuplicates removes duplicate values from arrays or lists and
// can emit animation frames describing which elements are kept or removed.
class RemoveDuplicates : public Algorithm {
private:
 DataStructure* structure;

public:
 RemoveDuplicates(DataStructure* ds = nullptr) : structure(ds) {}
 ~RemoveDuplicates() override = default;

 void execute() override {
 if (!structure) {
 qDebug() << "No structure provided";
 return;
 }

 if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
 executeOnArray(arr);
 }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 executeOnList(list);
 }
 else {
 qDebug() << "RemoveDuplicates: Unsupported structure type";
 }
 }

 // Generate animation frames while removing duplicates.
 std::vector<AnimationFrame> executeWithFrames() override {
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

 qDebug() << "RemoveDuplicates: Unsupported structure type";
 return {};
 }

private:
 // Non-visual duplicate removal on arrays.
 void executeOnArray(ArrayStructure* arr) {
 auto& data = arr->getData();
 std::set<int> seen;
 auto it = data.begin();
 while (it != data.end()) {
 if (seen.count(*it)) {
 it = data.erase(it);
 } else {
 seen.insert(*it);
 ++it;
 }
 }
 qDebug() << "RemoveDuplicates completed on array";
 }

 // Non-visual duplicate removal on lists (placeholder).
 void executeOnList(ListStructure* list) {
 qDebug() << "RemoveDuplicates completed on list";
 }

 // Animated duplicate removal for arrays. Produces frames that show checks,
 // removals and the final cleaned array.
 std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
 FrameRecorder recorder;
 auto& data = arr->getData();
 int originalSize = data.size();

 // Initial state frame
 AnimationFrame initialFrame;
 initialFrame.operationType = "Initial State";
 for (size_t i =0; i < data.size(); i++) {
 std::string nodeId = "node_" + std::to_string(i);
 initialFrame.addHighlightedNode(nodeId, "blue");
 initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
 }
 initialFrame.addAnnotation("Starting RemoveDuplicates on array");
 recorder.recordFrame(initialFrame);

 // Iterate and remove duplicates while recording frames for check/remove/keep.
 std::set<int> seen;
 size_t position =0;
 auto it = data.begin();
 while (it != data.end()) {
 // Frame: Check current element
 AnimationFrame checkFrame;
 checkFrame.operationType = "Check Element";
 checkFrame.addHighlightedNode("node_" + std::to_string(position), "yellow");
 // Mark unique elements already processed
 for (size_t i =0; i < position; i++) checkFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (size_t i =0; i < data.size(); i++) checkFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 checkFrame.addAnnotation("Checking: " + std::to_string(*it) + " - Seen before? " + (seen.count(*it) ? "YES" : "NO"));
 recorder.recordFrame(checkFrame);

 if (seen.count(*it)) {
 // Frame: Remove duplicate
 AnimationFrame removeFrame;
 removeFrame.operationType = "Remove Duplicate";
 removeFrame.addHighlightedNode("node_" + std::to_string(position), "red");
 for (size_t i =0; i < position; i++) removeFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (size_t i =0; i < data.size(); i++) removeFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 removeFrame.addAnnotation("Removing duplicate: " + std::to_string(*it));
 recorder.recordFrame(removeFrame);

 it = data.erase(it);

 // Frame: After removal
 AnimationFrame afterRemoveFrame;
 afterRemoveFrame.operationType = "After Removal";
 for (size_t i =0; i < position && i < data.size(); i++) afterRemoveFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (size_t i =0; i < data.size(); i++) afterRemoveFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 afterRemoveFrame.addAnnotation("Duplicate removed");
 recorder.recordFrame(afterRemoveFrame);
 } else {
 // Frame: Keep element
 AnimationFrame keepFrame;
 keepFrame.operationType = "Keep Element";
 keepFrame.addHighlightedNode("node_" + std::to_string(position), "cyan");
 for (size_t i =0; i <= position && i < data.size(); i++) keepFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 for (size_t i =0; i < data.size(); i++) keepFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 keepFrame.addAnnotation("Keeping unique element: " + std::to_string(*it));
 recorder.recordFrame(keepFrame);

 seen.insert(*it);
 ++it;
 position++;
 }
 }

 // Final frame summarizing the result
 AnimationFrame finalFrame;
 finalFrame.operationType = "Complete";
 for (size_t i =0; i < data.size(); i++) {
 finalFrame.addHighlightedNode("node_" + std::to_string(i), "green");
 finalFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
 }
 int removed = originalSize - data.size();
 finalFrame.addAnnotation("Removed " + std::to_string(removed) + " duplicate(s)!");
 recorder.recordFrame(finalFrame);

 qDebug() << "RemoveDuplicates generated" << recorder.getAllFrames().size() << "animation frames";
 return recorder.getAllFrames();
 }

 // Animated duplicate removal for lists is not implemented yet.
 std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
 FrameRecorder recorder;
 qDebug() << "RemoveDuplicates on list (animation not yet implemented)";
 return {};
 }
};

#endif // REMOVE_DUPLICATES_H
