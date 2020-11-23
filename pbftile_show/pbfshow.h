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
    explicit PbfShow(const QString &url, QWidget *parent = nullptr);
    ~PbfShow();

private slots:
    void ClickChoose();
    void GetReturn();

    void valueChanged(int i);
public:
    virtual void paintEvent(QPaintEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    Ui::PbfShow *ui;
    QNetworkAccessManager                   *mpNetMgr;
    PbfTileWidget                           *mpTile;
};

#endif // PBFSHOW_H
