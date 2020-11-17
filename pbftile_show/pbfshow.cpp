#include "pbfshow.h"
#include "ui_pbfshow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "pbftilewidget.h"
#include <QKeyEvent>
PbfShow::PbfShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PbfShow),mpNetMgr(new QNetworkAccessManager(this) )
{
    ui->setupUi(this);
    QNetworkAccessManager::
    connect(ui->btnChoose,SIGNAL(clicked()),this,SLOT(ClickChoose()));
    connect(ui->edPath,SIGNAL(returnPressed()),this,SLOT(GetReturn()));
    ui->gvCanvas->setVisible(false);

    mpTile = new PbfTileWidget(this->centralWidget());

    mpTile->setGeometry(ui->gvCanvas->geometry());

    mpTile->setNetWork(mpNetMgr);

    this->move(0,0);
}

PbfShow::~PbfShow()
{
    delete ui;
}

void PbfShow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setClipRect(ui->gvCanvas->geometry());

    //draw tile rectborder
    painter.setPen(QPen(QColor(Qt::darkGray),1,Qt::DashLine));
    painter.drawRect(ui->gvCanvas->geometry());

}

void PbfShow::ClickChoose()
{
    QDir dir;
    QString path = QFileDialog::getOpenFileName(this,"Plese Choose a vector file","/home/tu/Downloads/tiles/.","*.pbf");
    if(!path.isEmpty())
    {
        ui->edPath->setText(path);
        mpTile->settile(path.toStdString());
    }
}

void PbfShow::GetReturn()
{
    if(ui->edPath->text().isEmpty())
    {
        this->ClickChoose();
    }
    else {
        mpTile->settile(ui->edPath->text().toStdString(),mvt_pbf::mvtpbf_reader::ePathType::eData);
    }
}


void PbfShow::keyReleaseEvent(QKeyEvent *event)
{
    QString mpboxurl = "http://d.tiles.mapbox.com/v4/mapbox.mapbox-streets-v7/%1/%2/%3.vector.pbf?access_token=pk.eyJ1IjoibWFwYm94IiwiYSI6ImNpejY4M29iazA2Z2gycXA4N2pmbDZmangifQ.-g_vE53SD2WrJ6tFX7QHmA";

    switch (event->key()) {
        case Qt::Key_W:
    {
        const QRectF geo = ui->gvCanvas->geometry();

        const qreal half_w = (geo.width() / 2.0);
        const qreal half_h = (geo.height()/ 2.0);

        for(int i = 0;i < 2; ++i)
        {
            for(int j = 0; j < 2; ++j)
            {
                QString str = mpboxurl.arg("1",std::to_string(i).c_str(),std::to_string(j).c_str());
                auto tile = new PbfTileWidget(this->centralWidget());
                tile->setVisible(true);
                QRect igeo;
                igeo.setX(geo.x() + half_w * i);
                igeo.setY(geo.y() + half_h * j);
                igeo.setWidth(half_w);
                igeo.setHeight(half_h);
                tile->setNetWork(mpNetMgr);
                tile->setGeometry(igeo);
                tile->settile(str.toStdString(),mvt_pbf::mvtpbf_reader::ePathType::eData);
                tile->update();
                tile->raise();
            }
        }


    }
        break;
    default:
        //do nothing
        break;
    }
}
