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
 * @brief Bubble Sort algorithm implementation with animation frames
 * 
 * Repeatedly steps through the list, compares adjacent elements and swaps them
 * if they are in the wrong order. The pass through the list is repeated until
 * the list is sorted.
 * 
 * Time Complexity: O(n²)
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

    // Execute with frames and discard the result
   executeWithFrames();
    }

    std::vector<AnimationFrame> executeWithFrames() override {
        FrameRecorder recorder;

        if (!structure) {
          return recorder.getAllFrames();
        }

    // Handle Array Structure
        if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
   std::vector<int>& data = arr->getData();
            int n = static_cast<int>(data.size());

     // ? KEY FIX: Get canvas node IDs from the structure
        auto nodes = structure->getNodes();
     std::vector<std::string> canvasIds;
     for (const auto& node : nodes) {
     canvasIds.push_back(node.id);  // These are arr_0, arr_1, etc.
    }

       if (n <= 1) {
     // Already sorted - record single frame
      AnimationFrame frame;
           frame.operationType = "Complete";
   for (int i = 0; i < n; i++) {
frame.addHighlightedNode(canvasIds[i], "green");
      frame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
}
        frame.addAnnotation("Array is already sorted");
         recorder.recordFrame(frame);
                return recorder.getAllFrames();
            }

      // Initial state
 AnimationFrame initialFrame;
      initialFrame.operationType = "Initial State";
        for (int i = 0; i < n; i++) {
     initialFrame.addHighlightedNode(canvasIds[i], "blue");
             initialFrame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
            }
      initialFrame.addAnnotation("Starting Bubble Sort");
  recorder.recordFrame(initialFrame);

      // Bubble Sort Algorithm
    bool swapped;
for (int i = 0; i < n - 1; ++i) {
     swapped = false;

       AnimationFrame passFrame;
         passFrame.operationType = "Pass Start";
         for (int k = 0; k < n; k++) {
 if (k >= n - i) {
                passFrame.addHighlightedNode(canvasIds[k], "green");
       } else {
   passFrame.addHighlightedNode(canvasIds[k], "lightgray");
         }
        passFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
      }
     passFrame.addAnnotation("Pass " + std::to_string(i + 1) + ": Scanning unsorted portion");
    recorder.recordFrame(passFrame);

         for (int j = 0; j < n - i - 1; ++j) {
          // Compare adjacent elements
     AnimationFrame compareFrame;
     compareFrame.operationType = "Compare";
  for (int k = 0; k < n; k++) {
       if (k == j || k == j + 1) {
       compareFrame.addHighlightedNode(canvasIds[k], "yellow");
      } else if (k >= n - i) {
                  compareFrame.addHighlightedNode(canvasIds[k], "green");
    } else {
      compareFrame.addHighlightedNode(canvasIds[k], "lightgray");
        }
         compareFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
    }
        compareFrame.addAnnotation("Comparing: " + std::to_string(data[j]) + " and " + std::to_string(data[j + 1]));
    recorder.recordFrame(compareFrame);

             if (data[j] > data[j + 1]) {
    // Swap elements
          std::swap(data[j], data[j + 1]);
      swapped = true;

      AnimationFrame swapFrame;
              swapFrame.operationType = "Swap";
  for (int k = 0; k < n; k++) {
if (k == j || k == j + 1) {
               swapFrame.addHighlightedNode(canvasIds[k], "orange");
        } else if (k >= n - i) {
   swapFrame.addHighlightedNode(canvasIds[k], "green");
   } else {
      swapFrame.addHighlightedNode(canvasIds[k], "lightgray");
           }
  swapFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
      }
    swapFrame.addAnnotation("Swapped: " + std::to_string(data[j]) + " and " + std::to_string(data[j + 1]));
        recorder.recordFrame(swapFrame);
          }
       }

    // Mark last i+1 elements as sorted
       AnimationFrame sortedFrame;
    sortedFrame.operationType = "Element Sorted";
             for (int k = 0; k < n; k++) {
       if (k >= n - i - 1) {
sortedFrame.addHighlightedNode(canvasIds[k], "green");
         } else {
       sortedFrame.addHighlightedNode(canvasIds[k], "lightgray");
   }
    sortedFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
         }
             sortedFrame.addAnnotation("Element " + std::to_string(data[n - i - 1]) + " is now in final position");
     recorder.recordFrame(sortedFrame);

     // Early exit if no swaps occurred
    if (!swapped) {
       AnimationFrame earlyExitFrame;
        earlyExitFrame.operationType = "Early Exit";
          for (int k = 0; k < n; k++) {
  earlyExitFrame.addHighlightedNode(canvasIds[k], "green");
         earlyExitFrame.nodeLabels[canvasIds[k]] = std::to_string(data[k]);
      }
            earlyExitFrame.addAnnotation("No swaps needed - array is sorted!");
           recorder.recordFrame(earlyExitFrame);
                    break;
      }
        }

// Final sorted state
            AnimationFrame finalFrame;
      finalFrame.operationType = "Complete";
      for (int i = 0; i < n; i++) {
         finalFrame.addHighlightedNode(canvasIds[i], "green");
   finalFrame.nodeLabels[canvasIds[i]] = std::to_string(data[i]);
     }
      finalFrame.addAnnotation("? Bubble Sort Complete!");
    recorder.recordFrame(finalFrame);
        }
  // Handle List Structure
     else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
       // ? Get canvas node IDs from the structure (list_0, list_1, etc.)
         auto nodes = structure->getNodes();
   std::vector<std::string> canvasIds;
    std::vector<int> values;
   
      for (const auto& node : nodes) {
          canvasIds.push_back(node.id);
                values.push_back(std::stoi(node.value));
        }

     int n = static_cast<int>(values.size());
            if (n <= 1) {
     AnimationFrame frame;
     frame.operationType = "Complete";
                for (int i = 0; i < n; i++) {
       frame.addHighlightedNode(canvasIds[i], "green");
         frame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
       }
frame.addAnnotation("List is already sorted");
         recorder.recordFrame(frame);
   return recorder.getAllFrames();
            }

        // Initial state
    AnimationFrame initialFrame;
          initialFrame.operationType = "Initial State";
  for (int i = 0; i < n; i++) {
       initialFrame.addHighlightedNode(canvasIds[i], "blue");
    initialFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
}
      initialFrame.addAnnotation("Starting Bubble Sort on List");
        recorder.recordFrame(initialFrame);

     // Bubble Sort Algorithm
            bool swapped;
    for (int i = 0; i < n - 1; ++i) {
     swapped = false;

        AnimationFrame passFrame;
         passFrame.operationType = "Pass Start";
    for (int k = 0; k < n; k++) {
     if (k >= n - i) {
  passFrame.addHighlightedNode(canvasIds[k], "green");
       } else {
      passFrame.addHighlightedNode(canvasIds[k], "lightgray");
          }
 passFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
     }
       passFrame.addAnnotation("Pass " + std::to_string(i + 1) + ": Scanning unsorted portion");
       recorder.recordFrame(passFrame);

        for (int j = 0; j < n - i - 1; ++j) {
      AnimationFrame compareFrame;
      compareFrame.operationType = "Compare";
for (int k = 0; k < n; k++) {
    if (k == j || k == j + 1) {
 compareFrame.addHighlightedNode(canvasIds[k], "yellow");
            } else if (k >= n - i) {
         compareFrame.addHighlightedNode(canvasIds[k], "green");
          } else {
               compareFrame.addHighlightedNode(canvasIds[k], "lightgray");
   }
   compareFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
          }
                 compareFrame.addAnnotation("Comparing: " + std::to_string(values[j]) + " and " + std::to_string(values[j + 1]));
         recorder.recordFrame(compareFrame);

           if (values[j] > values[j + 1]) {
     std::swap(values[j], values[j + 1]);
  swapped = true;

     AnimationFrame swapFrame;
   swapFrame.operationType = "Swap";
 for (int k = 0; k < n; k++) {
     if (k == j || k == j + 1) {
     swapFrame.addHighlightedNode(canvasIds[k], "orange");
     } else if (k >= n - i) {
          swapFrame.addHighlightedNode(canvasIds[k], "green");
     } else {
       swapFrame.addHighlightedNode(canvasIds[k], "lightgray");
          }
   swapFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
   }
         swapFrame.addAnnotation("Swapped: " + std::to_string(values[j]) + " and " + std::to_string(values[j + 1]));
recorder.recordFrame(swapFrame);
         }
       }

AnimationFrame sortedFrame;
          sortedFrame.operationType = "Element Sorted";
       for (int k = 0; k < n; k++) {
                 if (k >= n - i - 1) {
 sortedFrame.addHighlightedNode(canvasIds[k], "green");
        } else {
sortedFrame.addHighlightedNode(canvasIds[k], "lightgray");
   }
    sortedFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
    }
        sortedFrame.addAnnotation("Element " + std::to_string(values[n - i - 1]) + " is now in final position");
   recorder.recordFrame(sortedFrame);

        if (!swapped) {
     AnimationFrame earlyExitFrame;
      earlyExitFrame.operationType = "Early Exit";
          for (int k = 0; k < n; k++) {
              earlyExitFrame.addHighlightedNode(canvasIds[k], "green");
 earlyExitFrame.nodeLabels[canvasIds[k]] = std::to_string(values[k]);
      }
           earlyExitFrame.addAnnotation("No swaps needed - list is sorted!");
   recorder.recordFrame(earlyExitFrame);
  break;
 }
            }

   // Write sorted values back to list
          list->clearList();
         for (int val : values) {
       list->append(val);
     }

       // Final sorted state
    AnimationFrame finalFrame;
    finalFrame.operationType = "Complete";
       for (int i = 0; i < n; i++) {
   finalFrame.addHighlightedNode(canvasIds[i], "green");
            finalFrame.nodeLabels[canvasIds[i]] = std::to_string(values[i]);
          }
     finalFrame.addAnnotation("? Bubble Sort Complete!");
            recorder.recordFrame(finalFrame);
        }

   return recorder.getAllFrames();
    }
};

#endif // BUBBLE_SORT_H
