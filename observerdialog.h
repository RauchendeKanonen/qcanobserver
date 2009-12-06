#ifndef OBSERVERDIALOG_H
#define OBSERVERDIALOG_H

#include <QtGui/QDialog>
#include "processdatabase.h"
#include "candataitemselector.h"
#include "messagebufferinterface.h"
#include "stringlistmodel.h"
#include "cansignalcollection.h"

namespace Ui {
    class ObserverDialog;
}

class ObserveItems
{
    public:
    ObserveItems(CANSignal* ASignal, QColor *AColor)
    {
        Color = AColor;
        Signal = ASignal;
    }
    CANSignal* Signal;
    QColor *Color;
};



class ObserverDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ObserverDialog)
public:
    explicit ObserverDialog(QWidget *parent = 0, CANSignalCollection *Collection = 0);
    virtual ~ObserverDialog();

public slots:
    void MainTimerSlot(void);
    void addItemToObserve(CANSignal* Signal, QColor Color);
    void deleteItemToObserve(CANSignal* Signal);
    void newMessage(CANMsgandTimeStruct *, int);
    void ClearAll();

protected:
    virtual void changeEvent(QEvent *e);

private:
    CANSignalCollection *pCollection;
    Ui::ObserverDialog *m_ui;
    QList<ObserveItems*> CANItems;
    CANDataItemSelector *Sel;
    StringListModel *TraceModel;

private slots:
    void on_pushButton_clicked();
};

#endif // OBSERVERDIALOG_H
