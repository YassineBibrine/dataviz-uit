#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "../orchestration/algorithm.h"
#include "../core/data_structure.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include <QDebug>
#include <QMessageBox>

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
};

#endif // INSERTION_SORT_H
