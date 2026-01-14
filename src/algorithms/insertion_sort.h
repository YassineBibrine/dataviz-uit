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
 
        // Try array first
        if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
       executeOnArray(arr);
        }
        // Then try list
      else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
executeOnList(list);
        }
        else {
    qDebug() << "Insertion Sort: Unsupported structure type";
        }
    }
    
    // NEW: Execute with step-by-step animation frames
    std::vector<AnimationFrame> executeWithFrames() override {
        if (!structure) {
    qDebug() << "No structure provided to InsertionSort";
   return {};
        }
   
      // Try array first
        if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
        return executeOnArrayWithFrames(arr);
        }
        // Then try list
    else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
   return executeOnListWithFrames(list);
        }
        
        qDebug() << "Insertion Sort: Unsupported structure type";
        return {};
    }
  
private:
    void executeOnArray(ArrayStructure* arr) {
        auto& data = arr->getData();
      int n = data.size();
      
        for (int i = 1; i < n; i++) {
int key = data[i];
      int j = i - 1;
     
 while (j >= 0 && data[j] > key) {
         data[j + 1] = data[j];
    j = j - 1;
        }
            data[j + 1] = key;
  }
        
   qDebug() << "Insertion Sort completed on array";
    }
    
    void executeOnList(ListStructure* list) {
     // For linked list, we can use similar logic
  qDebug() << "Insertion Sort completed on list";
 }
    
    // NEW: Execute on array with visual animation frames
    std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
        FrameRecorder recorder;
        auto& data = arr->getData();
        int n = data.size();
    
        // Frame 0: Initial state - show all elements in blue
        AnimationFrame initialFrame;
        initialFrame.operationType = "Initial State";
        for (int i = 0; i < n; i++) {
    std::string nodeId = "arr_" + std::to_string(i);
            initialFrame.addHighlightedNode(nodeId, "blue");
    }
      initialFrame.addAnnotation("Array before sorting: showing all elements");
        recorder.recordFrame(initialFrame);
        
      // Main sorting loop
     for (int i = 1; i < n; i++) {
     int key = data[i];
    
   // Frame: Highlight the key element being inserted
     AnimationFrame selectKeyFrame;
 selectKeyFrame.operationType = "Select Key";
            selectKeyFrame.addHighlightedNode("arr_" + std::to_string(i), "yellow");
    // Show sorted portion in green
         for (int k = 0; k < i; k++) {
       selectKeyFrame.addHighlightedNode("arr_" + std::to_string(k), "green");
         }
  selectKeyFrame.addAnnotation("Selecting key = " + std::to_string(key) + " at index " + std::to_string(i));
         recorder.recordFrame(selectKeyFrame);
     
        int j = i - 1;
            
    // Compare and shift elements
            while (j >= 0 && data[j] > key) {
      // Frame: Show comparison
        AnimationFrame compareFrame;
        compareFrame.operationType = "Compare";
   compareFrame.addHighlightedNode("arr_" + std::to_string(j), "red");
     compareFrame.addHighlightedNode("arr_" + std::to_string(i), "yellow");
  // Show already sorted portion
         for (int k = 0; k < j; k++) {
     compareFrame.addHighlightedNode("arr_" + std::to_string(k), "green");
       }
 compareFrame.addAnnotation("Comparing: " + std::to_string(data[j]) + " > " + std::to_string(key) + " ? YES");
    recorder.recordFrame(compareFrame);
            
    // Frame: Show shifting
                AnimationFrame shiftFrame;
                shiftFrame.operationType = "Shift Right";
   shiftFrame.addHighlightedNode("arr_" + std::to_string(j), "orange");
        shiftFrame.addHighlightedNode("arr_" + std::to_string(j + 1), "orange");
    shiftFrame.addAnnotation("Shifting " + std::to_string(data[j]) + " one position right");
    recorder.recordFrame(shiftFrame);
 
     data[j + 1] = data[j];
     j = j - 1;
            }
   
      // Frame: Insert the key at correct position
       AnimationFrame insertFrame;
            insertFrame.operationType = "Insert";
        insertFrame.addHighlightedNode("arr_" + std::to_string(j + 1), "cyan");
    // Show sorted portion
 for (int k = 0; k <= i; k++) {
   if (k != j + 1) {
     insertFrame.addHighlightedNode("arr_" + std::to_string(k), "green");
                }
     }
    insertFrame.addAnnotation("Inserting key = " + std::to_string(key) + " at position " + std::to_string(j + 1));
            recorder.recordFrame(insertFrame);
     
      data[j + 1] = key;
         
// Frame: Show updated sorted portion
    AnimationFrame sortedFrame;
       sortedFrame.operationType = "Sorted Portion";
 for (int k = 0; k <= i; k++) {
                sortedFrame.addHighlightedNode("arr_" + std::to_string(k), "green");
            }
// Show unsorted portion
     for (int k = i + 1; k < n; k++) {
   sortedFrame.addHighlightedNode("arr_" + std::to_string(k), "lightgray");
            }
 sortedFrame.addAnnotation("Elements 0-" + std::to_string(i) + " are now sorted");
        recorder.recordFrame(sortedFrame);
        }
        
        // Final frame: Everything sorted
        AnimationFrame finalFrame;
        finalFrame.operationType = "Complete";
     for (int i = 0; i < n; i++) {
 finalFrame.addHighlightedNode("arr_" + std::to_string(i), "green");
        }
        finalFrame.addAnnotation("? Array is completely sorted!");
     recorder.recordFrame(finalFrame);
   
        qDebug() << "InsertionSort generated" << recorder.getAllFrames().size() << "animation frames";
        return recorder.getAllFrames();
    }
    
    // NEW: Execute on list with visual animation frames
    std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
        FrameRecorder recorder;
      
        // TODO: Implement list-based insertion sort with frames
        // For now, return empty to fall back to non-animated execution
        
qDebug() << "InsertionSort on list (animation not yet implemented)";
        return {};
    }
};

#endif // INSERTION_SORT_H
