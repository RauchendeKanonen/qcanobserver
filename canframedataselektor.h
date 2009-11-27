#ifndef CANFRAMEDATASELEKTOR_H
#define CANFRAMEDATASELEKTOR_H

#include <QtGui/QDialog>

namespace Ui {
    class CANFrameDataSelektor;
}

class CANFrameDataSelektor : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(CANFrameDataSelektor)
public:
    explicit CANFrameDataSelektor(QWidget *parent = 0);
    virtual ~CANFrameDataSelektor();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::CANFrameDataSelektor *m_ui;
};

#endif // CANFRAMEDATASELEKTOR_H
