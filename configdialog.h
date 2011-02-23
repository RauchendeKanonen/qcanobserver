#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui/QDialog>
#include <iostream>
#include <fstream>
using namespace std;
namespace Ui {
    class ConfigDialog;
}

typedef struct
{
    int MainMemByte;
    int GraphMemByte;
    int ObserverMemByte;
    int WriteToDisk;
}__config;

class ConfigDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ConfigDialog)
public:
    explicit ConfigDialog(QWidget *parent = 0);
    virtual ~ConfigDialog();
    ofstream& operator>>(ofstream& os);

    ifstream& operator<<(ifstream& is);

 signals:
         void configChanged(__config);


protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ConfigDialog *m_ui;
    void uiUpdate(void);
    int MainMemByte;
    int GraphMemByte;
    int ObserverMemByte;
    int WriteToDisk;
    void informOthers(void);
private slots:
    void on_buttonBox_accepted();
};

#endif // CONFIGDIALOG_H
