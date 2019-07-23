#ifndef PERF_CHART_H
#define PERF_CHART_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <vector>
#include <qcustomplot.h>
#if defined(ENABLE_KUBERNETES_MODE)
#include "perf_bar.h"
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
    int mFPSValue = 0;
    double mRangeX;
    double mRangeY;
    int mNumGPUs;
#if defined(ENABLE_KUBERNETES_MODE)
    int mMode;
    int mNumPods = 0;
    int mTempPod = 0;
    int mDummyPods = 0;
    int mChangedCount = 0;
    int mLastPod = 0;
    int mCurGraph = 0;
    int mCurMax = 0;
    int localMaxFPS = 1250;
    int mThreshold = 320;
    std::vector<std::tuple<QCPItemText *, double, double, int>> mLabels;
    perf_bar *bar;
    QCPItemLine *mCurLine;
#endif

public slots:
    void initGraph();
    void setCPUName(QString cpuName);
    void setGPUName(QString gpuName);
    void setMode(int mode);
    void RealtimeDataSlot();
    void updateFPSValue(float fpsValue);
    void rescaleAxis(double key);
    void closeChartView();
    void setTotalGPUs(int numGPUs);
    void setGPUs(int numGPUs);
#if defined(ENABLE_KUBERNETES_MODE)
    void setPods(int numPods);
    void fixLabelLocation();
    void changePods(double key, double value);
    void coloredGraph();
    void barChart();
#endif
};
#endif // PERF_CHART_H
