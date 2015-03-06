#include <QBrush>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_maxMatch(0)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);
    ui->maxMatchLink->setReadOnly(true);

    ui->numThreads->setInputMask("D");
    ui->numLinks->setInputMask("D99");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if (ui->startButton->text() == "Start")
    {
        ui->textToSearch->setReadOnly(true);
        ui->startUrl->setReadOnly(true);
        ui->numThreads->setReadOnly(true);
        ui->numLinks->setReadOnly(true);
        ui->maxMatchLink->setText("");

        ui->stopButton->setEnabled(true);

        ui->lcdNumber->display(0);

        ui->processedLinksList->clear();
        ui->linksWithMatchList->clear();

        ui->startButton->setText("Pause");

        emit start(ui->startUrl->text(), ui->textToSearch->text(), ui->numThreads->text().toInt(), ui->numLinks->text().toInt());
    }
    else if (ui->startButton->text() == "Pause")
    {
        ui->startButton->setText("Resume");

        emit pause();
    }
    else if (ui->startButton->text() == "Resume")
    {
        ui->startButton->setText("Pause");

        emit resume();
    }
}

void MainWindow::updateLists(QString aProcessedLink, int aMatchNum)
{
    ui->processedLinksList->addItem(new QListWidgetItem(aProcessedLink));
    if (aMatchNum == -1)
    {
        ui->processedLinksList->item(ui->processedLinksList->count() - 1)->setForeground(QBrush(Qt::red));
    }
    else if (aMatchNum > 0)
    {
        ui->linksWithMatchList->addItem(new QListWidgetItem(aProcessedLink));
        ui->lcdNumber->display(ui->lcdNumber->value() + aMatchNum);
    }

    if (aMatchNum > m_maxMatch)
    {
        ui->lcdNumber_2->display(aMatchNum);
        m_maxMatch = aMatchNum;
        ui->maxMatchLink->setText(aProcessedLink);
    }
}

void MainWindow::on_stopButton_clicked()
{
    emit stop();
}

void MainWindow::onWorkerStopped()
{
    ui->startButton->setText("Start");
    ui->startButton->setEnabled(true);

    ui->stopButton->setEnabled(false);

    ui->textToSearch->setReadOnly(false);
    ui->startUrl->setReadOnly(false);
    ui->numThreads->setReadOnly(false);
    ui->numLinks->setReadOnly(false);

    m_maxMatch = 0;
}

