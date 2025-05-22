#include "labrecordsapp.h"
#include "umldiagramwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QDate>
#include <QComboBox>
#include <QLabel>
#include "chartwindow.h"

LabRecordsApp::LabRecordsApp(QWidget *parent) : QMainWindow(parent) {
    
    // Главный виджет и горизонтальный layout
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget); // Основной горизонтальный layout

    // Создаем таблицу (будет слева)
    tableWidget = new QTableWidget(0, 7, this);
    tableWidget->setHorizontalHeaderLabels({"Студент", "Номер группы", "Дисциплина",
                                            "Лаб. работа", "Срок сдачи", "Оценка", "Дата выдачи"});

    // Настраиваем растягивание таблицы
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    for (int col = 0; col < tableWidget->columnCount(); ++col) {
        tableWidget->setColumnWidth(col, 200);
    }

    // Создаем вертикальный layout для кнопок (будет справа)
    auto *buttonsLayout = new QVBoxLayout();
    buttonsLayout->setAlignment(Qt::AlignTop); // Выравнивание кнопок по верху

    // Создаем кнопки
    QPushButton *addButton = new QPushButton("Добавить запись");
    QPushButton *editButton = new QPushButton("Редактировать запись");
    QPushButton *searchButton = new QPushButton("Поиск");
    QPushButton *deleteButton = new QPushButton("Удалить запись");
    QPushButton *saveButton = new QPushButton("Сохранить в файл");
    QPushButton *loadButton = new QPushButton("Загрузить из файла");
    QPushButton *showUmlButton = new QPushButton("Показать UML");
    QPushButton *resetButton = new QPushButton("Сбросить фильтр");
    QPushButton *filterButton = new QPushButton("Фильтрация");
    QPushButton *queriesMenuButton = new QPushButton("Запросы");
    QMenu *queriesMenu = new QMenu(this);
    
    queriesMenu->addAction("По фамилии", this, &LabRecordsApp::zaprosLabPoSurname);
    queriesMenu->addAction("По группе", this, &LabRecordsApp::zaprosLabPoGroup);
    queriesMenu->addAction(">2 лаб в день", this, &LabRecordsApp::zaprosLabMoreTwoPerDay);
    queriesMenu->addAction("Хорошие оценки", this, &LabRecordsApp::zaprosPoCourseWithGoodMark);
    queriesMenu->addAction("Просрочка >2 месяцев", this, &LabRecordsApp::zaprosLabNotDoneTwoMonths);
    queriesMenu->addAction("Самое долгое выполнение", this, &LabRecordsApp::zaprosLongestLabToDo);
    
    queriesMenuButton->setMenu(queriesMenu);
    
    // Устанавливаем фиксированный размер кнопок
    const QSize buttonSize(300, 40);
    addButton->setFixedSize(buttonSize);
    editButton->setFixedSize(buttonSize);
    searchButton->setFixedSize(buttonSize);
    deleteButton->setFixedSize(buttonSize);
    saveButton->setFixedSize(buttonSize);
    loadButton->setFixedSize(buttonSize);
    resetButton->setFixedSize(buttonSize);
    filterButton->setFixedSize(buttonSize);
    queriesMenuButton->setFixedSize(buttonSize);
    showUmlButton->setFixedSize(buttonSize);


    // Добавляем кнопки в правый layout
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(editButton);
    buttonsLayout->addWidget(searchButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(loadButton);
    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addWidget(filterButton);
    buttonsLayout->addWidget(queriesMenuButton);
    buttonsLayout->addWidget(showUmlButton);
    buttonsLayout->addStretch(); // Добавляем растягивающийся элемент внизу


    // Добавляем таблицу и кнопки в главный layout
    mainLayout->addWidget(tableWidget, 1); // Таблица будет растягиваться
    mainLayout->addLayout(buttonsLayout);  // Кнопки справа

    // Добавим нижнюю панель анализа по курсу
    auto *analysisLayout = new QVBoxLayout();

    // Заполняем список уникальных курсов из таблицы
    QSet<QString> courseSet;
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = tableWidget->item(row, 2); // Колонка "Номер курса"
        if (item) {
            courseSet.insert(item->text());
        }
    }

    courseComboBox = new QComboBox(this);
    courseComboBox->addItem("Выберите курс");
    for (const QString &course : courseSet) {
        courseComboBox->addItem(course);
    }

    analyzeButton = new QPushButton("Показать диаграммы", this);
    analyzeButton->setFixedSize(buttonSize);

    buttonsLayout->addWidget(new QLabel("Анализ по курсу:"));
    buttonsLayout->addWidget(courseComboBox);
    buttonsLayout->addWidget(analyzeButton);

    // Обработчик кнопки анализа
    connect(analyzeButton, &QPushButton::clicked, this, [this]() {
        QString selected = courseComboBox->currentText();
        if (selected == "Выберите курс") {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите курс.");
            return;
        }
        showChartsForCourse(selected);
    });

    // Устанавливаем центральный виджет
    this->setCentralWidget(centralWidget);

    // Подключаем сигналы кнопок
    connect(addButton, &QPushButton::clicked, this, &LabRecordsApp::addRecord);
    connect(editButton, &QPushButton::clicked, this, &LabRecordsApp::editRecord);
    connect(searchButton, &QPushButton::clicked, this, &LabRecordsApp::searchRecord);
    connect(deleteButton, &QPushButton::clicked, this, &LabRecordsApp::deleteRecord);
    connect(saveButton, &QPushButton::clicked, this, &LabRecordsApp::saveToFile);
    connect(loadButton, &QPushButton::clicked, this, &LabRecordsApp::loadFromFile);
    connect(resetButton, &QPushButton::clicked, this, &LabRecordsApp::resetFilters);
    connect(filterButton, &QPushButton::clicked, this, &LabRecordsApp::filterRecords);
    connect(showUmlButton, &QPushButton::clicked, this, &LabRecordsApp::showUMLDiagram);
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
    // Обновляем список дисциплин в выпадающем списке
    if (courseComboBox) {
        QSet<QString> currentItems;
        for (int i = 1; i < courseComboBox->count(); ++i) {
            currentItems.insert(courseComboBox->itemText(i));
        }

        QString newSubject = tableWidget->item(tableWidget->rowCount() - 1, 2)->text(); // колонка "Дисциплина"

        if (!currentItems.contains(newSubject)) {
            courseComboBox->addItem(newSubject);
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

void LabRecordsApp::searchRecord() {
    bool ok;
    QString query = QInputDialog::getText(this, "Поиск", "Введите текст для поиска:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || query.isEmpty()) return;

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item && item->text().contains(query, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        tableWidget->setRowHidden(row, !match);
    }
}

void LabRecordsApp::deleteRecord() {
    int row = tableWidget->currentRow();
    if (row >= 0) {
        tableWidget->removeRow(row);
    }
}

void LabRecordsApp::saveToFile() {
    QString filename = QFileDialog::getSaveFileName(this, "Экспорт данных","Введите название файла с указанитем типа файла");
    if (filename.isEmpty()) {
        return;
    }

    QFile file(filename);
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
    QString FileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (FileName.isEmpty()) {
        return;
    }

    QFile file(FileName);
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
        // Обновляем список курсов после загрузки
        courseComboBox->clear();
        courseComboBox->addItem("Выберите дисциплину");

        QSet<QString> uniqueSubjects;
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QTableWidgetItem *item = tableWidget->item(row, 2); // колонка "Дисциплина"
            if (item) {
                uniqueSubjects.insert(item->text());
            }
        }
        for (const QString &subject : uniqueSubjects) {
            courseComboBox->addItem(subject);
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Ошибка при загрузке данных из файла");
    }
}

void LabRecordsApp::resetFilters() {
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        tableWidget->showRow(row);
    }
}

void LabRecordsApp::showUMLDiagram() {
    UmlDiagramWindow *umlWindow = new UmlDiagramWindow(this);
    umlWindow->show();
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
    QString subject = QInputDialog::getText(this, "Хорошие оценки по предмету",
                                            "Введите название предмета:", QLineEdit::Normal, "", &ok);
    if (!ok || subject.isEmpty()) return;

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QString subjectStr = tableWidget->item(row, 2)->text(); // дисциплина
        QString markStr = tableWidget->item(row, 5)->text();     // оценка

        bool isInt;
        int mark = markStr.toInt(&isInt);

        bool show = (subjectStr.compare(subject, Qt::CaseInsensitive) == 0) && isInt && mark >= 4;
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

void LabRecordsApp::showChartsForCourse(const QString &selectedSubject) {
    QMap<QString, int> courseCounts;
    int total = 0;

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem *courseItem = tableWidget->item(row, 2);
        if (courseItem) {
            QString course = courseItem->text();
            courseCounts[course]++;
            total++;
        }
    }

    QList<QPair<QString, int>> courseData;
    for (auto it = courseCounts.begin(); it != courseCounts.end(); ++it) {
        courseData.append(qMakePair(it.key(), it.value()));
    }

    ChartWindow *chart = new ChartWindow(this);
    chart->setData(courseData, selectedSubject);
    chart->exec();
}

void LabRecordsApp::filterRecords() {
    if (!tableWidget) return;

    if (tableWidget->isHidden()) {
        tableWidget->setVisible(true);
        return;
    }

    if (tableWidget->rowCount() == 0) {
        QMessageBox::warning(this, "Ошибка", "Нет записей для фильтрации!");
        return;
    }

    QString studentName = QInputDialog::getText(this, "Фильтрация по студенту",
                                                "Введите фамилию студента:",
                                                QLineEdit::Normal);

    if (studentName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не введено имя студента!");
        return;
    }

    QTableWidget *filteredTable = new QTableWidget(this);
    filteredTable->setColumnCount(tableWidget->columnCount());
    filteredTable->setHorizontalHeaderLabels(
        {"Студент", "Номер группы", "Дисциплина", "Лаб. работа",
         "Срок сдачи", "Оценка", "Дата выдачи"});

    int filteredRow = 0;
    bool found = false;

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = tableWidget->item(row, 0);

        if (item && item->text().contains(studentName, Qt::CaseInsensitive)) {
            found = true;
            filteredTable->insertRow(filteredRow);

            for (int col = 0; col < tableWidget->columnCount(); ++col) {
                QTableWidgetItem *originalItem = tableWidget->item(row, col);
                if (originalItem) {
                    filteredTable->setItem(filteredRow, col, new QTableWidgetItem(*originalItem));
                }
            }
            filteredRow++;
        }
    }

    if (!found) {
        QMessageBox::information(this, "Результат",
                                 QString("Не найдено записей для студента: %1").arg(studentName));
        delete filteredTable;
        return;
    }

    filteredTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    filteredTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    filteredTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Заменим текущую таблицу новой
    layout()->removeWidget(tableWidget);
    tableWidget->setVisible(false);
    layout()->addWidget(filteredTable);

}
