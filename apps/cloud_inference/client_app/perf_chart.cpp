#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(int mode, QString cpuName, QString gpuName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::perf_chart),
    mMode(mode)
{
    ui->setupUi(this);
    ui->cpuName_label->setText(cpuName);
    ui->gpuName_label->setText(gpuName);
    this->setStyleSheet("background-color: white;");
    initGraph();
}

perf_chart::~perf_chart()
{
    delete bar;
    delete ui;
}

void perf_chart::initGraph()
{
    ui->CustomPlot->addGraph();
    ui->CustomPlot->graph(mCurGraph)->setPen(QPen(colors[0], 6));
    QCPItemText *label = new QCPItemText(ui->CustomPlot);
    label->setVisible(false);
    mLabels.push_back(std::make_tuple(label, 0, 0, 0));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    // set default time duration to rb5 value
    ui->rb5->setChecked(1);

    // x axis
    ui->CustomPlot->xAxis->setTicker(timeTicker);
    ui->CustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 12));
    ui->CustomPlot->xAxis->setLabel("Time");
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
    if (mMode == 1) {
        connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
        ui->x_label->hide();
    } else if (mMode == 2) {
        connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
        ui->x_label->hide();
        ui->setMax->hide();
        ui->MaxFPS->setText("Workload %");
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        QVector<double> ticks;
        QVector<QString> labels;
        ticks << 0 << 130 << 260 << 390 << 520 << 650 << 780 << 910 << 1040 << 1170 << 1300 << 1430 << 1560 << 1690;
        labels << "0%" << "10%" << "20%" << "30%" << "40%" << "50%" << "60%" << "70%" << "80%" << "90%" << "100%" << "110%" << "120%" << "130%";
        textTicker->addTicks(ticks, labels);
        ui->CustomPlot->yAxis->setTicker(textTicker);

        QCPItemLine *line = new QCPItemLine(ui->CustomPlot);
        line->setPen(QPen(Qt::black, 2, Qt::DotLine));
        line->start->setCoords(0, 1300);
        line->end->setCoords(INFINITY, 1300);
    } else if (mMode == 3) {
        ui->setMax->hide();
        ui->MaxFPS->setText("Performance Scale");
        connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        QVector<double> ticks;
        QVector<QString> labels;
        for (int i=1; i<50; i++) {
            ticks << 1225*i;
            QString str;
            str = QString("%1x").arg(i);
            labels << str;
        }
        textTicker->addTicks(ticks, labels);
        ui->CustomPlot->yAxis->setTicker(textTicker);
    }

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

    bar = new perf_bar(this);
    bar->setPosition(perf_chart::x()+perf_chart::width(), perf_chart::y()+perf_chart::height());
    connect(ui->barChart, &QAbstractButton::clicked, this, &perf_chart::barChart);
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
    if (key-lastPointKey > 0.005) // at most add point every 5 ms
    {
        ui->CustomPlot->graph(mCurGraph)->addData(key, mFPSValue);
        lastPointKey = key;
#if defined(ENABLE_KUBERNETES_MODE)
        if (mLastPod != mNumPods) {
            mCurMaxFPS = 0;
            if (mNumPods == mTempPod) {
                mChangedCount++;
                if (mChangedCount == mThreshold) {
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
    ui->CustomPlot->addGraph();
    ui->CustomPlot->graph(mCurGraph)->setPen(QPen(colors[mLastPod%colorNum], 6));
    mCurGraph++;
    QCPItemText *label = new QCPItemText(ui->CustomPlot);
    label->setText("GPUs=" % QString::number(mNumPods));
    label->setFont(QFont(font().family(), 12));
    label->setPen(QPen(Qt::black));
    label->setPadding(QMargins(2,1,2,1));
    label->position->setCoords(key - mRangeX * 0.04, value + mRangeY * 0.12);
    mLabels.push_back(std::make_tuple(label, key, value, mNumPods));

    QCPItemLine *arrow = new QCPItemLine(ui->CustomPlot);
    arrow->start->setParentAnchor(label->bottom);
    arrow->end->setCoords(key, value);

    mLastPod = mNumPods;
    bar->addBar(mNumPods);
    coloredGraph();
}

void perf_chart::coloredGraph()
{
    if (ui->coloredGraph->isChecked()) {
        for (int i=0; i<=mCurGraph; i++) {
            int numPod = std::get<3>(mLabels[i]);
            ui->CustomPlot->graph(i)->setPen(QPen(colors[numPod%colorNum], 6));
        }
        for (unsigned int i=0; i<mLabels.size(); i++) {
            int numPod = std::get<3>(mLabels[i]);
            std::get<0>(mLabels[i])->setPen(QPen(colors[numPod%colorNum], 3));
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
        std::get<0>(mLabels[i])->position->setCoords(key-mRangeX*0.04, value+mRangeY*0.12);
        curRect.setTopLeft(std::get<0>(mLabels[i])->topLeft->pixelPosition());
        curRect.setBottomRight(std::get<0>(mLabels[i])->bottomRight->pixelPosition());
        if (curRect.intersects(prevRect)) {
            qreal newX, newY;
            if (curRect.contains(std::get<0>(mLabels[i])->bottom->pixelPosition())) {
                newX = curRect.x() + curRect.width();
                newY = prevRect.y() - curRect.height();
                std::get<0>(mLabels[i])->position->setPixelPosition(QPointF(newX,newY));
                curRect.setTopLeft(QPointF(newX, newY));
                curRect.setBottomRight(QPointF(newX+curRect.width(), newY+curRect.height()));
            }
            else if (curRect.contains(std::get<0>(mLabels[i])->top->pixelPosition())) {
                newX = curRect.x() + curRect.width();
                newY = prevRect.y() + curRect.height();
                std::get<0>(mLabels[i])->position->setPixelPosition(QPointF(newX,newY));
                curRect.setTopLeft(QPointF(newX, newY));
                curRect.setBottomRight(QPointF(newX+curRect.width(), newY+curRect.height()));
            }
        }
        ui->CustomPlot->replot();
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
        ui->CustomPlot->xAxis->setRange(-10, key+100);
        mRangeX = key+100;
    }
    if (ui->setMax->isChecked()) {
        ui->CustomPlot->yAxis->setRange(0, 10000);
        bar->setMax(10000);
    } else {
        mRangeY = mMaxFPS*1.5;
        bar->setMax(mRangeY);
        ui->CustomPlot->yAxis->setRange(0, mRangeY);
    }
#if defined(ENABLE_KUBERNETES_MODE)
    fixLabelLocation();
#endif
}

void perf_chart::updateFPSValue(float fpsValue)
{
    //fpsValue *= mDummyPods;
    if (mMode == 1) {
        mFPSValue = fpsValue;
        if (mFPSValue > mMaxFPS) {
            mMaxFPS = mFPSValue;
            ui->maxfps_lcdNumber->display(mMaxFPS);
        }
        if (mNumPods != 0)
            bar->setFPS(fpsValue);
    }
    else if (mMode == 2) {
        mNumPods = std::max(mNumPods, 1);
        mFPSValue = fpsValue / mNumPods;
        if (mFPSValue > mMaxFPS) {
            mMaxFPS = mFPSValue;
        }
        ui->maxfps_lcdNumber->display(mFPSValue / 13);
        if (mNumPods != 0) {
            bar->setFPS(mFPSValue);
        }
    }
    else if (mMode == 3) {
        mFPSValue = fpsValue;
        if (mFPSValue > mMaxFPS) {
            mMaxFPS = mFPSValue;
        }
        if (mFPSValue > mCurMaxFPS) {
            mCurMaxFPS = mFPSValue;
            mNumPods = std::max(mNumPods, 1);
            localMaxFPS = std::max((int)(mCurMaxFPS / mNumPods), 1225);
        }
        float scaling = fpsValue / localMaxFPS;
        ui->maxfps_lcdNumber->display(QString("%1").arg(scaling, 0, 'f', 2));
        if (mNumPods != 0)
            bar->setFPS(fpsValue);
    }
}

#if defined(ENABLE_KUBERNETES_MODE)
void perf_chart::setPods(int numPods)
{
    mNumPods = numPods;
    //ui->pods_lcdNumber->display(numPods);
    bar->setPods(numPods);
}

void perf_chart::barChart()
{
    if (ui->barChart->isChecked()) {
        bar->show();
    }
    else {
        bar->closeBarView();
    }
}
#endif

void perf_chart::setGPUs(int numGPUs)
{
    bar->setGPUs(numGPUs);
}

void perf_chart::setTotalGPUs(int numGPUs)
{
    mNumGPUs = numGPUs;
    ui->gpus_lcdNumber->display(numGPUs);
    bar->setTotalGPUs(numGPUs);
}

void perf_chart::closeChartView()
{
    //setPods(++mDummyPods);
    bar->closeBarView();
    this->close();
}

