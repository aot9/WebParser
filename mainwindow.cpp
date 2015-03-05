#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include "worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inProgressListModel(new QStringListModel),
    completedListModel(new QStringListModel),
    isRunning(false)
{
    ui->setupUi(this);

    this->setFixedSize(890, 490);

    ui->listView->setModel(inProgressListModel);
    ui->listView_2->setModel(completedListModel);

    ui->lineEdit_3->setInputMask("D");
    ui->lineEdit_4->setInputMask("D99");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setWorkerPtr(Worker *ptr)
{
    workerPtr = ptr;
}

void MainWindow::on_pushButton_clicked()
{
    if (!isRunning)
    {
        ui->lineEdit->setReadOnly(true);// text
        ui->lineEdit_5->setReadOnly(true);// start url
        ui->lineEdit_3->setReadOnly(true);// threads
        ui->lineEdit_4->setReadOnly(true);// links

        isRunning = true;
        ui->pushButton->setText("Pause");

        emit startBtnClicked(ui->lineEdit_5->text(), ui->lineEdit->text(), ui->lineEdit_3->text().toInt(), ui->lineEdit_4->text().toInt());

        return;
    }
    if (ui->pushButton->text() == "Pause")
    {
         ui->pushButton->setText("Resume");
         workerPtr->pause = true;
    }
    else if (ui->pushButton->text() == "Resume")
    {
       ui->pushButton->setText("Pause");
       workerPtr->pause = false;
    }
}

void MainWindow::updateLists(QString processedLink, int counter)
{
    inProgressListModel->insertRow(inProgressListModel->rowCount());
    QModelIndex index = inProgressListModel->index(inProgressListModel->rowCount()-1);
    inProgressListModel->setData(index, processedLink);

    if (counter > 0)
    {
        completedListModel->insertRow(completedListModel->rowCount());
        index = completedListModel->index(completedListModel->rowCount()-1);
        completedListModel->setData(index, processedLink);
    }

    ui->lcdNumber->display(ui->lcdNumber->value() + counter);
}

void MainWindow::onWorkerFinished()
{
    ui->lcdNumber->display(0);

    ui->lineEdit->setReadOnly(false);
    ui->lineEdit_5->setReadOnly(false);
    ui->lineEdit_3->setReadOnly(false);
    ui->lineEdit_4->setReadOnly(false);

    ui->pushButton->setText("Start");

    isRunning = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    workerPtr->stop = 1;
    workerPtr->pause = 0;
}


