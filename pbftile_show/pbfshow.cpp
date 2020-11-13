#include "pbfshow.h"
#include "ui_pbfshow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"

#include "vector_tile.hpp"

const qreal s_scale =  0.125;  //(512 / 4096.0);

#define TOTILE(V) ((V) * s_scale)

PbfShow::PbfShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PbfShow)
{
    ui->setupUi(this);
    connect(ui->btnChoose,SIGNAL(clicked()),this,SLOT(ClickChoose()));
    ui->gvCanvas->setVisible(false);
}

PbfShow::~PbfShow()
{
    delete ui;
}

void PbfShow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

    //draw tile rectborder
    painter.setPen(QPen(QColor(Qt::blue),1));
    painter.drawRect(ui->gvCanvas->geometry());

    QPointF pts[] = {QPointF(TOTILE(2365),TOTILE(1127)),
                     QPointF(TOTILE(2380),TOTILE(1181)),
                     QPointF(TOTILE(2416),TOTILE(1171)),
                     QPointF(TOTILE(2402),TOTILE(1117)),
                     QPointF(TOTILE(2365),TOTILE(1127))};
    //draw geometry
    painter.setPen(QPen(QColor(Qt::red),1));
    painter.drawPolygon(pts,5);

}
void PbfShow::ClickChoose()
{
    QDir dir;
    QString path = QFileDialog::getOpenFileName(this,"Plese Choose a vector file","./","*.pbf");
    if(!path.isEmpty())
    {
       ui->edPath->setText(path);

    }
}
