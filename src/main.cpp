#include <QApplication>
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QString style = R"(
        /* FOND APP */
        QMainWindow { background-color: #E8EFF5; }
        
        /* TITRE APP */
        QLabel#appTitle {
            font-family: 'Segoe UI', sans-serif;
            font-size: 24px; font-weight: bold; color: #1A365D;
            padding: 5px; margin-bottom: 0px;
        }

        /* PANNEAUX BLANCS (Base) */
        QWidget#borderedPanel {
            background-color: #FFFFFF;
            border: 2px solid #BCCCDC; 
            border-radius: 8px;
        }

        /* FONT GLOBALE */
        QWidget {
            font-family: 'Segoe UI', sans-serif; font-size: 10pt; color: #2D3748;
        }

        /* --- GROUP BOX (LE CADRE BLEUTÉ) --- */
        QGroupBox {
            border: 1px solid #CBD5E0; /* Bordure grise/bleue fine */
            border-radius: 6px;
            margin-top: 22px; /* Place pour le titre */
            font-weight: bold;
            color: #2B6CB0; /* Titre en Bleu pro */
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
            /* Hack pour effacer la ligne derrière le titre */
            background-color: #FFFFFF; 
        }

        /* BOUTONS */
        QPushButton {
            background-color: #F7FAFC; border: 1px solid #CBD5E0;
            border-radius: 5px; padding: 5px 15px;
            min-height: 25px; /* Hauteur minimale garantie */
            color: #4A5568;
        }
        QPushButton:hover { background-color: #EDF2F7; border-color: #A0AEC0; }
        QPushButton:pressed { background-color: #E2E8F0; }

        /* PLAY BUTTON */
        QPushButton#playButton {
            background-color: #3182CE; color: white; border: 1px solid #2B6CB0;
        }
        QPushButton#playButton:hover { background-color: #2B6CB0; }

        /* OUTILS SELECTIONNÉS (Checkable) */
        QPushButton:checked {
            background-color: #EBF8FF; border-color: #3182CE; color: #2B6CB0;
        }

        /* INPUTS */
        QComboBox, QSpinBox {
            background-color: #FFFFFF; border: 1px solid #CBD5E0;
            border-radius: 5px; padding: 4px; min-height: 22px;
        }
        
        /* TABLEAU */
        QTableWidget {
            border: none; gridline-color: #E2E8F0;
        }
        QHeaderView::section {
            background-color: #F7FAFC; padding: 4px; border: none; font-weight: bold;
        }
    )";

    app.setStyleSheet(style);

    MainWindow window;
    window.showMaximized();

    return app.exec();
}