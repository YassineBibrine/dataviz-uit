#ifndef REVERSE_H
#define REVERSE_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include "../core/list_node.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

class Reverse : public Algorithm {
private:
  DataStructure* structure;
    
public:
 Reverse(DataStructure* ds = nullptr) : structure(ds) {}
    ~Reverse() override = default;
 
  void execute() override {
  if (!structure) {
  qDebug() << "No structure provided";
 return;
 }
 
  if (ArrayStructure* arr = dynamic_cast<ArrayStructure*>(structure)) {
      auto& data = arr->getData();
  std::reverse(data.begin(), data.end());
 
   qDebug() << "Array reversed";
  QMessageBox::information(nullptr, "Reverse", "Array has been reversed successfully!");
  }
    else if (ListStructure* list = dynamic_cast<ListStructure*>(structure)) {
 // For list, we need to manually rebuild it reversed
        // Collect values first
  std::vector<int> values;
   ListNode* current = const_cast<ListNode*>(list->getHead());
        while (current != nullptr) {
   values.push_back(current->value);
  current = current->next;
 }
        
        // Clear and rebuild in reverse order
   list->clearList();
        for (auto it = values.rbegin(); it != values.rend(); ++it) {
 list->append(*it);
        }
  
    qDebug() << "List reversed";
  QMessageBox::information(nullptr, "Reverse", "List has been reversed successfully!");
   }
 else {
     qDebug() << "Reverse: Unsupported structure type";
 }
  }
};

#endif // REVERSE_H
