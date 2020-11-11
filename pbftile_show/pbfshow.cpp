#include "pbfshow.h"
#include "ui_pbfshow.h"

PbfShow::PbfShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PbfShow)
{
    ui->setupUi(this);
}

PbfShow::~PbfShow()
{
    delete ui;
}
