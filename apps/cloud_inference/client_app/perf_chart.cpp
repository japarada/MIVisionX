#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::perf_chart)
{
    ui->setupUi(this);
    maxFPS = 0;
    initGraph();
}

perf_chart::~perf_chart()
{
    delete ui;
}

void perf_chart::initGraph()
{
    ui->widget_RealTimeGraph->addGraph();
    ui->widget_RealTimeGraph->graph(0)->setPen(QPen(Qt::red));
    ui->widget_RealTimeGraph->graph(0)->setAntialiasedFill(false);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    // x axis
    ui->widget_RealTimeGraph->xAxis->setTicker(timeTicker);
    ui->widget_RealTimeGraph->axisRect()->setupFullAxesBox();
    ui->widget_RealTimeGraph->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->widget_RealTimeGraph->xAxis->setLabel("Time");

    ui->widget_RealTimeGraph->yAxis->setLabel("FPS");

    ui->widget_RealTimeGraph->yAxis->setRange(0, 1500);
    ui->widget_RealTimeGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc));
    ui->widget_RealTimeGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->widget_RealTimeGraph->legend->setVisible(true);
    ui->widget_RealTimeGraph->legend->setBrush(QBrush(QColor(255,255,255,150)));

    ui->widget_RealTimeGraph->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignTop);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->widget_RealTimeGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_RealTimeGraph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget_RealTimeGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget_RealTimeGraph->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&timer, SIGNAL(timeout()), this, SLOT(RealtimeDataSlot()));
    timer.start(0);
}

void perf_chart::updateFPSValue(int fpsValue) {
    mFPSValue = fpsValue;
}

void perf_chart::RealtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    //qsrand(QTime::currentTime().msecsSinceStartOfDay());

    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    //ui->widget_RealTimeGraph->graph(0)->addData(key, mFPSValue);
    QCPItemText *item=new QCPItemText(ui->widget_RealTimeGraph);

    item->setPositionAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    //item->position->setType(QCPItemPosition::ptAxisRectRatio);
    //item->position->setCoords(key,value+10);
    item->setColor(ui->widget_RealTimeGraph->graph(0)->pen().color());
    //item->setText(QString::number(value));

    if (key-lastPointKey > 0.001) // at most add point every 2 ms
    {
        ui->widget_RealTimeGraph->graph(0)->addData(key, mFPSValue);
        //ui->widget_RealTimeGraph->graph(0)->rescaleValueAxis();
        lastPointKey = key;
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->widget_RealTimeGraph->xAxis->setRange(key+10, 50, Qt::AlignRight);
    ui->widget_RealTimeGraph->replot();
}

void perf_chart::on_pushButton_Start_clicked()
{
    timer.start(1000);
}

void perf_chart::on_pushButton_Stop_clicked()
{
    timer.stop();
}


//void perf_chart::closePerformanceView()
//{
//   this->close();
//}
//void perf_chart::setModelName(QString ModelName)
//{
//   ui->modelName_label->setText(ModelName);
//}
//void perf_chart::setStartTime(QString startTime)
//{
//   ui->StartTime_label->setText(startTime);
//}
//void perf_chart::updateElapsedTime(QString elapsedTime)
//{
//   ui->elapsedTime_label->setText(elapsedTime);
//}
//void perf_chart::setNumGPU(int numGPU)
//{
//   ui->GPU_lcdNumber->display(numGPU);
//}
//void perf_chart::setTotalGPU(int totalGPU)
//{
//   ui->TotalGPU_lcdNumber->display(totalGPU);
//}
//void perf_chart::setPods(int numPods)
//{
//   ui->pods_lcdNumber->display(numPods);
//}
//void perf_chart::resetPerformanceView()
//{
//    ui->fps_lcdNumber->display(0);
//    ui->images_lcdNumber->display(0);
//}
//void perf_chart::updateFPSValue(float fps)
//{
//    fps = int(fps);
//    ui->fps_lcdNumber->display(fps);

//    if(maxFPS < fps){
//        maxFPS = fps;
//        ui->maxFPS_lcdNumber->display(maxFPS);
//    }
//}
//void perf_chart::updateTotalImagesValue(int images)
//{
//    ui->images_lcdNumber->display(images);
//}
