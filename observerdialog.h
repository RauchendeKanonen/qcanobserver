#ifndef OBSERVERDIALOG_H
#define OBSERVERDIALOG_H

#include <QtGui/QDialog>
#include "processdatabase.h"
#include "candataitemselector.h"
#include "messagebufferinterface.h"
#include "stringlistmodel.h"

namespace Ui {
    class ObserverDialog;
}

class ObserveItems
{
    public:
    ObserveItems(int AID, CanFrameRuleSet* ARuleSet, int ARule, QColor AColor)
    {
        ID = AID;
        Rule= ARule;
        Color = new QColor(AColor);
        RuleSet = ARuleSet;
    }
    CanFrameRuleSet* RuleSet;
    QColor *Color;
    int ID;
    int Rule;
};



class ObserverDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ObserverDialog)
public:
    explicit ObserverDialog(QWidget *parent = 0, QList<CanFrameRuleSet*> *RuleList = 0);
    virtual ~ObserverDialog();

public slots:
    void MainTimerSlot(void);
    void addItemToObserve(CanFrameRuleSet*, int, QColor);
    void deleteItemToObserve(CanFrameRuleSet*, int);
    void newMessage(CANMsgandTimeStruct *, int);
    void ClearAll();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ObserverDialog *m_ui;
    QList<CanFrameRuleSet*> *pRuleList;
    QList<ObserveItems*> CANItems;
    CANDataItemSelector *Sel;
    StringListModel *TraceModel;

private slots:
    void on_pushButton_clicked();
};

#endif // OBSERVERDIALOG_H
