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
    elapsedTimer = new QElapsedTimer();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimer()));
    timer->start(10);
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
    if (imageCount == ui->progressBar->maximum()) {
        stopTimer();
    }
}

void inference_panel::startTimer()
{
    elapsedTimer->start();
}

void inference_panel::setMaximum(int maximum)
{
    ui->progressBar->setMaximum(maximum);
}

void inference_panel::showTimer()
{
    qint64 time = elapsedTimer->elapsed();

    int h = time / 1000 / 60 / 60;
    int m = (time / 1000 / 60) - (h * 60);
    int s = (time / 1000) - (m * 60);
    int ms = time - ( s + ( m + ( h * 60)) * 60) * 1000;
    int ms_dis = ms / 10;
    if(ms_dis < 10) {
        ui->msLCDNumber->display(QStringLiteral("0%1").arg(ms_dis));
    }
    else {
        ui->msLCDNumber->display(QStringLiteral("%1").arg(ms_dis));
    }
    if(s < 10) {
        ui->sLCDNumber->display(QStringLiteral("0%1").arg(s));
    }
    else {
        ui->sLCDNumber->display(QStringLiteral("%1").arg(s));
    }
    if(m < 10) {
        ui->mLCDNumber->display(QStringLiteral("0%1").arg(m));
    }
    else {
        ui->mLCDNumber->display(QStringLiteral("%1").arg(m));
    }
}

void inference_panel::stopTimer()
{
    timer->stop();
    elapsedTimer->invalidate();
}
