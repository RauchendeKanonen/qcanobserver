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
