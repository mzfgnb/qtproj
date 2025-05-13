#include "umldiagramwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>

UmlDiagramWindow::UmlDiagramWindow(QWidget *parent)
    : QDialog(parent) {
    this->setWindowTitle("UML Диаграмма");
    this->resize(600, 400);

    auto *layout = new QVBoxLayout(this);
    auto *view = new QGraphicsView(this);
    auto *scene = new QGraphicsScene(this);

    view->setScene(scene);
    layout->addWidget(view);

    // Класс LabRecordsApp
    QGraphicsRectItem *classBox = scene->addRect(50, 50, 200, 120);
    QGraphicsTextItem *text = scene->addText("LabRecordsApp\n-----------------------\n+ addRecord()\n+ editRecord()\n+ deleteRecord()");
    text->setPos(60, 60);

    // Родитель QMainWindow
    QGraphicsRectItem *baseBox = scene->addRect(300, 50, 200, 50);
    QGraphicsTextItem *baseText = scene->addText("QMainWindow");
    baseText->setPos(320, 60);

    // Линия наследования
    scene->addLine(250, 100, 300, 75, QPen(Qt::black, 2));
}
