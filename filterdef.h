#ifndef FILTERDEF_H
#define FILTERDEF_H

#include <QtGui/QDialog>

namespace Ui {
    class Filterdef;
}

class Filterdef : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Filterdef)
public:
    explicit Filterdef(QWidget *parent = 0);
    virtual ~Filterdef();


protected:
    virtual void changeEvent(QEvent *e);



private:
    Ui::Filterdef *m_ui;

signals:
    void acceptedFilter(int from, int to);

private slots:
    void on_buttonBox_accepted();
};

#endif // FILTERDEF_H
