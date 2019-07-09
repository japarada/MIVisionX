#ifndef PERF_BAR_H
#define PERF_BAR_H

#include <QDialog>
#include <qcustomplot.h>

static const Qt::GlobalColor colors[4] = {Qt::darkRed, Qt::darkBlue, Qt::darkYellow, Qt::darkGreen};

namespace Ui {
class perf_bar;
}

class perf_bar : public QDialog
{
    Q_OBJECT

public:
    explicit perf_bar(QWidget *parent = nullptr);
    ~perf_bar();

public slots:
    void initGraph();
    void closeBarView();
    void addBar(int);
    void setFPS(int);
    void changeBar();
    void setPosition(double x, double y);

private:
    Ui::perf_bar *ui;
    QCPBars *mCurBar;

    double mCurMax = 0;
    int mMaxFPS = 0;
    int mCurBarTick = 0;


};

#endif // PERF_BAR_H
