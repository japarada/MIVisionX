#include "inference_panel.h"
#include "ui_inference_panel.h"

inference_panel::inference_panel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inference_panel)
{
    ui->setupUi(this);
    display_panel = new perf_graph(3);
    display_panel->setWindowIcon(QIcon(":/images/vega_icon_150.png"));
    connect(ui->close_pushButton, &QAbstractButton::clicked, this, &inference_panel::closePerformanceView);

}

inference_panel::~inference_panel()
{
    delete ui;
}
void inference_panel::closePerformanceView()
{
   this->close();
}
void inference_panel::setModelName(QString ModelName)
{
   ui->modelName_label->setText(ModelName);
}
void inference_panel::setGPUName(QString GPUName)
{
   ui->GPUName_label->setText(GPUName);
}
void inference_panel::setNumGPU(int numGPU)
{
   ui->GPU_lcdNumber->display(numGPU);
}
void inference_panel::setCPUName(QString CPUName)
{
   ui->CPUName_label->setText(CPUName);
}
void inference_panel::setNumCPU(int numCPU)
{
   ui->CPU_lcdNumber->display(numCPU);
}
void inference_panel::updateFPSValue(float fps)
{
    int fps_value = int(fps);
    ui->progressBar->setValue(fps_value);

}
