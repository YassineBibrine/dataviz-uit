#include "memory_viewer.h"
#include <QPainter>
#include <iomanip>
#include <sstream>

MemoryViewer::MemoryViewer(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #f0f0f0;");
}

void MemoryViewer::updateMemoryState(const std::map<std::string, std::string> &memoryState)
{
    currentMemoryState = memoryState;
    update();
}

void MemoryViewer::highlightAddress(uintptr_t address)
{
    highlightedAddress = address;
    hasHighlight = true;
    update();
}

void MemoryViewer::clearHighlight()
{
    hasHighlight = false;
    update();
}

void MemoryViewer::paintEvent(QPaintEvent *event)
{
 QPainter painter(this);
    painter.fillRect(event->rect(), QColor(240, 240, 240));
    
    painter.drawText(rect(), Qt::AlignCenter,
      "Memory View\n(Low-level pointer and address visualization)");
    
    // TODO: Implement detailed memory layout visualization
}
