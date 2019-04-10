#ifndef PERF_CHART_H
#define PERF_CHART_H

#include <QDialog>

namespace Ui {
class perf_chart;
}

class perf_chart : public QDialog
{
    Q_OBJECT

public:
    explicit perf_chart(QWidget *parent = 0);
    ~perf_chart();

private:
    Ui::perf_chart *ui;
    float maxFPS;

public slots:
     void closePerformanceView();
     void resetPerformanceView();
     void setModelName(QString ModelName);
     void setNumGPU(int numGPU);
     void setTotalGPU(int totalGPU);
     void setPods(int numPods);
     void setStartTime(QString startTime);
     void updateElapsedTime(QString elapsedTime);
     void updateFPSValue(float fps);
     void updateTotalImagesValue(int images);
};

#endif // PERF_CHART_H
