#include "perf_bar.h"
#include "ui_perf_bar.h"

perf_bar::perf_bar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::perf_bar)
{
    ui->setupUi(this);
    initGraph();
}

perf_bar::~perf_bar()
{
    delete mCurBar;
    delete ui;
}

void perf_bar::initGraph()
{
    ui->CustomBar->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->CustomBar, "Max FPS per pods", QFont("sans", 15, QFont::Bold));
    ui->CustomBar->plotLayout()->addElement(0, 0, title);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    ui->CustomBar->xAxis->setTicker(textTicker);
    ui->CustomBar->xAxis->setLabel("Pod(s)");
    ui->CustomBar->yAxis->setLabel("FPS");
}

void perf_bar::addBar(int numPod)
{
    QCPBars *bar = new QCPBars(ui->CustomBar->xAxis, ui->CustomBar->yAxis);
    mCurBarTick++;
    bar->setPen(QPen(colors[mCurBarTick % 4], 6));
    bar->setBrush(QColor(colors[mCurBarTick % 4]));
    QSharedPointer<QCPAxisTickerText> textTicker = qSharedPointerDynamicCast<QCPAxisTickerText>(ui->CustomBar->xAxis->ticker());
    textTicker->addTick(mCurBarTick, QString::number(numPod));
    mCurBar = bar;
    mCurMax = 0;
}

void perf_bar::setFPS(int fpsValue)
{
    if (fpsValue > mMaxFPS) {
        mMaxFPS = fpsValue;
    }
    if (fpsValue > mCurMax && mCurBarTick != 0) {
        mCurMax = fpsValue;
        changeBar();
    }
}

void perf_bar::changeBar()
{
    mCurBar->data()->clear();
    mCurBar->addData(mCurBarTick, mCurMax);
    ui->CustomBar->xAxis->setRange(0, mCurBarTick+1);
    ui->CustomBar->yAxis->setRange(0, mMaxFPS*1.5);
    ui->CustomBar->replot();
}

void perf_bar::setPosition(double x, double y)
{
    perf_bar::move(x, y);
}
void perf_bar::closeBarView()
{
    this->close();
}
