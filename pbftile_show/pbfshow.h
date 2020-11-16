#ifndef PBFSHOW_H
#define PBFSHOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <memory>
#include "mvt_utils.hpp"
namespace Ui {
class PbfShow;
}

class PbfTileWidget;

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
    void keyReleaseEvent(QKeyEvent *event);
private:
    Ui::PbfShow *ui;
    std::unique_ptr<QNetworkAccessManager>  mpNetMgr;
    QNetworkReply                           *mpReply;
    PbfTileWidget                           *mpTile;
    std::string                             mNetData;
};

#endif // PBFSHOW_H
