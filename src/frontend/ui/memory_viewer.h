#ifndef MEMORY_VIEWER_H
#define MEMORY_VIEWER_H

#include <QWidget>
#include <map>
#include <string>

/**
 * @class MemoryViewer
 * @brief Low-level memory visualization component
 * 
 * Displays:
 * - Heap memory allocations
 * - Pointer addresses and targets
 * - Memory layout and alignment
 * - Uninitialized memory regions
 */
class MemoryViewer : public QWidget {
    Q_OBJECT

public:
    MemoryViewer(QWidget *parent = nullptr);
    
    void updateMemoryState(const std::map<std::string, std::string> &memoryState);
    void highlightAddress(uintptr_t address);
    void clearHighlight();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::map<std::string, std::string> currentMemoryState;
    uintptr_t highlightedAddress = 0;
    bool hasHighlight = false;
};

#endif // MEMORY_VIEWER_H
