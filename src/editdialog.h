#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr, const QString& fullname = "", const QString& email = "", const QDate& birthday = QDate::currentDate());
    ~EditDialog();

    QString fullname() const;
    QString email() const;
    QDate birthday() const;

private:
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
