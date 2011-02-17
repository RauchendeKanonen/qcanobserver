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
