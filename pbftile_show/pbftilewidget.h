#ifndef PBFTILEWIDGET_H
#define PBFTILEWIDGET_H

#include <QWidget>
#include "mvt_utils.hpp"

class PbfTileWidget : public QWidget
{
public:
    PbfTileWidget(QWidget *parent = NULL):QWidget(parent){}
    ~PbfTileWidget(){}

    void set_tile(const std::string &data, mvt_pbf::mvtpbf_reader::ePathType type = mvt_pbf::mvtpbf_reader::ePathType::eFile);
protected:
    void paintEvent(QPaintEvent *event);


protected:
    mvt_pbf::mvtpbf_reader::GeomVector      mgeoms;
};


#endif // PBFTILEWIDGET_H
