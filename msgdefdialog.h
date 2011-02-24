/*
    QCANObserver - A CAN Sniffer
    Copyright (C) <2009>  <Netseal>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MSGDEFDIALOG_H
#define MSGDEFDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MsgDefDialog;
}

class MsgDefDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(MsgDefDialog)
public:
    explicit MsgDefDialog(QWidget *parent = 0, QString ID = 0 );
    virtual ~MsgDefDialog();

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString);
private:
    Ui::MsgDefDialog *m_ui;

private slots:
    void on_checkBoxTrigger_toggled(bool checked);
    void on_checkBox_toggled(bool checked);
    void on_buttonBox_accepted();
};

#endif // MSGDEFDIALOG_H
