#ifndef PERF_GRAPH_H
#define PERF_GRAPH_H

#include <QDialog>

namespace Ui {
class perf_graph;
}

class perf_graph : public QDialog
{
    Q_OBJECT

public:
    explicit perf_graph(int mode, QWidget *parent = 0);
    ~perf_graph();

private:
    Ui::perf_graph *ui;
    float maxFPS;
    float localMaxFPS = 0;
    float mCurMaxFPS = 0;
    int mNumPods = 0;
    int mLastPod = 0;
    int mMode = 0;

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
     void hideFPS();
};

#endif // PERF_GRAPH_H
