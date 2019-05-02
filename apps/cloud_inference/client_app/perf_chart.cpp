#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::perf_chart)
{
    ui->setupUi(this);
    mFPSValue = 0;
    mMaxFPS = 0;
    mScaleValueX = 5;
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

    // set default time duration to rb1 value
    ui->rb1->setChecked(1);

    // x axis
    ui->CustomPlot->xAxis->setTicker(timeTicker);
    ui->CustomPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->CustomPlot->xAxis->setLabel("Time Elapsed");

    // y axis
    ui->CustomPlot->yAxis->setLabel("FPS");

    ui->CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//    ui->CustomPlot->axisRect()->setupFullAxesBox();
    ui->CustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->CustomPlot->graph(0)->addData(-5, 10);
    ui->CustomPlot->graph(0)->addData(-4, 11);
    ui->CustomPlot->graph(0)->addData(-3, 12);
    ui->CustomPlot->graph(0)->addData(-2, 13);
    ui->CustomPlot->graph(0)->addData(-1, 14);
    ui->CustomPlot->graph(0)->addData(3, 16);
    ui->CustomPlot->graph(0)->addData(2, 17);
    ui->CustomPlot->graph(0)->addData(1, 18);
    ui->CustomPlot->graph(0)->addData(0, 19);
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->CustomPlot->graph(0)->setSelectable(QCP::stMultipleDataRanges);
    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    connect(&timer2, SIGNAL(timeout()), this, SLOT(colorSelected()));
    timer.start(1);
    timer2.start(5);
    //ui->CustomPlot->graph(0)->addData(0, 16);
}
void perf_chart::colorSelected()
{
    QCPDataRange range;
    range.setBegin(0);
    range.setEnd(20);

    QCPDataSelection selection;
    selection.addDataRange(range);
    QCPSelectionDecorator * decorator = new QCPSelectionDecorator();

    decorator->setBrush(QBrush(Qt::red));
    ui->CustomPlot->graph(0)->setSelectionDecorator(decorator);
    ui->CustomPlot->graph(0)->setSelection(selection);

}
void perf_chart::RealtimeDataSlot()
{
    //ui->CustomPlot->graph(0)->addData(0, 16);
    QCPDataRange range;
    range.setBegin(10);
    range.setEnd(20);

    QCPDataSelection selection;
    selection.addDataRange(range);
    QCPSelectionDecorator * decorator = new QCPSelectionDecorator();
    decorator->setBrush(QBrush(Qt::green));
    ui->CustomPlot->graph(0)->setSelectionDecorator(decorator);
    ui->CustomPlot->graph(0)->setSelection(selection);
//    static QTime time(QTime::currentTime());
//    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
//    static double lastPointKey = 0;
//    static int lastNumPods = 0;
//    ui->CustomPlot->graph(0)->addData(1, 16);
//    ui->CustomPlot->graph(0)->addData(3, 17);
//    ui->CustomPlot->graph(0)->addData(4, 18);
//    ui->CustomPlot->graph(0)->addData(7, 19);
//    if (key-lastPointKey > 0.01) // at most add point every 10 ms
//    {
//        ui->CustomPlot->graph(0)->setData(key, mFPSValue);
//        lastPointKey = key;

//        if (lastNumPods != mNumPods) {
//            QCPItemText *text = new QCPItemText(ui->CustomPlot);
//            text->position->setCoords(key - mScaleValueX, mFPSValue + 5);
//            text->setText("Pod(s) = " % QString::number(mNumPods));
//            text->setFont(QFont(font().family(), 10));
//            text->setPen(QPen(Qt::black));
//            lastNumPods = mNumPods;
//        }
//    }


//    QCPSelectionDecorator * decorator = new QCPSelectionDecorator();

//    decorator->setBrush(QBrush(Qt::red));

//    ui->CustomPlot->graph(0)->setSelectionDecorator(decorator);
//    //ui->CustomPlot->graph(0)->setSelection(selection);

////    if (ui->coloredGraph->isChecked())
////    {

////        ui->CustomPlot->graph(0)->setSelectionDecorator(decorator);
////    }
    ui->CustomPlot->graph(0)->rescaleValueAxis();
    ui->CustomPlot->replot();
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
        mScaleValueX = 4;
    } else if (ui->rb2->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 60, Qt::AlignRight);
        mScaleValueX = 6;
    } else if (ui->rb3->isChecked()) {
        ui->CustomPlot->xAxis->setRange(key+30, 120, Qt::AlignRight);
        mScaleValueX = 9;
    }
    ui->CustomPlot->yAxis->setRange(0, mMaxFPS+500);
    ui->CustomPlot->replot();
}

void perf_chart::closeChartView()
{
    this->close();
}

