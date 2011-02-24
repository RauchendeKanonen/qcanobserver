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
#ifndef QCADPLOTCONSOLE_H
#define QCADPLOTCONSOLE_H
#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>
#include <QStringList>
//#include <QStyleSheet>
#include <QColor>
class QConsole : public QTextEdit
{
    Q_OBJECT
signals:
    void ConsoleCommand(QString cmdline);

public:
    void setCmdLine(QString Cmd);
    QConsole();
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void insertText(char *text, QColor col, int len);


private:
    QStringList Lines;
};

#endif // QCADPLOTCONSOLE_H
