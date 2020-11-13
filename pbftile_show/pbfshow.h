#ifndef PBFSHOW_H
#define PBFSHOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <memory>
#include "mvt_utils.hpp"
namespace Ui {
class PbfShow;
}



class PbfShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PbfShow(QWidget *parent = nullptr);
    ~PbfShow();

    void requestUrl(const QUrl &url);

private slots:
    void ClickChoose();
    void GetReturn();
    void httpFinished();
    void httpReadyRead();
public:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::PbfShow *ui;
    std::unique_ptr<QNetworkAccessManager>  mpNetMgr;
    QNetworkReply                           *mpReply;
    mvt_pbf::mvtpbf_reader::GeomVector      mgeoms;
};

#endif // PBFSHOW_H
