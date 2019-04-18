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
    int mMaxFPS = 0;
    int mFPSValue;
    int mLastPod;
    void initGraph();

public slots:
    void RealtimeDataSlot();
    void updateFPSValue(int fpsValue);
    void setPods(int numPods);
    void closeChartView();
};
#endif // PERF_CHART_H
