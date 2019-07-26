#ifndef INFERENCE_PANEL_H
#define INFERENCE_PANEL_H

#include <QDialog>
#include <QElapsedTimer>
#include <QTimer>
#include "perf_graph.h"

namespace Ui {
class inference_panel;
}

class inference_panel : public QDialog
{
    Q_OBJECT

public:
    explicit inference_panel(QString model, QString cpuName, QString gpuName, int imageCount, QWidget *parent = 0);
    ~inference_panel();

private:
    Ui::inference_panel *ui;
    QTimer *timer;
    QElapsedTimer *elapsedTimer;
public slots:
     void closePanelView();
     void setValue(int imageCount);
     void setMaximum(int maximum);
     void startTimer();
     void showTimer();
     void stopTimer();
};

#endif // INFERENCE_PANEL_H
