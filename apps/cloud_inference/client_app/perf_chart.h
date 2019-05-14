#ifndef PERF_CHART_H
#define PERF_CHART_H

#include <QMainWindow>
#include <QTimer>
#include <QtWidgets>
#include <vector>
#include <qcustomplot.h>

static const Qt::GlobalColor colors[4] = {Qt::blue, Qt::green, Qt::red, Qt::yellow};

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
    int mTemPods = 0;
    int mNumPods;
    int mLastPod = 0;
    int mCurGraph = 0;
    double mRangeX;
    double mRangeY;
    std::vector<std::tuple<QCPItemText *, double, double>> mLabels;

public slots:
    void initGraph();
    void RealtimeDataSlot();
    void updateFPSValue(int fpsValue);
    void setPods(int numPods);
    void rescaleAxis(double key);
    void fixLabelLocation();
    void changePods(double key, double value);
    void closeChartView();
    void coloredGraph();
};
#endif // PERF_CHART_H
