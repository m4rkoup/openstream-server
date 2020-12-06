#include "openstreammainwindow.h"
#include "ui_openstreammainwindow.h"

OpenstreamMainWindow::OpenstreamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenstreamMainWindow)
{
    ui->setupUi(this);
}

OpenstreamMainWindow::~OpenstreamMainWindow()
{
    delete ui;
}
