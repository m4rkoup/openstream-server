#ifndef H265CONFIGURATIONDIALOG_H
#define H265CONFIGURATIONDIALOG_H

#include <QDialog>

namespace Ui {
class h265ConfigurationDialog;
}

class h265ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h265ConfigurationDialog(QWidget *parent = nullptr);
    ~h265ConfigurationDialog();

private:
    Ui::h265ConfigurationDialog *ui;
};

#endif // H265CONFIGURATIONDIALOG_H
