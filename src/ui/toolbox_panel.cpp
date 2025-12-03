#include "toolbox_panel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QPainter>
#include <QPixmap>

// Fonction utilitaire pour dessiner une icône "à la volée"
// Plus besoin de fichiers images externes !
QIcon createIcon(const QString& shape, const QColor& color) {
    QPixmap pixmap(50, 50); // Carré de 50x50 pixels
    pixmap.fill(Qt::transparent); // Fond transparent

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing); // Pour que ce soit lisse

    // Style du trait
    QPen pen(color, 3);
    painter.setPen(pen);

    // Remplissage léger
    QColor fillColor = color;
    fillColor.setAlpha(50); // Semi-transparent
    painter.setBrush(fillColor);

    if (shape == "CIRCLE") {
        painter.drawEllipse(5, 5, 40, 40);
    }
    else if (shape == "RECT") {
        painter.drawRect(5, 10, 40, 30);
        // Petite barre pour faire style "Liste Chainée"
        painter.drawLine(30, 10, 30, 40);
    }
    else if (shape == "LINE") {
        painter.drawLine(5, 40, 45, 10); // Diagonale
    }

    else if (shape == "ERASER") {
        // Dessin d'une gomme (petit rectangle incliné ou simple carré)
        painter.setBrush(QColor("#e74c3c")); // Rouge
        painter.setPen(Qt::NoPen);
        painter.drawRect(10, 10, 30, 30);

        // Petit détail blanc au milieu
        painter.setBrush(Qt::white);
        painter.drawRect(10, 20, 30, 10);
    }
    

    return QIcon(pixmap);
}

ToolboxPanel::ToolboxPanel(QWidget* parent) : QWidget(parent) {
    setupUI();
    setFixedWidth(140); // Un peu plus large pour être à l'aise

    // Petit style CSS juste pour ce panneau (pour te donner un avant-goût !)
    this->setStyleSheet("background-color: #f0f0f0; border-right: 1px solid #ccc;");
    updateTools("Graph");
}

void ToolboxPanel::setupUI() {
    auto layout = new QVBoxLayout(this);

    // Titre stylé
    QLabel* title = new QLabel("OUTILS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; color: #333; margin-bottom: 10px;");
    layout->addWidget(title);

    // La liste
    listWidget = new QListWidget();
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setIconSize(QSize(45, 45));
    listWidget->setSpacing(15);
    listWidget->setDragEnabled(true);
    listWidget->setDragDropMode(QAbstractItemView::DragOnly); // On ne peut que prendre, pas déposer ici

    // Enlève la bordure de la liste
    listWidget->setStyleSheet("QListWidget { border: none; background: transparent; } "
        "QListWidget::item { color: #555; } "
        "QListWidget::item:hover { background: #e0e0e0; border-radius: 5px; }");

    // --- LES LIGNES POUR L'ALIGNEMENT VERTICAL ---
    listWidget->setFlow(QListWidget::TopToBottom); // Force l'ordre vertical
    listWidget->setWrapping(false); // Empêche de remettre à droite si ça dépasse
    listWidget->setResizeMode(QListWidget::Adjust); // S'adapte à la largeur
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Cache la barre du bas
    // -------------------------------------------------------

    // --- ITEM 1 : CERCLE (Graphe/Arbre) ---
    auto itemCircle = new QListWidgetItem(createIcon("CIRCLE", QColor("#3498db")), "Noeud");
    itemCircle->setData(Qt::UserRole, "CREATE_NODE_CIRCLE"); // Nouveau code
    itemCircle->setTextAlignment(Qt::AlignCenter);


    // --- ITEM 2 : RECTANGLE (Liste) ---
    auto itemRect = new QListWidgetItem(createIcon("RECT", QColor("#e67e22")), "Liste");
    itemRect->setData(Qt::UserRole, "CREATE_NODE_RECT");
    itemRect->setTextAlignment(Qt::AlignCenter);


    // --- ITEM 3 : LIEN (Mode connexion) ---
    auto itemLink = new QListWidgetItem(createIcon("LINE", QColor("#34495e")), "Lier");
    itemLink->setData(Qt::UserRole, "ACTIVATE_LINK_MODE"); // Code spécial pas de drag
    itemLink->setTextAlignment(Qt::AlignCenter);
    // On désactive le drag pour celui-là (car c'est un outil clic-clic)
    itemLink->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);


    layout->addWidget(listWidget);

    // --- AJOUT : Détection du clic pour "Lier" ---
    connect(listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        // On envoie le code (ex: ACTIVATE_LINK_MODE)
        emit toolSelected(item->data(Qt::UserRole).toString());
        });
}
void ToolboxPanel::updateTools(const QString& mode) {
    // 1. On vide la boîte à outils actuelle
    listWidget->clear();

    // 2. Selon le mode choisi dans le menu déroulant
    if (mode == "Linked List") {
        // --- MODE LISTE : On met le RECTANGLE ---
        auto itemRect = new QListWidgetItem(createIcon("RECT", QColor("#e67e22")), "Noeud Liste");
        itemRect->setData(Qt::UserRole, "CREATE_NODE_RECT");
        itemRect->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(itemRect);
    }
    else {
        // --- MODE GRAPHE OU ARBRE : On met le ROND ---
        auto itemCircle = new QListWidgetItem(createIcon("CIRCLE", QColor("#3498db")), "Noeud");
        itemCircle->setData(Qt::UserRole, "CREATE_NODE_CIRCLE");
        itemCircle->setTextAlignment(Qt::AlignCenter);
        listWidget->addItem(itemCircle);
    }

    // --- L'outil LIEN est toujours là (sauf si tu veux l'enlever pour certains modes) ---
    auto itemLink = new QListWidgetItem(createIcon("LINE", QColor("#34495e")), "Lier");
    itemLink->setData(Qt::UserRole, "ACTIVATE_LINK_MODE");
    itemLink->setTextAlignment(Qt::AlignCenter);
    itemLink->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    listWidget->addItem(itemLink);

    auto itemEraser = new QListWidgetItem(createIcon("ERASER", Qt::transparent), "Gomme");
    itemEraser->setData(Qt::UserRole, "ACTIVATE_ERASER_MODE");
    itemEraser->setTextAlignment(Qt::AlignCenter);
    itemEraser->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable); // Pas de drag, juste clic
    listWidget->addItem(itemEraser);
}
