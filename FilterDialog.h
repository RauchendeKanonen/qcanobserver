#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QtGui/QDialog>
#include "filterdef.h"
#include <QListWidget>

#define HWFILTER 1
#define BEHINDDBFILTER 2
#define GRAPHICWINFILTER 3


namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FilterDialog)
public:
    explicit FilterDialog(QWidget *parent = 0);
    virtual ~FilterDialog();

protected:
    virtual void changeEvent(QEvent *e);

public slots:
    void acceptedFilter(int from, int to);

signals:
    void setFilter(int Place, int from, int to);

private:
    QListWidget *aktEditLst;
    Ui::FilterDialog *m_ui;
    Filterdef *FilterDef;
private slots:
    void on_pushButton_clicked();
    void on_DelGraphicInputFilter_clicked();
    void on_DelDataVaseOutputFilter_clicked();
    void on_AddGraphicInputFilter_clicked();
    void on_AddDataBaseOutputFilter_clicked();
    void on_DelInputFilter_clicked();
    void on_AddInputFilter_clicked();
};

#endif // FILTERDIALOG_H
