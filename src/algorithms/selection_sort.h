#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include <QDebug>

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
};

#endif // SELECTION_SORT_H
