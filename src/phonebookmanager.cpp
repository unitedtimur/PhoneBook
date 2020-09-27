#include "phonebookmanager.h"
#include "ui_phonebookmanager.h"
#include "config.h"
#include "addressmodel.h"
#include "editdialog.h"

#include <QDate>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QFile>

PhoneBookManager::PhoneBookManager(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::PhoneBookManager),
    addressModel(new AddressModel())
{
    ui->setupUi(this);

    // Setup geometry the form
    this->readSettings();

    // Initialize the main form
    this->init();

    // Setup model with tableView
    this->setup();

    // Initialize connections
    this->initConnections();

    // For tests
    this->testData();
}

PhoneBookManager::~PhoneBookManager()
{
    // Save geometry
    this->writeSettings();

    delete ui;
}

void PhoneBookManager::init()
{
    this->setWindowTitle(config::MAIN_TITLE);
    this->setMinimumSize(QSize(600, 600));

    QRegExp regExp("(0[1-9]|[12][0-9]|3[01]{1,2}).(0[1-9]|[12]{1,2}).(19[0-9][0-9]|20[0-9][0-9])");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    this->ui->birthday->setValidator(validator);
}

void PhoneBookManager::setup()
{
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(addressModel);
    proxyModel->setFilterKeyColumn(0);

    this->ui->tableView->setModel(proxyModel);
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableView->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableView->setSortingEnabled(true);
}

void PhoneBookManager::initConnections()
{
    connect(this->ui->addButton, &QPushButton::clicked, this, &PhoneBookManager::addEntry);
    connect(this->ui->remButton, &QPushButton::clicked, this, &PhoneBookManager::remEntry);
    connect(this->ui->editButton, &QPushButton::clicked, this, &PhoneBookManager::editEntry);
    connect(this->ui->searchLineEdit, &QLineEdit::textChanged, this, &PhoneBookManager::searchEntry);
    connect(this->ui->resetButton, &QPushButton::clicked, this, &PhoneBookManager::testData);
}

void PhoneBookManager::addEntry()
{
    if (!this->addressModel->appendContact(ui->fullname->text(),
                                      ui->email->text(),
                                      QDate::fromString(ui->birthday->text(), config::DATE_FRMT),
                                      QDate::currentDate()))
    {
        QMessageBox::critical(this, config::ER_TITLE, config::ER_EXIST);
    }
}

void PhoneBookManager::remEntry()
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    QItemSelectionModel* selectionModel = this->ui->tableView->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();

    for (QModelIndex index : indexes)
    {
        int row = proxy->mapToSource(index).row();
        this->addressModel->removeRows(row, 1, QModelIndex());
    }
}

void PhoneBookManager::editEntry()
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    QItemSelectionModel* selectionModel = this->ui->tableView->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();

    QString fullname;
    QString email;
    QDate birthday;
    QDate dateentry;

    int row = -1;

    if (indexes.empty())
        return;

    for (const auto& index : indexes)
    {
        row = proxy->mapToSource(index).row();

        QModelIndex fullnameIndex = this->addressModel->index(row, 1, QModelIndex());
        QVariant varFullname = this->addressModel->data(fullnameIndex, Qt::DisplayRole);
        fullname = varFullname.toString();

        QModelIndex emailIndex = this->addressModel->index(row, 2, QModelIndex());
        QVariant varEmail = this->addressModel->data(emailIndex, Qt::DisplayRole);
        email = varEmail.toString();

        QModelIndex birthdayIndex = this->addressModel->index(row, 3, QModelIndex());
        QVariant varBirthday = this->addressModel->data(birthdayIndex, Qt::DisplayRole);
        birthday = varBirthday.toDate();

        QModelIndex dateentryIndex = this->addressModel->index(row, 4, QModelIndex());
        QVariant varDateentry = this->addressModel->data(dateentryIndex, Qt::DisplayRole);
        dateentry = varDateentry.toDate();
    }

    EditDialog editDialog(this, fullname, email, birthday);

    if (editDialog.exec())
    {
        const QString newFullname = editDialog.fullname();
        const QString newEmail = editDialog.email();
        const QDate newBirthday = editDialog.birthday();

        if (newFullname != fullname || newEmail != email || newBirthday != birthday)
        {
            const QModelIndex newFullnameIndex = this->addressModel->index(row, 1, QModelIndex());
            this->addressModel->setData(newFullnameIndex, newFullname, Qt::EditRole);

            const QModelIndex newEmailIndex = this->addressModel->index(row, 2, QModelIndex());
            this->addressModel->setData(newEmailIndex, newEmail, Qt::EditRole);

            const QModelIndex newBirthdayIndex = this->addressModel->index(row, 3, QModelIndex());
            this->addressModel->setData(newBirthdayIndex, newBirthday, Qt::EditRole);

            const QModelIndex newDateEntry = this->addressModel->index(row, 4, QModelIndex());
            this->addressModel->setData(newDateEntry, QDate::currentDate(), Qt::EditRole);
        }
    }
}

void PhoneBookManager::searchEntry(const QString& text)
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    proxy->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::FixedString));
    proxy->setFilterKeyColumn(1);
}

void PhoneBookManager::writeSettings()
{
    QSettings settings(config::ORGANIZATION, config::APPLICATION);

    settings.beginGroup("PhoneBook");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void PhoneBookManager::readSettings()
{
    QSettings settings(config::ORGANIZATION, config::APPLICATION);

    settings.beginGroup("PhoneBook");
    this->resize(settings.value("size", QSize(600, 600)).toSize());
    this->move(settings.value("pos", QPoint(600, 600)).toPoint());
    settings.endGroup();
}

void PhoneBookManager::testData()
{
    const auto addContact = [&](const QString& fullname, const QString& email, const QString& birthday, const QString& dateentry)
    {
        this->addressModel->appendContact(fullname,
                                          email,
                                          QDate::fromString(birthday, config::DATE_FRMT),
                                          QDate::fromString(dateentry, config::DATE_FRMT)
        );
    };

    addContact("Альбертов А.К.", "alb@work.ru", "11.05.1960", "27.09.2020");
    addContact("Вазгенов Б.С.", "basgen@aha.com", "11.11.2011", "15.04.1999");
    addContact("Лихоманов Т.Д.", "lihomanov.t@inbox.ru", "17.12.1998", "27.09.2020");
    addContact("Кумириджон А.С.", "kumir@tadj.ik", "05.05.2005", "13.04.2013");
    addContact("Ингус Р.Р.", "ingus@com.ru", "13.01.1950", "11.11.2011");
    addContact("Иванов И.И.", "ivan@ivan.ru", "04.07.2009", "03.07.2000");
    addContact("Киреев В.П.", "kir@mail.ru", "04.05.1997", "03.02.2020");
    addContact("Тестов Т.Т.", "test@test.com", "01.08.1992", "01.05.2000");
    addContact("Провкин В.З.", "azaza@prov.com", "03.03.1994", "25.12.2019");
    addContact("Твардов И.Г.", "tvard@ya.ru", "01.02.1988", "04.04.2020");
}
