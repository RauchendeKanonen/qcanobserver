#include "suggestiondialog.h"
#include "ui_suggestiondialog.h"

SuggestionDialog::SuggestionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SuggestionDialog)
{
    ui->setupUi(this);
}

SuggestionDialog::~SuggestionDialog()
{
    delete ui;
}
