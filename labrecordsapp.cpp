#include "labrecordsapp.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDate>

LabRecordsApp::LabRecordsApp(QWidget *parent) : QMainWindow(parent) {
    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    filterLineEdit = new QLineEdit(this);
    filterLineEdit->setPlaceholderText("Enter text for filtering...");


    tableWidget = new QTableWidget(0, 7, this);
    tableWidget->setHorizontalHeaderLabels({"Student", "Group", "Course", "Lab Work", "Due Date", "Mark", "Issue Date"});
    layout->addWidget(tableWidget);

    QPushButton *addButton = new QPushButton("Add Record", this);
    QPushButton *editButton = new QPushButton("Edit Record", this);
    QPushButton *deleteButton = new QPushButton("Delete Record", this);
    QPushButton *saveButton = new QPushButton("Save to File", this);
    QPushButton *loadButton = new QPushButton("Load from File", this);
    QPushButton *zaprosLabPoSurnameButton = new QPushButton("Request by surname", this);
    QPushButton *zaprosLabPoGroupButton = new QPushButton("Request by group", this);
    QPushButton *zaprosLabMoreTwoPerDayButton = new QPushButton("Request by more 2 labs/day", this);
    QPushButton *zaprosPoCourseWithGoodMarkButton = new QPushButton("Request by good marks", this);
    QPushButton *zaprosLabNotDoneTwoMonthsButton = new QPushButton("Request by overdue in 2 months", this);
    QPushButton *zaprosLongestLabToDoButton = new QPushButton("Request by longest execution", this);

    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(saveButton);
    layout->addWidget(loadButton);
    layout->addWidget(zaprosLabPoSurnameButton);
    layout->addWidget(zaprosLabPoGroupButton);
    layout->addWidget(zaprosLabMoreTwoPerDayButton);
    layout->addWidget(zaprosPoCourseWithGoodMarkButton);
    layout->addWidget(zaprosLabNotDoneTwoMonthsButton);
    layout->addWidget(zaprosLongestLabToDoButton);

    connect(addButton, &QPushButton::clicked, this, &LabRecordsApp::addRecord);
    connect(editButton, &QPushButton::clicked, this, &LabRecordsApp::editRecord);
    connect(deleteButton, &QPushButton::clicked, this, &LabRecordsApp::deleteRecord);
    connect(saveButton, &QPushButton::clicked, this, &LabRecordsApp::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &LabRecordsApp::loadFromFile);
    connect(zaprosLabPoSurnameButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosLabPoSurname);
    connect(zaprosLabPoGroupButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosLabPoGroup);
    connect(zaprosLabMoreTwoPerDayButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosLabMoreTwoPerDay);
    connect(zaprosPoCourseWithGoodMarkButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosPoCourseWithGoodMark);
    connect(zaprosLabNotDoneTwoMonthsButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosLabNotDoneTwoMonths);
    connect(zaprosLongestLabToDoButton, &QPushButton::clicked, this, &LabRecordsApp::zaprosLongestLabToDo);

    setCentralWidget(centralWidget);
}


void LabRecordsApp::addRecord() {
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    for (int col = 0; col < 7; ++col) {
        bool ok;
        QString value = QInputDialog::getText(this, "Enter record", "Entered record:", QLineEdit::Normal, "", &ok);
        if (ok && !value.isEmpty()) {
            tableWidget->setItem(row, col, new QTableWidgetItem(value));
        } else {
            tableWidget->setItem(row, col, new QTableWidgetItem(""));
        }
    }
}

void LabRecordsApp::editRecord() {
    int row = tableWidget->currentRow();
    if (row < 0) return;
    for (int col = 0; col < 7; ++col) {
        QTableWidgetItem* item = tableWidget->item(row, col);
        if (item) {
            bool ok;
            QString value = QInputDialog::getText(this, "Edit record", "Enter new data:", QLineEdit::Normal, item->text(), &ok);
            if (ok && !value.isEmpty()) {
                item->setText(value);
            }
        }
    }
}

void LabRecordsApp::deleteRecord() {
    int row = tableWidget->currentRow();
    if (row >= 0) {
        tableWidget->removeRow(row);
    }
}

void LabRecordsApp::saveToFile() {
    QFile file("records.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            for (int col = 0; col < 7; ++col) {
                QTableWidgetItem* item = tableWidget->item(row, col);
                if (item) {
                    out << item->text() << (col < 6 ? "," : "\n");
                } else {
                    out << "" << (col < 6 ? "," : "\n");
                }
            }
        }
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "An error occured during saving the file.");
    }
}

void LabRecordsApp::loadFromFile() {
    QFile file("records.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        tableWidget->setRowCount(0);
        while (!in.atEnd()) {
            QStringList fields = in.readLine().split(",");
            if (fields.size() == 7) {
                int row = tableWidget->rowCount();
                tableWidget->insertRow(row);
                for (int col = 0; col < 7; ++col) {
                    tableWidget->setItem(row, col, new QTableWidgetItem(fields[col]));
                }
            }
        }
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "An error occured during saving the file.");
    }
}

void LabRecordsApp::zaprosLabPoSurname() {
    bool ok;
    QString surname = QInputDialog::getText(this, "Search by surname",
                                            "Enter student's surname:",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || surname.isEmpty()) return;

    // Сначала показываем все строки
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        tableWidget->showRow(i);
    }

    // Затем скрываем несоответствующие
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem* item = tableWidget->item(row, 0); // Колонка "Студент"
        if (!item || !item->text().contains(surname, Qt::CaseInsensitive)) {
            tableWidget->hideRow(row);
        }
    }
}

void LabRecordsApp::zaprosLabPoGroup() {
    bool ok;
    QString group = QInputDialog::getText(this, "Search by group",
                                          "Enter group number:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || group.isEmpty()) return;

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        tableWidget->showRow(i);
    }

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem* item = tableWidget->item(row, 1); // Колонка "Номер группы"
        if (!item || item->text() != group) {
            tableWidget->hideRow(row);
        }
    }
}

void LabRecordsApp::zaprosLabMoreTwoPerDay() {
    QMap<QString, QMap<QString, int>> countByStudentAndDate; // студент -> дата -> количество

    // Подсчет количества лаб по дням для каждого студента
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString student = tableWidget->item(row, 0)->text();
        QString date = tableWidget->item(row, 4)->text(); // Колонка "Дата сдачи"

        countByStudentAndDate[student][date]++;
    }

    // Показываем только те строки, где >2 лаб в день
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString student = tableWidget->item(row, 0)->text();
        QString date = tableWidget->item(row, 4)->text();

        tableWidget->setRowHidden(row, countByStudentAndDate[student][date] <= 2);
    }
}

void LabRecordsApp::zaprosPoCourseWithGoodMark() {
    bool ok;
    int course = QInputDialog::getInt(this, "Good marks in course",
                                      "Enter course number:",
                                      1, 1, 6, 1, &ok);
    if (!ok) return;

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString courseStr = tableWidget->item(row, 2)->text(); // Колонка "Номер курса"
        QString markStr = tableWidget->item(row, 5)->text();   // Колонка "Оценка"

        bool isInt;
        int mark = markStr.toInt(&isInt);

        bool show = (courseStr == QString::number(course)) && isInt && mark >= 4;
        tableWidget->setRowHidden(row, !show);
    }
}

void LabRecordsApp::zaprosLabNotDoneTwoMonths() {
    QDate currentDate = QDate::currentDate();

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString deadlineStr = tableWidget->item(row, 6)->text(); // Колонка "Срок сдачи"
        QString submitDateStr = tableWidget->item(row, 4)->text(); // Колонка "Дата сдачи"

        QDate deadline = QDate::fromString(deadlineStr, "dd.MM.yyyy");
        QDate submitDate = QDate::fromString(submitDateStr, "dd.MM.yyyy");

        if (!deadline.isValid() || !submitDate.isValid()) {
            tableWidget->setRowHidden(row, true);
            continue;
        }

        // Если работа сдана после дедлайна + 2 месяца
        bool isOverdue = (submitDate > deadline.addMonths(2));
        tableWidget->setRowHidden(row, !isOverdue);
    }
}

void LabRecordsApp::zaprosLongestLabToDo() {
    int maxDays = -1;
    QVector<int> longestRows;

    // Находим максимальное время выполнения
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString issueDateStr = tableWidget->item(row, 6)->text(); // Колонка "Срок сдачи"
        QString submitDateStr = tableWidget->item(row, 4)->text(); // Колонка "Дата сдачи"

        QDate issueDate = QDate::fromString(issueDateStr, "dd.MM.yyyy");
        QDate submitDate = QDate::fromString(submitDateStr, "dd.MM.yyyy");

        if (!issueDate.isValid() || !submitDate.isValid()) continue;

        int days = issueDate.daysTo(submitDate);
        if (days > maxDays) {
            maxDays = days;
            longestRows.clear();
            longestRows.append(row);
        } else if (days == maxDays) {
            longestRows.append(row);
        }
    }

    // Показываем только самые долгие
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        tableWidget->setRowHidden(row, !longestRows.contains(row));
    }

    if (maxDays >= 0) {
        QMessageBox::information(this, "Result",
                                 QString("Maximum execution time: %1 days").arg(maxDays));
    }
}
