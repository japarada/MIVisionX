#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::perf_chart)
{
    ui->setupUi(this);
    mFPSValue = 0;
    mMaxFPS = 0;
    initGraph();
}

perf_chart::~perf_chart()
{
    delete ui;
}

void perf_chart::initGraph()
{
    ui->CustomPlot->addGraph();
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    // set default time duration to rb2 value
    ui->rb2->setChecked(1);

    // x axis
    ui->CustomPlot->xAxis->setTicker(timeTicker);
    ui->CustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 12));
    ui->CustomPlot->xAxis->setLabel("Time Elapsed");

    // y axis
    ui->CustomPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 12));
    ui->CustomPlot->yAxis->setLabel("FPS");

    ui->CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->CustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    timer.start(5);
}

void perf_chart::RealtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    static int lastNumPods = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        ui->CustomPlot->graph(0)->addData(key, mFPSValue);
        lastPointKey = key;

        if (lastNumPods != mNumPods) {
            QCPItemText *text = new QCPItemText(ui->CustomPlot);
            text->setText("Pod(s) = " % QString::number(mNumPods));
            text->setFont(QFont(font().family(), 10));
            text->setPen(QPen(Qt::black));
            lastNumPods = mNumPods;
            text->position->setCoords(key - mRangeX * 0.1, mFPSValue + mRangeY * 0.1);
            mPodsVector.push_back(text);
        }
    }
    rescaleAxis(key);
}

void perf_chart::updateFPSValue(int fpsValue)
{
    mFPSValue = fpsValue;
    if (mFPSValue > mMaxFPS)
        mMaxFPS = mFPSValue;
}

void perf_chart::setPods(int numPods)
{
    mNumPods = numPods;
    ui->pods_lcdNumber->display(numPods);
}

void perf_chart::rescaleAxis(double key) {
    ui->CustomPlot->graph(0)->rescaleValueAxis();
    if (ui->rb1->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+20, 30, Qt::AlignRight);
        mRangeX = 30;
    } else if (ui->rb2->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 60, Qt::AlignRight);
        mRangeX = 60;
    } else if (ui->rb3->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 120, Qt::AlignRight);
        mRangeX = 120;
    } else if (ui->rb4->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 240, Qt::AlignRight);
        mRangeX = 240;
    } else if (ui->rb5->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 480, Qt::AlignRight);
        mRangeX = 480;
    }
    mRangeY = mMaxFPS*1.5;
    ui->CustomPlot->yAxis->setRange(0, mRangeY);
    mPodsVector[0]->position->setCoords(100, 100);
//    for (unsigned long i=0; i<mPodsVector.size(); i++) {
//        double x = mPodsVector[i]->position->key();
//        double y = mPodsVector[i]->position->value();
//        mPodsVector[i]->position->setCoords(x - mRangeX * 0.1, y + mRangeY * 0.1);
//    }
    ui->CustomPlot->replot();
}

void perf_chart::closeChartView()
{
    this->close();
}

