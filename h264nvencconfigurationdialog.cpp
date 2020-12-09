#include "h264nvencconfigurationdialog.h"
#include "ui_h264nvencconfigurationdialog.h"

h264NVENCConfigurationDialog::h264NVENCConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h264NVENCConfigurationDialog)
{
    ui->setupUi(this);

    /**
     * This Code deals with the Windows Size scaling
     */
    QDesktopWidget dw;
    int width=dw.width()*0.3;
    int height=dw.height()*0.7;
    this->setFixedSize(width, height);

    /**
     * Ensures labels equal size
     */
    int maxWidth = 0;
    for (QLabel* label : findChildren<QLabel*>()) {
        if (maxWidth < label->width())
            maxWidth = label->width();
    }
    for (QLabel* label : findChildren<QLabel*>()) {
        if( label->objectName() != "h264_nvenc_title_label")
        label->setFixedWidth(maxWidth);
    }

    /**
      * Ensure buttons equal size
      */
    maxWidth = 0;
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        if (maxWidth < btn->width())
            maxWidth = btn->width();
    }
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        btn->setFixedWidth(maxWidth);
    }

    config = new ConfigurationManager("/assets/h264NVENC.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    /*Initialize UI Elements values*/
    ui->h264_nvenc_encoding_speed_combobox->addItem(ENCODER_SPEED_LLHP_LABEL);
    ui->h264_nvenc_encoding_speed_combobox->addItem(ENCODER_SPEED_LLHQ_LABEL);

    ui->h264_nvenc_process_priority_combobox->addItems(SYS_PRIORITY_OP_LIST);

    ui->h264_nvenc_frame_threads_combobox->addItems(FRAME_THREADS_OPT_LIST);
    ui->h264_nvenc_pool_threads_combobox->addItems(POOL_THREADS_OPT_LIST);

    ui->h264_nvenc_rate_control_combobox->addItems(RATE_CONTROL_OPT_LIST);

    ui->h264_nvenc_fec_percentage_combobox->addItems(FEC_OPT_LIST);

    setLoadedValues();
}

h264NVENCConfigurationDialog::~h264NVENCConfigurationDialog()
{
    delete ui;
}

void h264NVENCConfigurationDialog::setLoadedValues() {
    if(config->getKey("nv_preset") == ENCODER_SPEED_LLHP) {
        ui->h264_nvenc_encoding_speed_combobox->setCurrentIndex(0);
        entries_snapshot.insert("nv_preset", ENCODER_SPEED_LLHP);
    }
    else if(config->getKey("nv_preset") == ENCODER_SPEED_LLHQ) {
        ui->h264_nvenc_encoding_speed_combobox->setCurrentIndex(1);
        entries_snapshot.insert("nv_preset", ENCODER_SPEED_LLHQ);
    }

    /*System Priority*/
    /*Compare against the configuration values (1,2,3)*/
    if(config->getKey("system_priority") == SYS_PRIORITY_ABOVE_NORMAL) {
       ui->h264_nvenc_process_priority_combobox->setCurrentIndex(0);
       entries_snapshot.insert("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_HIGH) {
        ui->h264_nvenc_process_priority_combobox->setCurrentIndex(1);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_REAL_TIME) {
        ui->h264_nvenc_process_priority_combobox->setCurrentIndex(2);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_REAL_TIME);
    }

    /*Frame threads*/
    if(config->getKey("min_threads") == FRAME_THREADS_2) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("min_threads", FRAME_THREADS_2);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_4) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("min_threads", FRAME_THREADS_4);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_6) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("min_threads", FRAME_THREADS_6);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_8) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("min_threads", FRAME_THREADS_8);
    }

    /*Pool threads*/
    if(config->getKey("pools") == POOL_THREADS_2) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("pools", POOL_THREADS_2);
    }
    else if(config->getKey("pools") == POOL_THREADS_4) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("pools", POOL_THREADS_4);
    }
    else if(config->getKey("pools") == POOL_THREADS_6) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("pools", POOL_THREADS_6);
    }
    else if(config->getKey("pools") == POOL_THREADS_8) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("pools", POOL_THREADS_8);
    }

    /*Rate control*/
    if(config->getKey("nv_rc") == RATE_CONTROL_AUTO) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(0);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_AUTO);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_CBR) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(1);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_CBR);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_CONSTANT_QP) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(2);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_CONSTANT_QP);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_VBR_HIGH_QUALITY) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(3);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_VBR_HIGH_QUALITY);
    }

    /*FEC*/
    if(config->getKey("fec_percentage") == FEC_10) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(0);
        entries_snapshot.insert("fec_percentage", FEC_10);
    }
    else if(config->getKey("fec_percentage") == FEC_20) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(1);
        entries_snapshot.insert("fec_percentage", FEC_20);
    }
    else if(config->getKey("fec_percentage") == FEC_30) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(2);
        entries_snapshot.insert("fec_percentage", FEC_30);
    }
    else if(config->getKey("fec_percentage") == FEC_40) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(3);
        entries_snapshot.insert("fec_percentage", FEC_40);
    }
}
