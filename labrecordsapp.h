#ifndef LABRECORDSAPP_H
#define LABRECORDSAPP_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

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
    void filterRecords();
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
    QTableWidget *filteredTable = nullptr;    
    QHBoxLayout *mainLayout = nullptr;
    void showChartsForCourse(const QString &subjectName);
};

#endif
