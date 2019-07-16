#ifndef PERF_BAR_H
#define PERF_BAR_H

#include <QDialog>
#include <qcustomplot.h>

static const Qt::GlobalColor colors[15] = {Qt::darkRed, Qt::darkBlue, Qt::darkYellow, Qt::darkGreen, Qt::darkMagenta, Qt::darkCyan, Qt::darkGray,
                                          Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan, Qt::black, Qt::gray};

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
    void setPosition(double, double);
    void setPods(int);
    void setGPUs(int);
    void setTotalGPUs(int);

private:
    Ui::perf_bar *ui;
    QCPBars *mCurBar;

    double mCurMax = 0;
    int mMaxFPS = 0;
    int mCurBarTick = 0;


};

#endif // PERF_BAR_H
