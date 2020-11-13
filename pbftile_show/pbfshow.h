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

public slots:
    void ClickChoose();

public:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::PbfShow *ui;
};

#endif // PBFSHOW_H
