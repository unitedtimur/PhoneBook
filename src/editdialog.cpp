#include "editdialog.h"
#include "ui_editdialog.h"
#include "config.h"

#include <QRegExpValidator>

EditDialog::EditDialog(QWidget *parent, const QString &fullname, const QString &email, const QDate &birthday) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);

    QRegExp regExp("(0[1-9]|[12][0-9]|3[01]{1,2}).(0[1-9]|[12]{1,2}).(19[0-9][0-9]|20[0-9][0-9])");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    this->ui->birthday->setValidator(validator);

    ui->fullname->setText(fullname);
    ui->email->setText(email);
    ui->birthday->setText(birthday.toString(config::DATE_FRMT));
}

EditDialog::~EditDialog()
{
    delete ui;
}

QString EditDialog::fullname() const
{
    return this->ui->fullname->text();
}

QString EditDialog::email() const
{
    return this->ui->email->text();
}

QDate EditDialog::birthday() const
{
    return QDate::fromString(this->ui->birthday->text(), config::DATE_FRMT);
}
