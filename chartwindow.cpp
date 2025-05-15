#include "chartwindow.h"
#include <QtCharts>
#include <QVBoxLayout>

ChartWindow::ChartWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Анализ лабораторных работ");
    resize(800, 600);
}

void ChartWindow::setData(const QList<QPair<QString, int>> &courseCounts, const QString &selectedCourse) {
    this->courseData = courseCounts;
    this->selectedCourse = selectedCourse;

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Первая диаграмма: распределение по всем курсам
    QPieSeries *allCoursesSeries = new QPieSeries();
    for (const auto &entry : courseData) {
        allCoursesSeries->append(entry.first, entry.second);
    }
    QChart *allCoursesChart = new QChart();
    allCoursesChart->addSeries(allCoursesSeries);
    allCoursesChart->setTitle("Распределение лабораторных по курсам");

    QChartView *allCoursesView = new QChartView(allCoursesChart);
    allCoursesView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(allCoursesView);

    // Вторая диаграмма: выбранный курс против всех
    int selectedCount = 0;
    int total = 0;
    for (const auto &entry : courseData) {
        total += entry.second;
        if (entry.first == selectedCourse) {
            selectedCount = entry.second;
        }
    }
    int others = total - selectedCount;

    QPieSeries *selectedCourseSeries = new QPieSeries();
    selectedCourseSeries->append(selectedCourse, selectedCount);
    selectedCourseSeries->append("Остальные курсы", others);
    QPieSlice *selectedSlice = selectedCourseSeries->slices().at(0);
    selectedSlice->setBrush(Qt::green);
    selectedSlice->setExploded(true);
    selectedSlice->setLabelVisible(true);

    QChart *selectedChart = new QChart();
    selectedChart->addSeries(selectedCourseSeries);
    selectedChart->setTitle("Доля лабораторных по выбранному курсу");

    QChartView *selectedChartView = new QChartView(selectedChart);
    selectedChartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(selectedChartView);
}
