#ifndef PERF_CHART_H
#define PERF_CHART_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>

namespace Ui {
class perf_chart;
}

class perf_chart : public QMainWindow
{
    Q_OBJECT

public:
    explicit perf_chart(QWidget *parent = 0);
    ~perf_chart();

private:
    Ui::perf_chart *ui;
    QTimer timer;
    float maxFPS;
    void initGraph();

//     void closePerformanceView();
//     void resetPerformanceView();
//     void setModelName(QString ModelName);
//     void setNumGPU(int numGPU);
//     void setTotalGPU(int totalGPU);
//     void setPods(int numPods);
//     void setStartTime(QString startTime);
//     void updateElapsedTime(QString elapsedTime);
//     void updateFPSValue(float fps);
//     void updateTotalImagesValue(int images);
};

#endif // PERF_CHART_H
