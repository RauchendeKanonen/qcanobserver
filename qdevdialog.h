#ifndef QDEVDIALOG_H
#define QDEVDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class QDevDialog;
}

class QDevDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(QDevDialog)
public:
    explicit QDevDialog(QWidget *parent = 0);
    virtual ~QDevDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::QDevDialog *m_ui;
    QString DevPath;

signals:
    void setDev(QString PathArg);

private slots:
    void on_lineEdit_textEdited(QString );
    void on_buttonBox_accepted();
};

#endif // QDEVDIALOG_H
