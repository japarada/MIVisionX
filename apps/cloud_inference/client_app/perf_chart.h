#ifndef PERF_CHART_H
#define PERF_CHART_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <vector>
#include <qcustomplot.h>

#if defined(ENABLE_KUBERNETES_MODE)
static const Qt::GlobalColor colors[4] = {Qt::darkRed, Qt::darkBlue, Qt::darkYellow, Qt::darkGreen};
#endif

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
    double mRangeX;
    double mRangeY;
    int mNumGPUs;
#if defined(ENABLE_KUBERNETES_MODE)
    int mNumPods;
    int mTempPod = 0;
    int mChangedCount = 0;
    int mLastPod = 0;
    int mCurGraph = 0;
    std::vector<std::tuple<QCPItemText *, double, double>> mLabels;
#endif

public slots:
    void initGraph();
    void RealtimeDataSlot();
    void updateFPSValue(int fpsValue);
    void rescaleAxis(double key);
    void closeChartView();
    void setGPUs(int numGPUs);
#if defined(ENABLE_KUBERNETES_MODE)
    void setPods(int numPods);
    void fixLabelLocation();
    void changePods(double key, double value);
    void coloredGraph();
#endif
};
#endif // PERF_CHART_H
