#include "toolbox_panel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPainter>
#include <QPixmap>
#include <QGroupBox>

QIcon createIcon(const QString& shape, const QColor& color) {
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(color, 3);
    painter.setPen(pen);
    QColor fillColor = color;
    fillColor.setAlpha(50);
    painter.setBrush(fillColor);

    if (shape == "CIRCLE") {
        painter.drawEllipse(5, 5, 40, 40);
    }
    // --- NOUVEAU : Forme spécifique pour ARRAY (Carré simple) ---
    else if (shape == "ARRAY") {
        painter.drawRect(5, 5, 40, 40);
        // Pas de ligne interne, juste le carré
    }
    else if (shape == "RECT") {
        // Forme LISTE (Rectangle + Barre pointeur)
        painter.drawRect(5, 10, 40, 30);
        painter.drawLine(30, 10, 30, 40);
    }
    else if (shape == "LINE") {
        painter.drawLine(5, 40, 45, 10);
    }
    else if (shape == "ERASER") {
        painter.setBrush(QColor("#e74c3c")); painter.setPen(Qt::NoPen);
        painter.drawRect(10, 10, 30, 30);
        painter.setBrush(Qt::white); painter.drawRect(10, 20, 30, 10);
    }
    return QIcon(pixmap);
}

ToolboxPanel::ToolboxPanel(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setupUI();
}

void ToolboxPanel::setupUI() {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    
    QGroupBox* toolsGroup = new QGroupBox("TOOLS", this);
    toolsGroup->setStyleSheet(
        "QGroupBox::title { subcontrol-origin: margin; margin-left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* groupLayout = new QVBoxLayout(toolsGroup);
    groupLayout->setAlignment(Qt::AlignHCenter);
    groupLayout->setSpacing(5);
    groupLayout->setContentsMargins(20, 25, 0, 10);

    listWidget = new QListWidget();
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setIconSize(QSize(45, 45));
    listWidget->setGridSize(QSize(65, 75));
    listWidget->setSpacing(5);
    listWidget->setFixedWidth(70);

    listWidget->setDragEnabled(true);
    listWidget->setDragDropMode(QAbstractItemView::DragOnly);

    listWidget->setStyleSheet(
        "QListWidget { border: none; background: transparent; outline: none; }"
        "QListWidget::item { color: #555; margin-left: 2px; }"
        "QListWidget::item:hover { background: #E2E8F0; border-radius: 8px; }"
        "QListWidget::item:selected { background: #BEE3F8; border-radius: 8px; color: #2B6CB0; }"
    );

    listWidget->setFlow(QListWidget::TopToBottom);
    listWidget->setWrapping(false);
    listWidget->setResizeMode(QListWidget::Fixed);
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    updateTools("Graph");

    groupLayout->addWidget(listWidget, 1, Qt::AlignHCenter);
    mainLayout->addWidget(toolsGroup);

    connect(listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        emit toolSelected(item->data(Qt::UserRole).toString());
        });
}

void ToolboxPanel::updateTools(const QString& mode) {
    listWidget->clear();

    // Pour LinkedList
    if (mode == "Linked List" || mode == "LinkedList" || mode == "List") {
        // --- TRADUCTION : Noeud -> Node ---
        auto itemRect = new QListWidgetItem(createIcon("RECT", QColor("#e67e22")), "Node");
        itemRect->setData(Qt::UserRole, "CREATE_NODE_RECT");
        itemRect->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(itemRect);
    }
    // Pour Array
    else if (mode == "Array") {
        // --- MODIF : Utilise l'icône ARRAY et le rôle CREATE_NODE_ARRAY ---
        auto itemRect = new QListWidgetItem(createIcon("ARRAY", QColor("#9b59b6")), "Cell");
        itemRect->setData(Qt::UserRole, "CREATE_NODE_ARRAY"); // <-- IMPORTANT
        itemRect->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(itemRect);
    }
    // Pour Arbre et Graphe
    else {
        auto itemCircle = new QListWidgetItem(createIcon("CIRCLE", QColor("#3498db")), "Node");
        itemCircle->setData(Qt::UserRole, "CREATE_NODE_CIRCLE");
        itemCircle->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(itemCircle);
    }

    // Outil de lien (Link) - Pas pour Array
    if (mode != "Array") {
        
        auto itemLink = new QListWidgetItem(createIcon("LINE", QColor("#34495e")), "Link");
        itemLink->setData(Qt::UserRole, "ACTIVATE_LINK_MODE");
        itemLink->setTextAlignment(Qt::AlignCenter);
        itemLink->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        listWidget->addItem(itemLink);
    }

    // Outil gomme (Eraser)
    
    auto itemEraser = new QListWidgetItem(createIcon("ERASER", Qt::transparent), "Eraser");
    itemEraser->setData(Qt::UserRole, "ACTIVATE_ERASER_MODE");
    itemEraser->setTextAlignment(Qt::AlignCenter);
    itemEraser->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    listWidget->addItem(itemEraser);
}