#ifndef LABRECORDSAPP_H
#define LABRECORDSAPP_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>

class LabRecordsApp : public QMainWindow {
    Q_OBJECT
public:
    LabRecordsApp(QWidget *parent = nullptr) : QMainWindow(parent) {
        auto *centralWidget = new QWidget(this);
        auto *layout = new QVBoxLayout(centralWidget);
        
        tableWidget = new QTableWidget(0, 7, this);
        tableWidget->setHorizontalHeaderLabels({"Student", "Group", "Course", "Lab Work", "Due Date", "Mark", "Issue Date"});
        layout->addWidget(tableWidget);
        
        QPushButton *addButton = new QPushButton("Add Record", this);
        QPushButton *editButton = new QPushButton("Edit Record", this);
        QPushButton *deleteButton = new QPushButton("Delete Record", this);
        QPushButton *saveButton = new QPushButton("Save to File", this);
        QPushButton *loadButton = new QPushButton("Load from File", this);
        
        layout->addWidget(addButton);
        layout->addWidget(editButton);
        layout->addWidget(deleteButton);
        layout->addWidget(saveButton);
        layout->addWidget(loadButton);
        
        connect(addButton, &QPushButton::clicked, this, &LabRecordsApp::addRecord);
        connect(editButton, &QPushButton::clicked, this, &LabRecordsApp::editRecord);
        connect(deleteButton, &QPushButton::clicked, this, &LabRecordsApp::deleteRecord);
        connect(saveButton, &QPushButton::clicked, this, &LabRecordsApp::saveToFile);
        connect(loadButton, &QPushButton::clicked, this, &LabRecordsApp::loadFromFile);
        
        setCentralWidget(centralWidget);
    }

private slots:
    void addRecord() {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            bool ok;
            QString value = QInputDialog::getText(this, "Enter Data", "Enter value:", QLineEdit::Normal, "", &ok);
            if (ok && !value.isEmpty()) {
                tableWidget->setItem(row, col, new QTableWidgetItem(value));
            } else {
                tableWidget->setItem(row, col, new QTableWidgetItem(""));
            }
        }
    }
    
    void editRecord() {
        int row = tableWidget->currentRow();
        if (row < 0) return;
        for (int col = 0; col < 7; ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item) {
                bool ok;
                QString value = QInputDialog::getText(this, "Edit Data", "Enter new value:", QLineEdit::Normal, item->text(), &ok);
                if (ok && !value.isEmpty()) {
                    item->setText(value);
                }
            }
        }
    }
    
    void deleteRecord() {
        int row = tableWidget->currentRow();
        if (row >= 0) {
            tableWidget->removeRow(row);
        }
    }
    
    void saveToFile() {
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
            QMessageBox::warning(this, "Error", "Failed to save data to file.");
        }
    }
    
    void loadFromFile() {
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
            QMessageBox::warning(this, "Error", "Failed to load data from file.");
        }
    }

private:
    QTableWidget *tableWidget;
};

#endif
