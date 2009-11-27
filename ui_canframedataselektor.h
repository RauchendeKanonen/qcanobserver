/********************************************************************************
** Form generated from reading ui file 'canframedataselektor.ui'
**
** Created: Wed Nov 25 15:20:21 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CANFRAMEDATASELEKTOR_H
#define UI_CANFRAMEDATASELEKTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include "qwt_text_label.h"

QT_BEGIN_NAMESPACE

class Ui_CANFrameDataSelektor
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *IDlineEdit;
    QLineEdit *DatalineEdit;
    QwtTextLabel *TextLabel;
    QwtTextLabel *DataTextLabel;
    QLineEdit *GraphNamelineEdit;
    QwtTextLabel *GraphNameTextLabel;

    void setupUi(QDialog *CANFrameDataSelektor)
    {
        if (CANFrameDataSelektor->objectName().isEmpty())
            CANFrameDataSelektor->setObjectName(QString::fromUtf8("CANFrameDataSelektor"));
        CANFrameDataSelektor->resize(471, 320);
        buttonBox = new QDialogButtonBox(CANFrameDataSelektor);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(90, 270, 221, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        IDlineEdit = new QLineEdit(CANFrameDataSelektor);
        IDlineEdit->setObjectName(QString::fromUtf8("IDlineEdit"));
        IDlineEdit->setGeometry(QRect(160, 70, 61, 20));
        DatalineEdit = new QLineEdit(CANFrameDataSelektor);
        DatalineEdit->setObjectName(QString::fromUtf8("DatalineEdit"));
        DatalineEdit->setGeometry(QRect(238, 70, 91, 20));
        DatalineEdit->setAlignment(Qt::AlignCenter);
        TextLabel = new QwtTextLabel(CANFrameDataSelektor);
        TextLabel->setObjectName(QString::fromUtf8("TextLabel"));
        TextLabel->setGeometry(QRect(158, 30, 61, 20));
        DataTextLabel = new QwtTextLabel(CANFrameDataSelektor);
        DataTextLabel->setObjectName(QString::fromUtf8("DataTextLabel"));
        DataTextLabel->setGeometry(QRect(238, 30, 91, 20));
        GraphNamelineEdit = new QLineEdit(CANFrameDataSelektor);
        GraphNamelineEdit->setObjectName(QString::fromUtf8("GraphNamelineEdit"));
        GraphNamelineEdit->setGeometry(QRect(160, 110, 111, 20));
        GraphNameTextLabel = new QwtTextLabel(CANFrameDataSelektor);
        GraphNameTextLabel->setObjectName(QString::fromUtf8("GraphNameTextLabel"));
        GraphNameTextLabel->setGeometry(QRect(20, 110, 100, 20));

        retranslateUi(CANFrameDataSelektor);
        QObject::connect(buttonBox, SIGNAL(accepted()), CANFrameDataSelektor, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CANFrameDataSelektor, SLOT(reject()));

        QMetaObject::connectSlotsByName(CANFrameDataSelektor);
    } // setupUi

    void retranslateUi(QDialog *CANFrameDataSelektor)
    {
        CANFrameDataSelektor->setWindowTitle(QApplication::translate("CANFrameDataSelektor", "Dialog", 0, QApplication::UnicodeUTF8));
        IDlineEdit->setText(QApplication::translate("CANFrameDataSelektor", "0x00", 0, QApplication::UnicodeUTF8));
        DatalineEdit->setText(QApplication::translate("CANFrameDataSelektor", "XXXXXXXX", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CANFrameDataSelektor);
    } // retranslateUi

};

namespace Ui {
    class CANFrameDataSelektor: public Ui_CANFrameDataSelektor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANFRAMEDATASELEKTOR_H
