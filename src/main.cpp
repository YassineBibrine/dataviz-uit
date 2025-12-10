#include <QApplication>
#include "ui/main_window.h"
#include <iostream>
#include <exception>
#include <QDebug>

int main(int argc, char* argv[]) {
    try {
        std::cout << "Initializing QApplication..." << std::endl;
        QApplication app(argc, argv);
        
        std::cout << "Creating MainWindow..." << std::endl;
        MainWindow window;
        
        std::cout << "Showing MainWindow..." << std::endl;
        window.show();
        window.raise();
        window.activateWindow();
        
        std::cout << "Window geometry: " << window.geometry().x() << ", " 
         << window.geometry().y() << ", "
      << window.geometry().width() << "x" 
       << window.geometry().height() << std::endl;
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
