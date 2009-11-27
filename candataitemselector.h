#ifndef CANDATAITEMSELECTOR_H
#define CANDATAITEMSELECTOR_H
#include "processdatabase.h"
#include <QtGui/QDialog>

namespace Ui {
    class CANDataItemSelector;
}

class CANDataItemSelector : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(CANDataItemSelector)
public:
    explicit CANDataItemSelector(QWidget *parent = 0, QList<CalRule*> *RuleList=0);
    virtual ~CANDataItemSelector();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::CANDataItemSelector *m_ui;
    QList<CalRule*> *pRuleList;
signals:
    void addItemToDraw(CalRule*, int, QColor);
private slots:
    void on_AddItem_clicked();
    void on_pushButton_clicked();
    void on_ComboItemSelector_currentIndexChanged(int index);
    void on_ComboItemSelector_highlighted(int index);
};

#endif // CANDATAITEMSELECTOR_H
