#include "pbftilewidget.h"
#include <QMessageBox>
#include <QPainter>

qreal s_scale =  0.125;  //(512 / 4096.0);
#define TILEEXTEND 4096.0
#define TOTILE(V)  static_cast<qreal>((V) * s_scale)
#define TOTILEX(V) (TOTILE(V) )
#define TOTILEY(V) (TOTILE(V) )
#define TRANPT(P) QPointF(TOTILEX(P.x),TOTILEY(P.y))

void PbfTileWidget::settile(const std::string &path,
                             mvt_pbf::mvtpbf_reader::ePathType type /*= mvt_pbf::mvtpbf_reader::ePathType::eFile*/ )
{
    switch(type)
    {
    case mvt_pbf::mvtpbf_reader::ePathType::eFile:
        try
        {
             mvt_pbf::mvtpbf_reader(path).getVectileData(mgeoms);
        } catch (...) {
            QMessageBox::critical(this,"error","There Url Prase Error.");
        }
         this->repaint();
        break;
    case mvt_pbf::mvtpbf_reader::ePathType::eData:
    if(mpNetWork)
    {
        mpReply = mpNetWork->get(QNetworkRequest(QUrl(QString(path.c_str()).trimmed())));
        connect(mpReply,SIGNAL(readyRead() ),this,SLOT(httpReadyRead()));
        connect(mpReply,SIGNAL(finished()),this, SLOT(httpFinished()));
    }
    else
    {
        QMessageBox::warning(this,"error","Please set network first.");
    }

        break;
    default:
        assert(NULL);
    }


}

void PbfTileWidget::httpReadyRead()
{
    if(mpReply)
    {
        mTempData.append(mpReply->readAll().toStdString());
    }
}

void PbfTileWidget::httpFinished()
{
    mvt_pbf::mvtpbf_reader(mTempData,
                           mvt_pbf::mvtpbf_reader::ePathType::eData).getVectileData(mgeoms);
    this->repaint();
    mpReply->deleteLater();
    mpReply = nullptr;
    mTempData.clear();
}

void PbfTileWidget::paintEvent(QPaintEvent *event)
{
      QPainter painter(this);
      s_scale = ((this->width() + this->height()) >> 1) / TILEEXTEND;
      // 反走样
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.setPen(QPen(QColor(Qt::darkGray),1,Qt::DashLine));
      painter.drawRect(this->rect());
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
 }


