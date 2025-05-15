#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QDialog>
#include <QList>
#include <QPair>
#include <QString>

class ChartWindow : public QDialog {
    Q_OBJECT

public:
    explicit ChartWindow(QWidget *parent = nullptr);
    void setData(const QList<QPair<QString, int>> &courseCounts, const QString &selectedCourse);

private:
    void createCourseDistributionChart();
    void createSelectedCourseHighlightChart();

    QList<QPair<QString, int>> courseData;
    QString selectedCourse;
};

#endif // CHARTWINDOW_H
