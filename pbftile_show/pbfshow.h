#ifndef PBFSHOW_H
#define PBFSHOW_H

#include <QMainWindow>

namespace Ui {
class PbfShow;
}

class PbfShow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PbfShow(QWidget *parent = nullptr);
    ~PbfShow();

private:
    Ui::PbfShow *ui;
};

#endif // PBFSHOW_H
