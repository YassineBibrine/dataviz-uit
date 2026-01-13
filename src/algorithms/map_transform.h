#ifndef MAP_TRANSFORM_H
#define MAP_TRANSFORM_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/list_node.h"
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
   if (!structure) {
  qDebug() << "No structure provided";
   return;
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
     for (auto& val : data) {
    val = val * multiplier + addValue;
 }
            
  QString message = QString("Transformed array:\nEach value = value × %1 + %2")
  .arg(multiplier)
  .arg(addValue);
       
    qDebug() << message;
  QMessageBox::information(nullptr, "Map Transform", message);
   }
 else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
      ListNode* current = const_cast<ListNode*>(list->getHead());
  while (current != nullptr) {
  current->value = current->value * multiplier + addValue;
   current = current->next;
 }
       
   QString message = QString("Transformed list:\nEach value = value × %1 + %2")
  .arg(multiplier)
 .arg(addValue);
 
   qDebug() << message;
 QMessageBox::information(nullptr, "Map Transform", message);
}
else {
    qDebug() << "Map Transform: Unsupported structure type";
  }
   }
    }
};

#endif // MAP_TRANSFORM_H
