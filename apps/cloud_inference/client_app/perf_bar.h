#ifndef PERF_BAR_H
#define PERF_BAR_H

#include <QDialog>
#include <qcustomplot.h>

//static Qt::GlobalColor colors[16] = {Qt::darkRed, Qt::darkBlue, Qt::darkYellow, Qt::darkGreen, Qt::darkMagenta, Qt::darkCyan, Qt::darkGray,
//                                          Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan, Qt::black, Qt::gray, QColor(320, 320, 320)};

static const int colorNum = 19;
static QColor colors[colorNum] = {
    { 0,255,0 },
    { 0, 0,255 },
    { 255, 0,0 },
    { 250, 150, 70 },
    { 102,102,156 },
    { 190,153,153 },
    { 0,  0,   0 },
    { 220,220,  0 },
    { 0, 255, 0 },
    { 152,251,152 },
    { 135,206,250 },
    { 220, 20, 60 },
    { 255,  0,  0 },
    { 0,  0,255 },
    { 0,  0, 70 },
    { 0, 60,100 },
    { 0, 80,100 },
    { 0,  0,230 },
    { 119, 11, 32 }
};

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
    void setMax(int);
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
