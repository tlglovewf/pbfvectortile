#include "pbfshow.h"
#include "ui_pbfshow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include <mvt_utils.hpp>
const qreal s_scale =  0.125;  //(512 / 4096.0);

#define TOTILE(V)  static_cast<qreal>((V) * s_scale)
#define TOTILEX(V) (TOTILE(V) + ui->gvCanvas->x())
#define TOTILEY(V) (TOTILE(V) + ui->gvCanvas->y())
#define TRANPT(P) QPointF(TOTILEX(P.x),TOTILEY(P.y))
PbfShow::PbfShow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PbfShow)
{
    ui->setupUi(this);
    connect(ui->btnChoose,SIGNAL(clicked()),this,SLOT(ClickChoose()));
    ui->gvCanvas->setVisible(false);
    this->move(0,0);
    std::cout << ui->gvCanvas->x() << " " << ui->gvCanvas->y() << std::endl;
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
    painter.setPen(QPen(QColor(Qt::darkGray),1,Qt::DashLine));
    painter.drawRect(ui->gvCanvas->geometry());

    //draw geometry
    if(!mgeoms.empty())
    {
        for(auto item : mgeoms)
        {

            switch (item->type())
            {
            case vtzero::GeomType::POINT :
            {
                painter.setPen(QPen(QColor(Qt::red),3));
                SETGEOMVALUE(point,vtzero::point,item);
                painter.drawPoint(TRANPT(point));

            }
                break;
            case vtzero::GeomType::LINESTRING:
            {
                painter.setPen(QPen(QColor(Qt::green),2));
                SETGEOMVALUE(line,vtzero::line,item);
                QPolygonF polyline;
                for(auto pt : line._pts)
                {
                    polyline.push_back(TRANPT(pt));
                }
                painter.drawPolyline(polyline);
            }
                break;
            case vtzero::GeomType::POLYGON:
            {
                painter.setPen(QPen(QColor(Qt::blue),1));
                SETGEOMVALUE(polygon,vtzero::polygon,item);
                for(auto ring : polygon._rs)
                {
                    QPolygonF inpoly;
                    for(auto pt : ring._l._pts)
                    {
                        inpoly.push_back(TRANPT(pt));
                    }
                    painter.drawPolygon(inpoly,Qt::FillRule::WindingFill);
                }
            }
                break;
            default:
                break;
            }
        }
    }
//    update();

}
void PbfShow::ClickChoose()
{
    QDir dir;
    QString path = QFileDialog::getOpenFileName(this,"Plese Choose a vector file","/home/tu/Downloads/.","*.pbf");
    if(!path.isEmpty())
    {
        ui->edPath->setText(path);
        mgeoms.clear();
        mvt_pbf::mvtpbf_reader(path.toStdString()).getVectileData(mgeoms);
        this->repaint();
    }
}
