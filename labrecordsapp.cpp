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
    filterLineEdit->setPlaceholderText("Введите текст для фильтрации...");


    tableWidget = new QTableWidget(0, 7, this);
    tableWidget->setHorizontalHeaderLabels({"Студент", "Номер группы", "Номер курса", "Лаб. работа", "Срок сдачи", "Оценка", "Дата выдачи"});
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

    setCentralWidget(centralWidget);
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
    bool ok;
    QString surname = QInputDialog::getText(this, "Поиск по фамилии",
                                            "Введите фамилию студента:",
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
    QString group = QInputDialog::getText(this, "Поиск по группе",
                                          "Введите номер группы:",
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
    int course = QInputDialog::getInt(this, "Хорошие оценки по курсу",
                                      "Введите номер курса:",
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
        QMessageBox::information(this, "Результат",
                                 QString("Максимальное время выполнения: %1 дней").arg(maxDays));
    }
}
