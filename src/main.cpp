#include <QApplication>
#include "ui/main_window.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        QApplication app(argc, argv);
        
        MainWindow window;
        window.show();
        
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
