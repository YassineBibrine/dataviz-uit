#include <QApplication>
#include <QGuiApplication>
#include "ui/main_window.h"
#include <iostream>
#include <exception>
#include <QDebug>
#include <QStyleHints>
#include <QPalette>

// Helper function to detect if system is in dark mode
bool isDarkMode() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
#else
    // Fallback for older Qt versions: check if window background is dark
    QPalette defaultPalette;
    QColor windowColor = defaultPalette.color(QPalette::Window);
    // Consider it dark mode if the window color luminance is below 128
    return (windowColor.red() * 0.299 + windowColor.green() * 0.587 + windowColor.blue() * 0.114) < 128;
#endif
}

QString getLightModeStyle() {
    return R"(
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
       border: 1px solid #CBD5E0;
            border-radius: 6px;
    margin-top: 22px;
        font-weight: bold;
            color: #2B6CB0;
      }
      QGroupBox::title {
     subcontrol-origin: margin;
            left: 10px;
   padding: 0 5px;
       background-color: #FFFFFF; 
        }

        /* BOUTONS */
    QPushButton {
            background-color: #F7FAFC; border: 1px solid #CBD5E0;
            border-radius: 5px; padding: 5px 15px;
  min-height: 25px;
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
        QComboBox, QSpinBox, QLineEdit {
            background-color: #FFFFFF; border: 1px solid #CBD5E0;
            border-radius: 5px; padding: 4px; min-height: 22px;
      color: #2D3748;
        }
 QComboBox:hover, QSpinBox:hover, QLineEdit:hover {
         border-color: #A0AEC0;
        }
  QComboBox::drop-down {
      border: none;
      padding-right: 4px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
   border: 1px solid #CBD5E0;
     selection-background-color: #EBF8FF;
   selection-color: #2B6CB0;
        }
      
      /* LABELS */
        QLabel {
  color: #2D3748;
     }
      
        /* SLIDERS */
        QSlider::groove:horizontal {
            background: #E2E8F0;
     height: 6px;
    border-radius: 3px;
        }
        QSlider::handle:horizontal {
    background: #3182CE;
    width: 14px;
   margin: -4px 0;
            border-radius: 7px;
      }
 QSlider::handle:horizontal:hover {
            background: #2B6CB0;
     }
        
        /* LIST WIDGET */
        QListWidget {
background-color: #FFFFFF;
       border: 1px solid #CBD5E0;
            border-radius: 5px;
       color: #2D3748;
        }
        QListWidget::item {
            padding: 4px;
     }
        QListWidget::item:selected {
            background-color: #EBF8FF;
       color: #2B6CB0;
    }
        QListWidget::item:hover {
       background-color: #F7FAFC;
        }
        
        /* TABLEAU */
        QTableWidget {
            border: none; gridline-color: #E2E8F0;
background-color: #FFFFFF;
   color: #2D3748;
        }
      QHeaderView::section {
      background-color: #F7FAFC; padding: 4px; border: none; font-weight: bold;
     color: #2D3748;
        }
  
        /* SCROLL AREA */
        QScrollArea {
  background: transparent;
        border: none;
        }
      QScrollBar:vertical {
            background: #F7FAFC;
            width: 10px;
            border-radius: 5px;
     }
     QScrollBar::handle:vertical {
 background: #CBD5E0;
          border-radius: 5px;
      min-height: 20px;
        }
     QScrollBar::handle:vertical:hover {
    background: #A0AEC0;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
     height: 0px;
        }
        
        /* MENU BAR */
        QMenuBar {
 background-color: #F7FAFC;
       color: #2D3748;
        }
        QMenuBar::item:selected {
         background-color: #EDF2F7;
  }
      QMenu {
     background-color: #FFFFFF;
        border: 1px solid #CBD5E0;
     color: #2D3748;
        }
  QMenu::item:selected {
    background-color: #EBF8FF;
            color: #2B6CB0;
        }
    )";
}

QString getDarkModeStyle() {
    return R"(
        /* FOND APP - Dark */
        QMainWindow { background-color: #1A202C; }
     
 /* TITRE APP */
        QLabel#appTitle {
    font-family: 'Segoe UI', sans-serif;
        font-size: 24px; font-weight: bold; color: #90CDF4;
            padding: 5px; margin-bottom: 0px;
   }

        /* PANNEAUX (Base) - Dark */
        QWidget#borderedPanel {
            background-color: #2D3748;
            border: 2px solid #4A5568; 
      border-radius: 8px;
        }

        /* FONT GLOBALE */
        QWidget {
            font-family: 'Segoe UI', sans-serif; font-size: 10pt; color: #E2E8F0;
        }

        /* --- GROUP BOX --- */
        QGroupBox {
  border: 1px solid #4A5568;
 border-radius: 6px;
  margin-top: 22px;
    font-weight: bold;
     color: #63B3ED;
        }
        QGroupBox::title {
       subcontrol-origin: margin;
     left: 10px;
         padding: 0 5px;
       background-color: #2D3748; 
        }

        /* BOUTONS - Dark */
        QPushButton {
         background-color: #4A5568; border: 1px solid #718096;
       border-radius: 5px; padding: 5px 15px;
    min-height: 25px;
          color: #E2E8F0;
    }
        QPushButton:hover { background-color: #718096; border-color: #A0AEC0; }
        QPushButton:pressed { background-color: #2D3748; }
        QPushButton:disabled { 
        background-color: #2D3748; 
color: #718096;
     border-color: #4A5568;
        }

        /* PLAY BUTTON */
        QPushButton#playButton {
  background-color: #3182CE; color: white; border: 1px solid #2B6CB0;
  }
        QPushButton#playButton:hover { background-color: #4299E1; }

        /* OUTILS SELECTIONNÉS (Checkable) */
        QPushButton:checked {
          background-color: #2C5282; border-color: #63B3ED; color: #90CDF4;
        }

        /* INPUTS - Dark */
  QComboBox, QSpinBox, QLineEdit {
            background-color: #4A5568; border: 1px solid #718096;
   border-radius: 5px; padding: 4px; min-height: 22px;
            color: #E2E8F0;
   }
        QComboBox:hover, QSpinBox:hover, QLineEdit:hover {
            border-color: #A0AEC0;
        }
        QComboBox::drop-down {
   border: none;
            padding-right: 4px;
        }
        QComboBox QAbstractItemView {
            background-color: #4A5568;
  border: 1px solid #718096;
  selection-background-color: #2C5282;
         selection-color: #90CDF4;
            color: #E2E8F0;
        }

        /* LABELS */
        QLabel {
      color: #E2E8F0;
        }
        
        /* SLIDERS */
   QSlider::groove:horizontal {
            background: #4A5568;
            height: 6px;
 border-radius: 3px;
        }
        QSlider::handle:horizontal {
    background: #63B3ED;
            width: 14px;
            margin: -4px 0;
        border-radius: 7px;
        }
   QSlider::handle:horizontal:hover {
            background: #90CDF4;
        }
   
        /* LIST WIDGET - Dark */
QListWidget {
            background-color: #4A5568;
       border: 1px solid #718096;
            border-radius: 5px;
          color: #E2E8F0;
        }
        QListWidget::item {
    padding: 4px;
      }
     QListWidget::item:selected {
    background-color: #2C5282;
         color: #90CDF4;
        }
        QListWidget::item:hover {
        background-color: #718096;
    }
        
        /* TABLEAU - Dark */
        QTableWidget {
    border: none; gridline-color: #4A5568;
            background-color: #2D3748;
     color: #E2E8F0;
     }
        QHeaderView::section {
            background-color: #4A5568; padding: 4px; border: none; font-weight: bold;
         color: #E2E8F0;
        }
    
 /* SCROLL AREA - Dark */
        QScrollArea {
background: transparent;
            border: none;
    }
  QScrollBar:vertical {
            background: #2D3748;
     width: 10px;
            border-radius: 5px;
        }
     QScrollBar::handle:vertical {
         background: #4A5568;
     border-radius: 5px;
       min-height: 20px;
      }
        QScrollBar::handle:vertical:hover {
        background: #718096;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
  
        /* MENU BAR - Dark */
    QMenuBar {
      background-color: #2D3748;
            color: #E2E8F0;
     }
        QMenuBar::item:selected {
            background-color: #4A5568;
 }
        QMenu {
  background-color: #2D3748;
            border: 1px solid #4A5568;
            color: #E2E8F0;
        }
   QMenu::item:selected {
            background-color: #2C5282;
         color: #90CDF4;
}
        
        /* MESSAGE BOX - Dark */
        QMessageBox {
     background-color: #2D3748;
            color: #E2E8F0;
   }
        QMessageBox QLabel {
   color: #E2E8F0;
    }

      /* INPUT DIALOG - Dark */
        QInputDialog {
     background-color: #2D3748;
   color: #E2E8F0;
      }
        QInputDialog QLabel {
            color: #E2E8F0;
        }
)";
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "Initializing QApplication..." << std::endl;
        qputenv("QTFRAMEWORK_BYPASS_LICENSE_CHECK", "1");
  QApplication app(argc, argv);

      // Detect system theme and apply appropriate style
        QString style;
     if (isDarkMode()) {
   std::cout << "Dark mode detected, applying dark theme..." << std::endl;
            style = getDarkModeStyle();
        } else {
            std::cout << "Light mode detected, applying light theme..." << std::endl;
            style = getLightModeStyle();
        }

        app.setStyleSheet(style);

        std::cout << "Creating MainWindow..." << std::endl;
      MainWindow window;
        
        std::cout << "Showing MainWindow..." << std::endl;
 window.showMaximized(); 
      
        window.raise();
        window.activateWindow();
   
   std::cout << "Starting event loop..." << std::endl;
  
        return app.exec();
    }
    
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
     return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return 1;
    }
}