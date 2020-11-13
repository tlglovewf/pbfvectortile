#ifndef PBFSHOW_H
#define PBFSHOW_H

#include <QMainWindow>
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

public slots:
    void ClickChoose();

public:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::PbfShow *ui;
    mvt_pbf::mvtpbf_reader::GeomVector mgeoms;
};

#endif // PBFSHOW_H
