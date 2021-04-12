#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnExit, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(setTextHello()));
    qDebug()<<"Hello world";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btbClick_clicked()
{
    ui->lblTxt->setText("Hello Vinh");
    return;
}

void MainWindow::on_btbClick_pressed()
{

}

void MainWindow:: setTextHello(){
    ui->lblText->setText("txt");
}
