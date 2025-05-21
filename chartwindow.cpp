#include "chartwindow.h"
#include <QtCharts>
#include <QVBoxLayout>

ChartWindow::ChartWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Анализ лабораторных работ");
    resize(800, 600);
}

/*void ChartWindow::setData(const QList<QPair<QString, int>> &courseCounts, const QString &selectedCourse) {
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
} */

void ChartWindow::setData(const QList<QPair<QString, int>> &data, const QString &selectedCourse) {
    int total = 0;
    int selectedCount = 0;

    QPieSeries *seriesAll = new QPieSeries();
    for (const auto &pair : data) {
        seriesAll->append(pair.first, pair.second);
        total += pair.second;
        if (pair.first == selectedCourse) {
            selectedCount = pair.second;
        }
    }

    QPieSeries *seriesCourse = new QPieSeries();
    seriesCourse->append(selectedCourse, selectedCount);
    seriesCourse->append("Остальные", total - selectedCount);

    QChart *chart1 = new QChart();
    chart1->addSeries(seriesAll);
    chart1->setTitle("Распределение лабораторных по курсам");

    QChart *chart2 = new QChart();
    chart2->addSeries(seriesCourse);
    chart2->setTitle("Доля лабораторных по выбранному курсу");

    QChartView *view1 = new QChartView(chart1);
    QChartView *view2 = new QChartView(chart2);

    auto *layout = qobject_cast<QVBoxLayout *>(this->layout());
    layout->addWidget(view1);
    layout->addWidget(view2);
}

