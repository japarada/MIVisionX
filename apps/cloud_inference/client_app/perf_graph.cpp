#include "perf_graph.h"
#include "ui_perf_graph.h"

perf_graph::perf_graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::perf_graph)
{
    ui->setupUi(this);
    maxFPS = 0;
}

perf_graph::~perf_graph()
{
    delete ui;
}
void perf_graph::closePerformanceView()
{
   this->close();
}
void perf_graph::setModelName(QString ModelName)
{
   ui->modelName_label->setText(ModelName);
}
void perf_graph::setStartTime(QString startTime)
{
   ui->StartTime_label->setText(startTime);
}
void perf_graph::updateElapsedTime(QString elapsedTime)
{
   ui->elapsedTime_label->setText(elapsedTime);
}
void perf_graph::setNumGPU(int numGPU)
{
   ui->GPU_lcdNumber->display(numGPU);
}
void perf_graph::setTotalGPU(int totalGPU)
{
   ui->TotalGPU_lcdNumber->display(totalGPU);
}
void perf_graph::setPods(int numPods)
{
   ui->pods_lcdNumber->display(numPods);
}
void perf_graph::resetPerformanceView()
{
    ui->fps_lcdNumber->display(0);
    ui->images_lcdNumber->display(0);
}
void perf_graph::updateFPSValue(float fps)
{
    if (mMode == 3) {
        float scale = fps / localMaxFPS;
        ui->fps_lcdNumber->display(QString("%1").arg((double)scale, 0, 'f', 3));
    }
    else {
        fps = int(fps);
        ui->fps_lcdNumber->display(fps);

        if(maxFPS < fps){
            maxFPS = fps;
            ui->maxFPS_lcdNumber->display(maxFPS);
        }
    }
}
void perf_graph::updateTotalImagesValue(int images)
{
    ui->images_lcdNumber->display(images);
}

void perf_graph::setMode(int mode)
{
    mMode = mode;
}
void perf_graph::hideFPS()
{
    ui->maxFPS_label->hide();
    ui->maxFPS_lcdNumber->hide();
    ui->fps_label->setText("Performance scaling");
    ui->elapsedTime_label->hide();
    ui->ElapsedTimeName_label->hide();
}
