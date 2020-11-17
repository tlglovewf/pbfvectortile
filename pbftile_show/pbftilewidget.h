#ifndef PBFTILEWIDGET_H
#define PBFTILEWIDGET_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <memory>
#include "mvt_utils.hpp"

class PbfTileWidget : public QWidget
{
    Q_OBJECT
public:
    PbfTileWidget(QWidget *parent = NULL):QWidget(parent),mpNetWork(NULL){}
    ~PbfTileWidget(){}

    void setNetWork(QNetworkAccessManager *network)
    {
        mpNetWork = network;
    }

    void settile(const std::string &data, mvt_pbf::mvtpbf_reader::ePathType type = mvt_pbf::mvtpbf_reader::ePathType::eFile);

private slots:
    void httpFinished();
    void httpReadyRead();

protected:
    void paintEvent(QPaintEvent *event);


protected:
    mvt_pbf::mvtpbf_reader::GeomVector       mgeoms;
    QNetworkAccessManager                   *mpNetWork;
    QNetworkReply                           *mpReply;
    std::string                              mTempData;
};


#endif // PBFTILEWIDGET_H
