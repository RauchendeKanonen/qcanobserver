#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ErrorDialog;
}

class ErrorDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ErrorDialog)
public:
    explicit ErrorDialog(QWidget *parent = 0);
    virtual ~ErrorDialog();
    void SetErrorMessage(QString Message);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ErrorDialog *m_ui;
};

#endif // ERRORDIALOG_H
