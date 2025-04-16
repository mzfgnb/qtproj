#include "labrecordsapp.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>

LabRecordsApp::LabRecordsApp(QWidget *parent) : QMainWindow(parent) {
    auto *centralWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(centralWidget);

    filterLineEdit = new QLineEdit(this);
    filterLineEdit->setPlaceholderText("Введите текст для фильтрации...");
    layout->addWidget(filterLineEdit);

    tableWidget = new QTableWidget(0, 7, this);
    tableWidget->setHorizontalHeaderLabels({"Студент", "Номер группы", "Номер курса", "Лаб. работа", "Дата сдачи", "Оценка", "Срок сдачи"});
    tableWidget->setSortingEnabled(true);
    layout->addWidget(tableWidget);

    QPushButton *addButton = new QPushButton("Добавить запись", this);
    QPushButton *editButton = new QPushButton("Редактировать запись", this);
    QPushButton *deleteButton = new QPushButton("Удалить запись", this);
    QPushButton *saveButton = new QPushButton("Сохранить в файл", this);
    QPushButton *loadButton = new QPushButton("Загрузить из файла", this);
    QPushButton *zaprosLabPoSurnameButton = new QPushButton("Запрос по фамилии", this);
    QPushButton *zaprosLabPoGroupButton = new QPushButton("Запрос по группе", this);
    QPushButton *zaprosLabMoreTwoPerDayButton = new QPushButton("Запрос по >2 лаб/день", this);
    QPushButton *zaprosPoCourseWithGoodMarkButton = new QPushButton("Запрос по хорошим оценкам", this);
    QPushButton *zaprosLabNotDoneTwoMonthsButton = new QPushButton("Запрос по просрочке в 2 месяца", this);
    QPushButton *zaprosLongestLabToDoButton = new QPushButton("Запрос по самому долгому выполнению", this);

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
    connect(filterLineEdit, &QLineEdit::textChanged, this, &LabRecordsApp::filterRecords);

    setCentralWidget(centralWidget);
}

void LabRecordsApp::filterRecords() {
    QString filterText = filterLineEdit->text();

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item && item->text().contains(filterText, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        tableWidget->setRowHidden(row, !match);
    }
}

void LabRecordsApp::addRecord() {
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    for (int col = 0; col < 7; ++col) {
        bool ok;
        QString value = QInputDialog::getText(this, "Ввести запись", "Введенная запись:", QLineEdit::Normal, "", &ok);
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
            QString value = QInputDialog::getText(this, "Редактировать данные", "Ввести новые данные:", QLineEdit::Normal, item->text(), &ok);
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
        QMessageBox::warning(this, "Ошибка", "Произошла ошибка при сохранении в файл");
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
        QMessageBox::warning(this, "Ошибка", "Ошибка при загрузке данных из файла");
    }
}

void LabRecordsApp::zaprosLabPoSurname() {
    // Реализация запроса по фамилии
}

void LabRecordsApp::zaprosLabPoGroup() {
    // Реализация запроса по группе
}

void LabRecordsApp::zaprosLabMoreTwoPerDay() {
    // Реализация запроса по >2 лаб в день
}

void LabRecordsApp::zaprosPoCourseWithGoodMark() {
    // Реализация запроса по хорошим оценкам
}

void LabRecordsApp::zaprosLabNotDoneTwoMonths() {
    // Реализация запроса по просрочке в 2 месяца
}

void LabRecordsApp::zaprosLongestLabToDo() {
    // Реализация запроса по самому долгому выполнению
}