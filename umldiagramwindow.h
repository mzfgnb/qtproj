#pragma once
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

class UmlDiagramWindow : public QDialog {
    Q_OBJECT
public:
    explicit UmlDiagramWindow(QWidget *parent = nullptr);
};
