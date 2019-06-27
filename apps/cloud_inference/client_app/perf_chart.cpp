#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::perf_chart)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
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
    ui->CustomPlot->graph(0)->setPen(QPen(Qt::darkBlue, 6));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    // set default time duration to rb1 value
    ui->rb1->setChecked(1);

    // x axis
    ui->CustomPlot->xAxis->setTicker(timeTicker);
    ui->CustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 12));
    ui->CustomPlot->xAxis->setLabel("Time Elapsed");
    ui->CustomPlot->xAxis->setLabelFont((QFont(QFont().family(), 20)));
    ui->CustomPlot->xAxis->grid()->setSubGridVisible(false);

    // y axis
    ui->CustomPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 12));
    ui->CustomPlot->yAxis->setLabel("FPS");
    ui->CustomPlot->yAxis->setLabelFont((QFont(QFont().family(), 20)));
    ui->CustomPlot->yAxis->grid()->setSubGridVisible(false);

    ui->CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->CustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));

    // rescale axis
    connect(ui->rb1, &QAbstractButton::clicked, this, &perf_chart::rescaleAxis);
    connect(ui->rb2, &QAbstractButton::clicked, this, &perf_chart::rescaleAxis);
    connect(ui->rb3, &QAbstractButton::clicked, this, &perf_chart::rescaleAxis);
    connect(ui->rb4, &QAbstractButton::clicked, this, &perf_chart::rescaleAxis);
    connect(ui->rb5, &QAbstractButton::clicked, this, &perf_chart::rescaleAxis);

    // set default option to moving graph
    ui->movingGraph->setChecked(1);

#if defined(ENABLE_KUBERNETES_MODE)
    // set default option to moving colored graph
    ui->coloredGraph->setChecked(1);
    // use checkbox to set colored graph
    connect(ui->coloredGraph, &QAbstractButton::clicked, this, &perf_chart::coloredGraph);
    coloredGraph();
#endif

    // close button
    connect(ui->closeButton, &QAbstractButton::clicked, this, &perf_chart::closeChartView);
    timer.start(5);
}

void perf_chart::RealtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        ui->CustomPlot->graph(mCurGraph)->addData(key, mFPSValue);
        lastPointKey = key;
#if defined(ENABLE_KUBERNETES_MODE)
        if (mLastPod != mNumPods) {
            if (mNumPods == mTempPod) {
                mChangedCount++;
                if (mChangedCount == 250) {
                    changePods(key, mFPSValue);
                }
            }
            else {
                mChangedCount = 0;
                mTempPod = mNumPods;
            }
        }
#endif
    }
    if (ui->movingGraph->isChecked()) {
        rescaleAxis(key);
    }
    ui->CustomPlot->replot();
}

#if defined(ENABLE_KUBERNETES_MODE)
void perf_chart::changePods(double key, double value)
{
    QCPItemText *label = new QCPItemText(ui->CustomPlot);
    label->setText("Pods=" % QString::number(mNumPods));
    label->setFont(QFont(font().family(), 10));
    label->setPen(QPen(Qt::black));
    label->setPadding(QMargins(2,1,2,1));
    label->position->setCoords(key - mRangeX * 0.03, value + mRangeY * 0.12);
    mLabels.push_back(std::make_tuple(label, key, value));

    QCPItemLine *arrow = new QCPItemLine(ui->CustomPlot);
    arrow->start->setParentAnchor(label->bottom);
    arrow->end->setCoords(key, value);

    mLastPod = mNumPods;
    ui->CustomPlot->addGraph();
    mCurGraph++;
    coloredGraph();
}

void perf_chart::coloredGraph()
{
    if (ui->coloredGraph->isChecked()) {
        for (int i=0; i<=mCurGraph; i++) {
            ui->CustomPlot->graph(i)->setPen(QPen(colors[i % 4], 6));
        }
        for (unsigned int i=0; i<mLabels.size(); i++) {
            std::get<0>(mLabels[i])->setPen(QPen(colors[(i+1) % 4], 3));
        }
    }
    else {
        for (int i=0; i<=mCurGraph; i++) {
            ui->CustomPlot->graph(i)->setPen(QPen(Qt::gray, 6));
        }
        for (unsigned int i=0; i<mLabels.size(); i++) {
            std::get<0>(mLabels[i])->setPen(QPen(Qt::black, 3));
        }
    }
    ui->CustomPlot->replot();
}

void perf_chart::fixLabelLocation()
{
    QRectF prevRect(0,0,0,0);
    QRectF curRect;
    for (unsigned int i=0; i<mLabels.size(); i++) {
        double key = std::get<1>(mLabels[i]);
        double value = std::get<2>(mLabels[i]);
        std::get<0>(mLabels[i])->position->setCoords(key-mRangeX*0.03, value+mRangeY*0.12);
        curRect.setTopLeft(std::get<0>(mLabels[i])->topLeft->pixelPosition());
        curRect.setBottomRight(std::get<0>(mLabels[i])->bottomRight->pixelPosition());
        if (curRect.intersects(prevRect)) {
            qreal newX, newY;
            if (curRect.contains(std::get<0>(mLabels[i])->bottom->pixelPosition())) {
                newX = curRect.x() + curRect.width();
                newY = prevRect.y() - curRect.height();
                std::get<0>(mLabels[i])->position->setPixelPosition(QPointF(newX,newY));
            }
            else if (curRect.contains(std::get<0>(mLabels[i])->top->pixelPosition())) {
                newX = curRect.x() + curRect.width();
                newY = prevRect.y() + curRect.height();
                std::get<0>(mLabels[i])->position->setPixelPosition(QPointF(newX,newY));
            }
        }
        prevRect = curRect;
    }
}

#endif

void perf_chart::rescaleAxis(double key)
{
    ui->CustomPlot->graph()->rescaleValueAxis();
    if (ui->rb1->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+20, 60, Qt::AlignRight);
        mRangeX = 60;
    } else if (ui->rb2->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 120, Qt::AlignRight);
        mRangeX = 120;
    } else if (ui->rb3->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 240, Qt::AlignRight);
        mRangeX = 240;
    } else if (ui->rb4->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 480, Qt::AlignRight);
        mRangeX = 480;
    } else if (ui->rb5->isChecked()) {
        ui->CustomPlot->xAxis->setRange(0, key+100);
        mRangeX = key+100;
    }
    mRangeY = mMaxFPS*1.5;
    ui->CustomPlot->yAxis->setRange(0, mRangeY);
#if defined(ENABLE_KUBERNETES_MODE)
    fixLabelLocation();
#endif
}

void perf_chart::updateFPSValue(int fpsValue)
{
    mFPSValue = fpsValue;
    if (mFPSValue > mMaxFPS) {
        mMaxFPS = mFPSValue;
        ui->maxfps_lcdNumber->display(mMaxFPS);
    }
}

#if defined(ENABLE_KUBERNETES_MODE)
void perf_chart::setPods(int numPods)
{
    mNumPods = numPods;
    ui->pods_lcdNumber->display(numPods);
}
#endif

void perf_chart::setGPUs(int numGPUs)
{
    mNumGPUs = numGPUs;
    ui->gpus_lcdNumber->display(numGPUs);
}

void perf_chart::closeChartView()
{
    this->close();
}

