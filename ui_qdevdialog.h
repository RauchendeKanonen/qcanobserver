/********************************************************************************
** Form generated from reading ui file 'qdevdialog.ui'
**
** Created: Wed Nov 11 10:40:12 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QDEVDIALOG_H
#define UI_QDEVDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <qdevdialog.h>

QT_BEGIN_NAMESPACE

class Ui_QDevDialog
{
public:
    QDialogButtonBox *buttonBox;
    QDevDialog *lineEdit;

    void setupUi(QDialog *QDevDialog)
    {
        if (QDevDialog->objectName().isEmpty())
            QDevDialog->setObjectName(QString::fromUtf8("QDevDialog"));
        QDevDialog->resize(320, 148);
        buttonBox = new QDialogButtonBox(QDevDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(60, 90, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(QDevDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(100, 40, 113, 20));

        retranslateUi(QDevDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), QDevDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QDevDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(QDevDialog);
    } // setupUi

    void retranslateUi(QDialog *QDevDialog)
    {
        QDevDialog->setWindowTitle(QApplication::translate("QDevDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        //lineEdit->setText(QApplication::translate("QDevDialog", "/dev/pcanusb0", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(QDevDialog);
    } // retranslateUi

};

namespace Ui {
    class QDevDialog: public Ui_QDevDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDEVDIALOG_H
