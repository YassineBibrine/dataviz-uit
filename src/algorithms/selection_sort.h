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
 }
        else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
       executeOnList(list);
 }
        else {
       qDebug() << "Selection Sort: Unsupported structure type";
}
    }
    
    // NEW: Execute with step-by-step animation frames
    std::vector<AnimationFrame> executeWithFrames() override {
      if (!structure) {
 qDebug() << "No structure provided to SelectionSort";
            return {};
        }
 
        if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
  return executeOnArrayWithFrames(arr);
        }
     else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
         return executeOnListWithFrames(list);
        }
        
        qDebug() << "Selection Sort: Unsupported structure type";
        return {};
    }
  
private:
    void executeOnArray(ArrayStructure* arr) {
     auto& data = arr->getData();
        int n = data.size();
  
        for (int i = 0; i < n - 1; i++) {
       int min_idx = i;
      for (int j = i + 1; j < n; j++) {
      if (data[j] < data[min_idx]) {
           min_idx = j;
          }
    }
            if (min_idx != i) {
         std::swap(data[i], data[min_idx]);
            }
        }
        
        qDebug() << "Selection Sort completed on array";
    }
    
    void executeOnList(ListStructure* list) {
        qDebug() << "Selection Sort completed on list";
    }
    
    // NEW: Execute on array with visual animation frames
    std::vector<AnimationFrame> executeOnArrayWithFrames(ArrayStructure* arr) {
        FrameRecorder recorder;
     auto& data = arr->getData();
        int n = data.size();
        
 // Frame 0: Initial state
  AnimationFrame initialFrame;
      initialFrame.operationType = "Initial State";
        for (int i = 0; i < n; i++) {
            std::string nodeId = "node_" + std::to_string(i);
     initialFrame.addHighlightedNode(nodeId, "blue");
   initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
        }
        initialFrame.addAnnotation("Starting Selection Sort on array of " + std::to_string(n) + " elements");
    recorder.recordFrame(initialFrame);
        
  // Main sorting loop
        for (int i = 0; i < n - 1; i++) {
            // Frame: Start searching for minimum
            AnimationFrame searchFrame;
  searchFrame.operationType = "Search Minimum";
            searchFrame.addHighlightedNode("node_" + std::to_string(i), "cyan");
         
         // Show already sorted portion
     for (int k = 0; k < i; k++) {
        searchFrame.addHighlightedNode("node_" + std::to_string(k), "green");
            }

            // Show unsorted portion
            for (int k = i; k < n; k++) {
     searchFrame.addHighlightedNode("node_" + std::to_string(k), "yellow");
            }
        
       // Add current array state
     for (int k = 0; k < n; k++) {
           searchFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
         }
            
         searchFrame.addAnnotation("Iteration " + std::to_string(i) + ": Searching for minimum in unsorted portion");
   recorder.recordFrame(searchFrame);
            
          int min_idx = i;
            
       // Compare elements
 for (int j = i + 1; j < n; j++) {
  // Frame: Comparing
AnimationFrame compareFrame;
                compareFrame.operationType = "Compare";
  
                // Current minimum
 compareFrame.addHighlightedNode("node_" + std::to_string(min_idx), "red");
          // Candidate
    compareFrame.addHighlightedNode("node_" + std::to_string(j), "orange");
  
                // Sorted portion
                for (int k = 0; k < i; k++) {
     compareFrame.addHighlightedNode("node_" + std::to_string(k), "green");
    }
                
     // Add current array state
                for (int k = 0; k < n; k++) {
                compareFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
   }
    
  compareFrame.addAnnotation("Comparing: arr[" + std::to_string(min_idx) + "]=" + std::to_string(data[min_idx]) + 
          " vs arr[" + std::to_string(j) + "]=" + std::to_string(data[j]));
   recorder.recordFrame(compareFrame);
    
          if (data[j] < data[min_idx]) {
                    min_idx = j;
    
                // Frame: Found new minimum
           AnimationFrame newMinFrame;
        newMinFrame.operationType = "New Minimum Found";
           newMinFrame.addHighlightedNode("node_" + std::to_string(min_idx), "magenta");
           
 // Sorted portion
        for (int k = 0; k < i; k++) {
     newMinFrame.addHighlightedNode("node_" + std::to_string(k), "green");
        }
      
            // Add current array state
          for (int k = 0; k < n; k++) {
      newMinFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
     }
   
            newMinFrame.addAnnotation("New minimum found: arr[" + std::to_string(min_idx) + "]=" + std::to_string(data[min_idx]));
         recorder.recordFrame(newMinFrame);
      }
      }
         
     if (min_idx != i) {
           // Frame: Before swap
         AnimationFrame beforeSwapFrame;
                beforeSwapFrame.operationType = "Before Swap";
                beforeSwapFrame.addHighlightedNode("node_" + std::to_string(i), "red");
 beforeSwapFrame.addHighlightedNode("node_" + std::to_string(min_idx), "orange");
     
      // Sorted portion
            for (int k = 0; k < i; k++) {
          beforeSwapFrame.addHighlightedNode("node_" + std::to_string(k), "green");
                }
   
 // Add current array state
                for (int k = 0; k < n; k++) {
          beforeSwapFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
            }
    
          beforeSwapFrame.addAnnotation("Swap positions: arr[" + std::to_string(i) + "] ? arr[" + std::to_string(min_idx) + "]");
                recorder.recordFrame(beforeSwapFrame);
                
                // Perform swap
std::swap(data[i], data[min_idx]);
    
     // Frame: After swap
    AnimationFrame afterSwapFrame;
          afterSwapFrame.operationType = "After Swap";
        
        // Sorted portion (now includes newly placed element)
       for (int k = 0; k <= i; k++) {
         afterSwapFrame.addHighlightedNode("node_" + std::to_string(k), "green");
            }
          
      // Unsorted portion
                for (int k = i + 1; k < n; k++) {
          afterSwapFrame.addHighlightedNode("node_" + std::to_string(k), "lightgray");
                }
      
    // Add updated array state
    for (int k = 0; k < n; k++) {
          afterSwapFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
      }
      
  afterSwapFrame.addAnnotation("Element " + std::to_string(data[i]) + " placed at position " + std::to_string(i));
    recorder.recordFrame(afterSwapFrame);
      } else {
        // Frame: Element already in correct position
  AnimationFrame alreadyMinFrame;
 alreadyMinFrame.operationType = "Already Minimum";
                
    // Sorted portion
       for (int k = 0; k <= i; k++) {
     alreadyMinFrame.addHighlightedNode("node_" + std::to_string(k), "green");
     }
       
           // Unsorted portion
        for (int k = i + 1; k < n; k++) {
       alreadyMinFrame.addHighlightedNode("node_" + std::to_string(k), "lightgray");
                }
       
  // Add current array state
          for (int k = 0; k < n; k++) {
alreadyMinFrame.nodeLabels["node_" + std::to_string(k)] = std::to_string(data[k]);
     }
       
      alreadyMinFrame.addAnnotation("Element already in correct position");
     recorder.recordFrame(alreadyMinFrame);
  }
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
        
   qDebug() << "SelectionSort generated" << recorder.getAllFrames().size() << "animation frames";
        return recorder.getAllFrames();
    }
    
    // NEW: Execute on list with visual animation frames
    std::vector<AnimationFrame> executeOnListWithFrames(ListStructure* list) {
     FrameRecorder recorder;
        
        // TODO: Implement list-based selection sort with frames
     qDebug() << "SelectionSort on list (animation not yet implemented)";
        return {};
    }
};

#endif // SELECTION_SORT_H
