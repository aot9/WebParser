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

    setWindowTitle("Web Parser");

    QObject::connect(ui->processedLinksList->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                     ui->processedLinksList,          SLOT(scrollToBottom()));

    QObject::connect(ui->linksWithMatchList->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                     ui->linksWithMatchList,          SLOT(scrollToBottom()));

    ui->stopButton->setEnabled(false);
    ui->maxMatchLink->setReadOnly(true);

    ui->numThreads->setInputMask("D");
    ui->numLinks->setInputMask("D99");

    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    if (ui->startButton->text() == "Start")
    {
        QString text = ui->textToSearch->text();
        QString url = ui->startUrl->text();
        int nt = ui->numThreads->text().toInt();
        int nl = ui->numLinks->text().toInt();

        if (nt < 1 || nl < 1 || text.size() == 0 || url.size() == 0)
            return;

        ui->textToSearch->setReadOnly(true);
        ui->startUrl->setReadOnly(true);
        ui->numThreads->setReadOnly(true);
        ui->numLinks->setReadOnly(true);
        ui->maxMatchLink->setText("");

        ui->stopButton->setEnabled(true);

        ui->lcdNumber->display(0);
        ui->lcdNumber_2->display(0);

        ui->progressBar->setValue(0);
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(ui->numLinks->text().toInt());

        ui->processedLinksList->clear();
        ui->linksWithMatchList->clear();

        ui->startButton->setText("Pause");

        emit start(url, text, nt, nl);
    }
    else if (ui->startButton->text() == "Pause")
    {
        ui->startButton->setEnabled(false);
        ui->startButton->setText("Pausing...");

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
        ui->maxMatchLink->setCursorPosition(0);
    }

    ui->progressBar->setValue(ui->progressBar->value() + 1);
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

void MainWindow::onWorkerPaused()
{
    ui->startButton->setEnabled(true);
    ui->startButton->setText("Resume");
}

