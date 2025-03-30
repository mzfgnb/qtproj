#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include "labrecordsapp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LabRecordsApp window;
    window.show();
    return app.exec();
}

