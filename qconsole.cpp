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
#include "qconsole.h"
#include <QString>

QConsole::QConsole()

{
    this->setReadOnly(true);
    QTextEdit::QTextEdit(NULL);
}


void QConsole::keyPressEvent(QKeyEvent* e)
{
    QTextEdit::keyPressEvent(e);
}
void QConsole::setCmdLine(QString Cmd)
{
    moveCursor(QTextCursor::End);
}

void  QConsole::insertText(char *text, QColor col, int len)
{
    QString TPeace;
    char formatted[len+1];
    memcpy(formatted, text, len);
    formatted[len] = 0;
    TPeace.sprintf("%s", formatted);
    this->setTextColor(col);
    this->insertPlainText(TPeace);
    this->ensureCursorVisible();
}



void QConsole::keyReleaseEvent(QKeyEvent* e)
{

}
