#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include "worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inProgressListModel(new QStringListModel),
    completedListModel(new QStringListModel),
    maxMatch(0)
{
    ui->setupUi(this);

    //this->setFixedSize(890, 490);
    ui->pushButton_2->setEnabled(false);

    ui->listView->setModel(inProgressListModel);
    ui->listView_2->setModel(completedListModel);

    ui->lineEdit_3->setInputMask("D");
    ui->lineEdit_4->setInputMask("D99");

    ui->lineEdit_2->setReadOnly(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setReadOnly(true);// text
    ui->lineEdit_5->setReadOnly(true);// start url
    ui->lineEdit_3->setReadOnly(true);// threads
    ui->lineEdit_4->setReadOnly(true);// links

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);

    ui->lcdNumber->display(0);

    inProgressListModel->removeRows(0, inProgressListModel->rowCount());
    completedListModel->removeRows(0, completedListModel->rowCount());

    emit startBtnClicked(ui->lineEdit_5->text(), ui->lineEdit->text(), ui->lineEdit_3->text().toInt(), ui->lineEdit_4->text().toInt());
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

    if (counter > maxMatch)
    {
        ui->lcdNumber_2->display(counter);
        maxMatch = counter;
        ui->lineEdit_2->setText(processedLink);
    }

    ui->lcdNumber->display(ui->lcdNumber->value() + counter);
}

void MainWindow::on_pushButton_2_clicked()
{
    emit stopBtnPressed();
}

void MainWindow::onWorkerStopped()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);

    ui->lineEdit->setReadOnly(false);
    ui->lineEdit_5->setReadOnly(false);
    ui->lineEdit_3->setReadOnly(false);
    ui->lineEdit_4->setReadOnly(false);


}


