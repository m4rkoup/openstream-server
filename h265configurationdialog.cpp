#include "h265configurationdialog.h"
#include "ui_h265configurationdialog.h"
#include <QDesktopWidget>

h265ConfigurationDialog::h265ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h265ConfigurationDialog)
{
    ui->setupUi(this);
    /**
     * This Code deals with the Windows Size scaling
     */
    QDesktopWidget dw;
    int width=dw.width()*0.3;
    int height=dw.height()*0.7;
    this->setFixedSize(width, height);

    int maxWidth = 0;
    for (QLabel* label : findChildren<QLabel*>()) {
        if (maxWidth < label->width())
            maxWidth = label->width();
    }
    for (QLabel* label : findChildren<QLabel*>()) {
        if( label->objectName() != "h265_cpu_title_label"
                && label->objectName() != "h265_cpu_info_label" )
        label->setFixedWidth(maxWidth);
    }

    maxWidth = 0;
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        if (maxWidth < btn->width())
            maxWidth = btn->width();
    }
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        btn->setFixedWidth(maxWidth);
    }
}

h265ConfigurationDialog::~h265ConfigurationDialog()
{
    delete ui;
}
