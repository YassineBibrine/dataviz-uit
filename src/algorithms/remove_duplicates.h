#ifndef REMOVE_DUPLICATES_H
#define REMOVE_DUPLICATES_H

#include "../orchestration/algorithm.h"
#include "../core/array_structure.h"
#include "../core/list_structure.h"
#include <QDebug>
#include <QMessageBox>
#include <set>
#include <algorithm>

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
            auto& data = arr->getData();
            int originalSize = data.size();

            std::set<int> seen;
            auto newEnd = std::remove_if(data.begin(), data.end(), [&seen](int val) {
                if (seen.count(val)) {
                    return true;  // Remove duplicate
                }
                seen.insert(val);
                return false;
            });
            data.erase(newEnd, data.end());

            QString message = QString("Removed %1 duplicate(s).\nOriginal size: %2\nNew size: %3")
                .arg(originalSize - data.size())
                .arg(originalSize)
                .arg(data.size());

            qDebug() << message;
            QMessageBox::information(nullptr, "Remove Duplicates", message);
        } else {
            qDebug() << "Remove Duplicates: Unsupported structure type";
        }
    }
};

#endif // REMOVE_DUPLICATES_H
