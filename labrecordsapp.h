#ifndef LABRECORDSAPP_H
#define LABRECORDSAPP_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>

class LabRecordsApp : public QMainWindow {
    Q_OBJECT

public:
    explicit LabRecordsApp(QWidget *parent = nullptr);

private slots:
    void addRecord();
    void editRecord();
    void deleteRecord();
    void saveToFile();
    void loadFromFile();
    void zaprosLabPoSurname();
    void zaprosLabPoGroup();
    void zaprosLabMoreTwoPerDay();
    void zaprosPoCourseWithGoodMark();
    void zaprosLabNotDoneTwoMonths();
    void zaprosLongestLabToDo();
    void filterRecords();

private:
    QTableWidget *tableWidget;
    QLineEdit *filterLineEdit;
};

#endif
