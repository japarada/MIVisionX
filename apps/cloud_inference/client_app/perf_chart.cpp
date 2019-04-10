#include "perf_chart.h"
#include "ui_perf_chart.h"

perf_chart::perf_chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::perf_chart)
{
    ui->setupUi(this);
    maxFPS = 0;
    connect(ui->close_pushButton, &QAbstractButton::clicked, this, &perf_chart::closePerformanceView);
    connect(ui->reset_pushButton, &QAbstractButton::clicked, this, &perf_chart::resetPerformanceView);
}

perf_chart::~perf_chart()
{
    delete ui;
}
void perf_chart::closePerformanceView()
{
   this->close();
}
void perf_chart::setModelName(QString ModelName)
{
   ui->modelName_label->setText(ModelName);
}
void perf_chart::setStartTime(QString startTime)
{
   ui->StartTime_label->setText(startTime);
}
void perf_chart::updateElapsedTime(QString elapsedTime)
{
   ui->elapsedTime_label->setText(elapsedTime);
}
void perf_chart::setNumGPU(int numGPU)
{
   ui->GPU_lcdNumber->display(numGPU);
}
void perf_chart::setTotalGPU(int totalGPU)
{
   ui->TotalGPU_lcdNumber->display(totalGPU);
}
void perf_chart::setPods(int numPods)
{
   ui->pods_lcdNumber->display(numPods);
}
void perf_chart::resetPerformanceView()
{
    ui->fps_lcdNumber->display(0);
    ui->images_lcdNumber->display(0);
}
void perf_chart::updateFPSValue(float fps)
{
    fps = int(fps);
    ui->fps_lcdNumber->display(fps);

    if(maxFPS < fps){
        maxFPS = fps;
        ui->maxFPS_lcdNumber->display(maxFPS);
    }
}
void perf_chart::updateTotalImagesValue(int images)
{
    ui->images_lcdNumber->display(images);
}
