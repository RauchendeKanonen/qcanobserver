#ifndef SUGGESTIONDIALOG_H
#define SUGGESTIONDIALOG_H

#include <QDialog>

namespace Ui {
class SuggestionDialog;
}

class SuggestionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SuggestionDialog(QWidget *parent = 0);
    ~SuggestionDialog();
    
private:
    Ui::SuggestionDialog *ui;
};

#endif // SUGGESTIONDIALOG_H
