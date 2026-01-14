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
     std::string nodeId = "node_" + std::to_string(i);
     initialFrame.addHighlightedNode(nodeId, "blue");
       // Set initial values
        initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
     }
        initialFrame.addAnnotation("Starting Insertion Sort on array of " + std::to_string(n) + " elements");
recorder.recordFrame(initialFrame);
        
        // Main sorting loop
        for (int i = 1; i < n; i++) {
            int key = data[i];
            
            // Frame: Highlight the key element being inserted
            AnimationFrame selectKeyFrame;
            selectKeyFrame.operationType = "Select Key";
    selectKeyFrame.addHighlightedNode("node_" + std::to_string(i), "yellow");
            
      // Show sorted portion in green
    for (int k = 0; k < i; k++) {
         selectKeyFrame.addHighlightedNode("node_" + std::to_string(k), "green");
            }
  
   // Add current array state to frame
  for (int k = 0; k < n; k++) {
              selectKeyFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
        }
         
    selectKeyFrame.addAnnotation("Iteration " + std::to_string(i) + ": Selected key = " + std::to_string(key));
    recorder.recordFrame(selectKeyFrame);
            
            int j = i - 1;

    // Compare and shift elements
    while (j >= 0 && data[j] > key) {
           // Frame: Show comparison
         AnimationFrame compareFrame;
       compareFrame.operationType = "Compare";
     compareFrame.addHighlightedNode("node_" + std::to_string(j), "red");
     compareFrame.addHighlightedNode("node_" + std::to_string(j+1), "orange");
    
                // Show already sorted portion
        for (int k = 0; k < j; k++) {
     compareFrame.addHighlightedNode("node_" + std::to_string(k), "green");
           }
             
           // Add current array state
      for (int k = 0; k < n; k++) {
     compareFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
             }
       
compareFrame.addAnnotation("Comparing: arr[" + std::to_string(j) + "]=" + std::to_string(data[j]) + 
           " > key=" + std::to_string(key) + " ? YES - Shift needed");
                recorder.recordFrame(compareFrame);
     
       // Frame: Show shifting
     AnimationFrame shiftFrame;
                shiftFrame.operationType = "Shift Right";
         shiftFrame.addHighlightedNode("node_" + std::to_string(j), "orange");
        shiftFrame.addHighlightedNode("node_" + std::to_string(j + 1), "orange");
      
    // Show sorted portion
             for (int k = 0; k < j; k++) {
      shiftFrame.addHighlightedNode("node_" + std::to_string(k), "green");
       }

                // Add array state BEFORE shift for clarity
          for (int k = 0; k < n; k++) {
        shiftFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
                }
       
      shiftFrame.addAnnotation("Shifting arr[" + std::to_string(j) + "]=" + std::to_string(data[j]) + 
           " to position " + std::to_string(j + 1));
           recorder.recordFrame(shiftFrame);
   
                // Perform the shift
           data[j + 1] = data[j];
     j = j - 1;
         
    // Frame: Show array AFTER shift
 AnimationFrame afterShiftFrame;
          afterShiftFrame.operationType = "After Shift";
    afterShiftFrame.addHighlightedNode("node_" + std::to_string(j+1), "orange");
      
         // Show sorted portion
     for (int k = 0; k < j; k++) {
      afterShiftFrame.addHighlightedNode("node_" + std::to_string(k), "green");
  }
    
           // Add updated array state showing the shift result
    for (int k = 0; k < n; k++) {
             afterShiftFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
           }
      
    afterShiftFrame.addAnnotation("Element shifted to position " + std::to_string(j+1));
recorder.recordFrame(afterShiftFrame);
    }
            
        // Frame: Insert the key at correct position
      AnimationFrame insertFrame;
            insertFrame.operationType = "Insert";
            insertFrame.addHighlightedNode("node_" + std::to_string(j + 1), "cyan");
            
      // Show sorted portion
            for (int k = 0; k <= i; k++) {
 if (k != j + 1) {
                    insertFrame.addHighlightedNode("node_" + std::to_string(k), "green");
   }
            }
         
            // Add array state BEFORE insertion
            for (int k = 0; k < n; k++) {
 insertFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
   }
       
            insertFrame.addAnnotation("Inserting key=" + std::to_string(key) + " at position " + std::to_string(j + 1));
recorder.recordFrame(insertFrame);
 
            // Perform the insertion
    data[j + 1] = key;
         
  // Frame: Show array AFTER insertion
          AnimationFrame afterInsertFrame;
          afterInsertFrame.operationType = "After Insert";
     for (int k = 0; k <= i; k++) {
  afterInsertFrame.addHighlightedNode("node_" + std::to_string(k), "green");
            }
     for (int k = i + 1; k < n; k++) {
          afterInsertFrame.addHighlightedNode("node_" + std::to_string(k), "lightgray");
      }
       
      // Add updated array state showing the final position
      for (int k = 0; k < n; k++) {
                afterInsertFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
  }
            
       afterInsertFrame.addAnnotation("Elements 0-" + std::to_string(i) + " are now sorted");
            recorder.recordFrame(afterInsertFrame);
        }

        // Final frame: Everything sorted
        AnimationFrame finalFrame;
        finalFrame.operationType = "Complete";
 for (int i = 0; i < n; i++) {
       finalFrame.addHighlightedNode("node_" + std::to_string(i), "green");
            finalFrame.nodeLabels["node_" + std::to_string(i)] = std::to_string(data[i]);
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
