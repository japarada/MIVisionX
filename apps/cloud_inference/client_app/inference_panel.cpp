#include "inference_panel.h"
#include "ui_inference_panel.h"

inference_panel::inference_panel(QString model, QString cpuName, QString gpuName, int imageCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inference_panel)
{
    ui->setupUi(this);
    ui->modelName_label->setText(model);
    ui->CPUName_label->setText(cpuName);
    ui->GPUName_label->setText(gpuName);
    ui->progressBar->setMaximum(imageCount);
    connect(ui->close_pushButton, &QAbstractButton::clicked, this, &inference_panel::closePanelView);
}

inference_panel::~inference_panel()
{
    delete ui;
}

void inference_panel::closePanelView()
{
   this->close();
}

void inference_panel::setValue(int imageCount)
{
    ui->progressBar->setValue(imageCount);
}
