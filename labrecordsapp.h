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
    void searchRecord();
    void editRecord();
    void deleteRecord();
    void saveToFile();
    void loadFromFile();
    void resetFilters();
    void showUMLDiagram();
    void zaprosLabPoSurname();
    void zaprosLabPoGroup();
    void zaprosLabMoreTwoPerDay();
    void zaprosPoCourseWithGoodMark();
    void zaprosLabNotDoneTwoMonths();
    void zaprosLongestLabToDo();

private:
    QTableWidget *tableWidget;
    QLineEdit *filterLineEdit;
    QComboBox *courseComboBox;
    QPushButton *analyzeButton;
    void showChartsForCourse(const QString &selectedCourse);
};

#endif
