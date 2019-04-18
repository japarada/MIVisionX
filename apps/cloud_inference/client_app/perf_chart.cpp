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
    connect(ui->close_pushButton, &QAbstractButton::clicked, this, &perf_chart::closeChartView);
}

perf_chart::~perf_chart()
{
    delete ui;
}

void perf_chart::initGraph()
{
    ui->widget_RealTimeGraph->addGraph();
    ui->widget_RealTimeGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->widget_RealTimeGraph->graph(0)->setAntialiasedFill(false);
    //ui->widget_RealTimeGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    // x axis
    ui->widget_RealTimeGraph->xAxis->setTicker(timeTicker);
    ui->widget_RealTimeGraph->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->widget_RealTimeGraph->xAxis->setLabel("Time");

    // y axis
    ui->widget_RealTimeGraph->yAxis->setLabel("FPS");
    ui->widget_RealTimeGraph->yAxis->setRange(0, 1500);

    ui->widget_RealTimeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //ui->widget_RealTimeGraph->legend->setVisible(true);
    //ui->widget_RealTimeGraph->legend->setBrush(QBrush(QColor(255,255,255,150)));

    ui->widget_RealTimeGraph->axisRect()->setupFullAxesBox();
    ui->widget_RealTimeGraph->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignTop);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->widget_RealTimeGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_RealTimeGraph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_RealTimeGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_RealTimeGraph->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    timer.start(5);
}

void perf_chart::RealtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        ui->widget_RealTimeGraph->graph(0)->addData(key, mFPSValue);
        lastPointKey = key;
    }

    // make x axis range scroll with the data
    ui->widget_RealTimeGraph->graph(0)->rescaleValueAxis();
    ui->widget_RealTimeGraph->xAxis->setRange(key+20, 60, Qt::AlignRight);
    ui->widget_RealTimeGraph->yAxis->setRange(0, mMaxFPS+500);

    ui->widget_RealTimeGraph->replot();
}

void perf_chart::updateFPSValue(int fpsValue)
{
    mFPSValue = fpsValue;
    if (mFPSValue > mMaxFPS)
        mMaxFPS = mFPSValue;
}

void perf_chart::setPods(int numPods)
{
    //ui->widget_RealTimeGraph->graph(0)->addData(key, mFPSValue);
}

void perf_chart::closeChartView()
{
    this->close();
}

