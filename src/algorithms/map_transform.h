#ifndef MAP_TRANSFORM_H
#define MAP_TRANSFORM_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/list_node.h"
#include "../algorithms/frame_recorder.h"
#include "../ui/algorithm_input_dialog.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

class MapTransform : public Algorithm {
private:
 DataStructure* structure;
    
public:
    MapTransform(DataStructure* ds = nullptr) : structure(ds) {}
    ~MapTransform() override = default;
    
    void execute() override {
// Execute with frames and discard the result
   executeWithFrames();
    }
    
    std::vector<AnimationFrame> executeWithFrames() override {
        FrameRecorder recorder;
   
        if (!structure) {
   qDebug() << "No structure provided";
       return recorder.getAllFrames();
        }

      // Ask user for transformation: multiply, add, or square
    AlgorithmInputDialog dialog("Map Transform");
  dialog.addIntInput("Multiply by", 2, -100, 100);
 dialog.addIntInput("Then add", 0, -100, 100);
    
      if (dialog.exec() == QDialog::Accepted) {
  int multiplier = dialog.getIntValue(0);
   int addValue = dialog.getIntValue(1);
     
    if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
    auto& data = arr->getData();
   int n = static_cast<int>(data.size());
      
          // Initial state
    AnimationFrame initialFrame;
     initialFrame.operationType = "Initial State";
      for (int i = 0; i < n; i++) {
      std::string nodeId = "node_" + std::to_string(i);
         initialFrame.addHighlightedNode(nodeId, "blue");
   initialFrame.nodeLabels[nodeId] = std::to_string(data[i]);
 }
       initialFrame.addAnnotation("Starting Map Transform: value = value × " + std::to_string(multiplier) + " + " + std::to_string(addValue));
   recorder.recordFrame(initialFrame);
   
        // Transform each element
     for (int i = 0; i < n; i++) {
   int oldValue = data[i];
        data[i] = data[i] * multiplier + addValue;
       
        AnimationFrame transformFrame;
      transformFrame.operationType = "Transform";
    for (int k = 0; k < n; k++) {
   std::string nodeId = "node_" + std::to_string(k);
 if (k == i) {
            transformFrame.addHighlightedNode(nodeId, "yellow");
  } else if (k < i) {
        transformFrame.addHighlightedNode(nodeId, "green");
    } else {
     transformFrame.addHighlightedNode(nodeId, "lightgray");
        }
transformFrame.nodeLabels[nodeId] = std::to_string(data[k]);
     }
     transformFrame.addAnnotation("Transformed index " + std::to_string(i) + ": " + std::to_string(oldValue) + " ? " + std::to_string(data[i]));
   recorder.recordFrame(transformFrame);
      }
       
     // Final state
   AnimationFrame finalFrame;
        finalFrame.operationType = "Complete";
   for (int i = 0; i < n; i++) {
       std::string nodeId = "node_" + std::to_string(i);
             finalFrame.addHighlightedNode(nodeId, "green");
          finalFrame.nodeLabels[nodeId] = std::to_string(data[i]);
  }
  finalFrame.addAnnotation("? Map Transform Complete!");
        recorder.recordFrame(finalFrame);
            }
       else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
      // Convert list to vector for frames
std::vector<int> values;
  ListNode* current = const_cast<ListNode*>(list->getHead());
        while (current != nullptr) {
            values.push_back(current->value);
            current = current->next;
    }
      int n = static_cast<int>(values.size());

            // Initial state
   AnimationFrame initialFrame;
  initialFrame.operationType = "Initial State";
    for (int i = 0; i < n; i++) {
  std::string nodeId = "node_" + std::to_string(i);
        initialFrame.addHighlightedNode(nodeId, "blue");
  initialFrame.nodeLabels[nodeId] = std::to_string(values[i]);
           }
      initialFrame.addAnnotation("Starting Map Transform: value = value × " + std::to_string(multiplier) + " + " + std::to_string(addValue));
     recorder.recordFrame(initialFrame);
      
   // Transform each element
       current = const_cast<ListNode*>(list->getHead());
   int index = 0;
   while (current != nullptr) {
       int oldValue = current->value;
        current->value = current->value * multiplier + addValue;
    values[index] = current->value;

        AnimationFrame transformFrame;
  transformFrame.operationType = "Transform";
for (int k = 0; k < n; k++) {
     std::string nodeId = "node_" + std::to_string(k);
      if (k == index) {
 transformFrame.addHighlightedNode(nodeId, "yellow");
       } else if (k < index) {
     transformFrame.addHighlightedNode(nodeId, "green");
  } else {
 transformFrame.addHighlightedNode(nodeId, "lightgray");
     }
    transformFrame.nodeLabels[nodeId] = std::to_string(values[k]);
  }
        transformFrame.addAnnotation("Transformed node " + std::to_string(index) + ": " + std::to_string(oldValue) + " ? " + std::to_string(current->value));
     recorder.recordFrame(transformFrame);

 current = current->next;
     index++;
     }
     
  // Final state
   AnimationFrame finalFrame;
   finalFrame.operationType = "Complete";
     for (int i = 0; i < n; i++) {
  std::string nodeId = "node_" + std::to_string(i);
   finalFrame.addHighlightedNode(nodeId, "green");
   finalFrame.nodeLabels[nodeId] = std::to_string(values[i]);
     }
       finalFrame.addAnnotation("? Map Transform Complete!");
            recorder.recordFrame(finalFrame);
       }
    else {
    qDebug() << "Map Transform: Unsupported structure type";
  }
        }
        
return recorder.getAllFrames();
    }
};

#endif // MAP_TRANSFORM_H
