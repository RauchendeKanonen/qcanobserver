#ifndef OBSERVERDIALOG_H
#define OBSERVERDIALOG_H

#include <QtGui/QDialog>
#include "processdatabase.h"
#include "signalselectordialog.h"
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
    ~ObserveItems()
    {
        delete Color;
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
    ofstream& operator>>(ofstream& os);
    ifstream& operator<<(ifstream& is);
public slots:
    void MainTimerSlot(void);
    void addItemToObserve(CANSignal* Signal, QColor Color);
    void deleteItemToObserve(CANSignal* Signal);
    void newMessage(_CANMsg *, int);
    void ClearAll();

protected:
    virtual void changeEvent(QEvent *e);

private:
    CANSignalCollection *pCollection;
    Ui::ObserverDialog *m_ui;
    QList<ObserveItems*> CANItems;
    SignalSelectorDialog *Sel;
    StringListModel *TraceModel;
    void closeEvent( QCloseEvent *e );
    QWidget *pparent;

private slots:
    void on_ConnectedcheckBox_toggled(bool checked);
    void on_pushButton_clicked();
};

#endif // OBSERVERDIALOG_H
