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
#include "config.h"
#include "DevDialog.h"
#include "ui_DevDialog.h"
#include "DeviceLib/candevice.h"
 #include <QDir>
#include "errordialog.h"


#ifdef LINUX
#include <errno.h>
#include <dlfcn.h>
#endif



DevDialog::DevDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DevDialog)
{
    m_ui->setupUi(this);
    confBuffer = malloc(CONFDATA_SIZEMAX);

    QDir moduldir = QDir(QString("lib"));
    QStringList filter;
#ifdef LINUX
    QString filterstr("*.so");
#endif

#ifdef WINDOWS
    QString filterstr("*.dll");
#endif
    CANLibFilePath = new QString("");
    filter.append(filterstr);
    moduldir.setNameFilters(filter);
    QFileInfoList list = moduldir.entryInfoList();

    //keine gefunden
    if(!list.count())
    {
        m_ui->comboBoxLibSelector->addItem(QString("none"), QVariant("none"));
    }

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QVariant path(fileInfo.absoluteFilePath());
        m_ui->comboBoxLibSelector->addItem(fileInfo.fileName(), path);
    }
}

DevDialog::~DevDialog()
{
    free(confBuffer);
    delete m_ui;
}

void DevDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DevDialog::on_buttonBox1_accepted()
{
    void *tmp;
    bool shareDevInstLib = false;
    QString *ErrStr = new QString(" ");
    if(CANLibFilePath == QString("none"))
    {
        QString *ErrStr = new QString("Please place the interface .so/.dll first inside of lib subdirectory!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*ErrStr);
        delete ErrStr;
        ed->setModal(true);
        ed->exec();
        delete ed;
        return;
    }

#ifdef WINDOWS
    wchar_t Filename[256];
    memset(Filename, 0, 256*sizeof(wchar_t));
    CANLibFilePath.toWCharArray(Filename);


    HINSTANCE__ *PCANDll;

    PCANDll = LoadLibrary(Filename);
    int ErrCode = GetLastError();
    if(!PCANDll)
    {
        QString *Err = new QString();
        Err->sprintf("%s %s,GetLastError() = (%d)","Could not load Device Mapper: ", CANLibFilePath.toStdString().c_str(),ErrCode);
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        delete Err;
        ed->setModal(true);
        ed->show();
        delete ed;
        return;
    }

    typedef void* (CALLBACK* createCon)(void*);
    createCon createCfg;
    createCfg = (createCon)GetProcAddress(PCANDll,"createConfig");



#endif
    //Load the interface to the hardware
    const char *libstr;
    libstr =  (char*)CANLibFilePath->toStdString().c_str();
    void* libhandle = dlopen(libstr, RTLD_LAZY);

    char *est  = dlerror();

    if(!libhandle)
    {
        ErrStr->sprintf("%s %s","Could not load Device Mapper: ", CANLibFilePath);
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*ErrStr);
        delete ErrStr;
        ed->setModal(true);
        ed->exec();
        delete ed;
        return;
    }
    void* (*createCfg)(void*);
    createCfg = (void* (*)(void*))dlsym(libhandle, "createConfig");

#ifdef LINUX
#endif

    tmp = createCfg(confBuffer);

    shareDevInstLib = m_ui->checkBoxShareLibInst->isChecked();
    if(tmp)
	emit setDev(confBuffer, *CANLibFilePath, shareDevInstLib);
}

ofstream& DevDialog::operator>>(ofstream& os)
{
    char temp[512];
    memset(temp, 0, 512);
    memcpy(temp,CANLibFilePath->toStdString().c_str(), CANLibFilePath->count());
    for(int f = 0; f < 512 ; f++)
        os.put(temp[f]);
    memset(temp, 0, 512);

    //Save Configuration Data of the device (used by the interface dll)
    for(int f = 0; f < CONFDATA_SIZEMAX ; f++)
        os.put(((char*)confBuffer)[f]);

    return os;
}
ifstream& DevDialog::operator<<(ifstream& is)
{
    int g=0;
    char temp[512];
    memset(temp, 0, 512);
    for(int f = 0; f < 512 ; f++)
        is.get(temp[f]);
    CANLibFilePath = new QString(temp);

    int idx = m_ui->comboBoxLibSelector->findData(QVariant(*CANLibFilePath),Qt::UserRole ,Qt::MatchCaseSensitive);

    if(idx == -1)
    {
        QString *ErrStr = new QString("Could not find the right interface .so/.dll in the lib subdirectory!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*ErrStr);
        delete ErrStr;
        ed->setModal(true);
        ed->exec();
        delete ed;

        m_ui->comboBoxLibSelector->setCurrentIndex(0);

        return is;
    }


    m_ui->comboBoxLibSelector->setCurrentIndex(idx);

    //Load Configuration Data of the device (used by the interface dll)
    for(int f = 0; f < CONFDATA_SIZEMAX ; f++)
        is.get(((char*)confBuffer)[f]);

    return is;
}


void DevDialog::on_comboBoxLibSelector_currentIndexChanged(int index)
{
    QVariant path = m_ui->comboBoxLibSelector->itemData(index, Qt::UserRole);
    *CANLibFilePath = path.toString();
}

void DevDialog::on_comboBoxLibSelector_activated(int index)
{
    QVariant path = m_ui->comboBoxLibSelector->itemData(index, Qt::UserRole);
    *CANLibFilePath = path.toString();
}
