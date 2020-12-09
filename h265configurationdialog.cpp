#include "h265configurationdialog.h"
#include "ui_h265configurationdialog.h"
#include <QDesktopWidget>
#include <QCoreApplication>


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

    config = new ConfigurationManager("/assets/h265CPU.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    /*Initialize UI Elements values*/
    /*Software Encoder Speed*/
    ui->h265_cpu_encoding_speed_combobox->addItem(this->ENCODER_SPEED_FAST);
    ui->h265_cpu_encoding_speed_combobox->addItem(this->ENCODER_SPEED_FASTER);
    ui->h265_cpu_encoding_speed_combobox->addItem(this->ENCODER_SPEED_SUPERFAST);

    /*Process Priority*/
    ui->h265_cpu_process_priority_combobox->addItem(this->SYS_PRIORITY_ABOVE_NORMAL_LABEL);
    ui->h265_cpu_process_priority_combobox->addItem(this->SYS_PRIORITY_HIGH_LABEL);
    ui->h265_cpu_process_priority_combobox->addItem(this->SYS_PRIORITY_REAL_TIME_LABEL);

    /*Frame threads*/
    ui->h265_cpu_frame_threads_combobox->addItem(this->FRAME_THREADS_2);
    ui->h265_cpu_frame_threads_combobox->addItem(this->FRAME_THREADS_4);
    ui->h265_cpu_frame_threads_combobox->addItem(this->FRAME_THREADS_6);
    ui->h265_cpu_frame_threads_combobox->addItem(this->FRAME_THREADS_8);

    /*Pool threads*/
    ui->h265_cpu_pool_threads_combobox->addItem(this->POOL_THREADS_2);
    ui->h265_cpu_pool_threads_combobox->addItem(this->POOL_THREADS_4);
    ui->h265_cpu_pool_threads_combobox->addItem(this->POOL_THREADS_6);
    ui->h265_cpu_pool_threads_combobox->addItem(this->POOL_THREADS_8);

    /*VBV max rate*/
    ui->h265_cpu_vbv_max_rate_value_combobox->addItem(VBV_MAX_RATE_10_LABEL);
    ui->h265_cpu_vbv_max_rate_value_combobox->addItem(VBV_MAX_RATE_20_LABEL);
    ui->h265_cpu_vbv_max_rate_value_combobox->addItem(VBV_MAX_RATE_30_LABEL);
    ui->h265_cpu_vbv_max_rate_value_combobox->addItem(VBV_MAX_RATE_40_LABEL);

    /*VBV_BUFSIZE*/
    ui->h265_cpu_vbv_bufsize_combobox->addItem(VBV_BUFSIZE_1_LABEL);
    ui->h265_cpu_vbv_bufsize_combobox->addItem(VBV_BUFSIZE_5_LABEL);
    ui->h265_cpu_vbv_bufsize_combobox->addItem(VBV_BUFSIZE_10_LABEL);
    ui->h265_cpu_vbv_bufsize_combobox->addItem(VBV_BUFSIZE_15_LABEL);

    /*CRF*/
    ui->h265_cpu_crf_rate_combobox->addItem(CRF_15);
    ui->h265_cpu_crf_rate_combobox->addItem(CRF_20);
    ui->h265_cpu_crf_rate_combobox->addItem(CRF_25);
    ui->h265_cpu_crf_rate_combobox->addItem(CRF_30);

    /*QP*/
    ui->h265_cpu_qp_rate_combobox->addItem(QP_15);
    ui->h265_cpu_qp_rate_combobox->addItem(QP_20);
    ui->h265_cpu_qp_rate_combobox->addItem(QP_25);
    ui->h265_cpu_qp_rate_combobox->addItem(QP_30);

    /*FEC*/
    ui->h265_cpu_fec_percentage_combobox->addItem(FEC_10);
    ui->h265_cpu_fec_percentage_combobox->addItem(FEC_20);
    ui->h265_cpu_fec_percentage_combobox->addItem(FEC_30);
    ui->h265_cpu_fec_percentage_combobox->addItem(FEC_40);

    setLoadedValues();
}

h265ConfigurationDialog::~h265ConfigurationDialog()
{
    delete ui;
}

void h265ConfigurationDialog::restoreDefaultsValues() {
    config->restoreDefaultConfiguration("/h265CPU.conf");
}

/**
 * @brief h265ConfigurationDialog::setLoadedValues
 * Set the values in the GUI form according to the last state found in
 * the encoder configuration file, and already saved in the In Memory config.
 */
void h265ConfigurationDialog::setLoadedValues() {
    /*Encoder preset*/
    if(config->getKey("sw_preset") == ENCODER_SPEED_FAST) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(0);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FAST);
    }
    else if(config->getKey("sw_preset") == ENCODER_SPEED_FASTER) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(1);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FASTER);
    }
    else if(config->getKey("sw_preset") == ENCODER_SPEED_SUPERFAST) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(2);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_SUPERFAST);
    }

    /*System Priority*/
    /*Compare against the configuration values (1,2,3)*/
    if(config->getKey("system_priority") == SYS_PRIORITY_ABOVE_NORMAL) {
       ui->h265_cpu_process_priority_combobox->setCurrentIndex(0);
       entries_snapshot.insert("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_HIGH) {
        ui->h265_cpu_process_priority_combobox->setCurrentIndex(1);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_REAL_TIME) {
        ui->h265_cpu_process_priority_combobox->setCurrentIndex(2);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_REAL_TIME);
    }

    /*Frame threads*/
    if(config->getKey("min_threads") == FRAME_THREADS_2) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("min_threads", FRAME_THREADS_2);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_4) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("min_threads", FRAME_THREADS_4);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_6) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("min_threads", FRAME_THREADS_6);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_8) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("min_threads", FRAME_THREADS_8);
    }

    /*pool threads*/
    if(config->getKey("pools") == POOL_THREADS_2) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("pools", POOL_THREADS_2);
    }
    else if(config->getKey("pools") == POOL_THREADS_4) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("pools", POOL_THREADS_4);
    }
    else if(config->getKey("pools") == POOL_THREADS_6) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("pools", POOL_THREADS_6);
    }
    else if(config->getKey("pools") == POOL_THREADS_8) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("pools", POOL_THREADS_8);
    }

    /*CRF VS VBV VS QP*/
    qDebug() << "CRF" << config->getKey("on_crf") << Qt::endl;
    if(config->getKey("on_crf") == "1") {
        setCRForVBVorQP("crf");
        /*CRF*/
        if(config->getKey("crf") == CRF_15) {
            ui->h265_cpu_crf_rate_combobox->setCurrentIndex(0);
            entries_snapshot.insert("crf", CRF_15);
        }
        else if(config->getKey("crf") == CRF_20) {
            ui->h265_cpu_crf_rate_combobox->setCurrentIndex(1);
            entries_snapshot.insert("crf", CRF_20);
        }
        else if(config->getKey("crf") == CRF_25) {
            ui->h265_cpu_crf_rate_combobox->setCurrentIndex(2);
            entries_snapshot.insert("crf", CRF_25);
        }
        else if(config->getKey("crf") == CRF_30) {
            ui->h265_cpu_crf_rate_combobox->setCurrentIndex(3);
            entries_snapshot.insert("crf", CRF_30);
        }
        entries_snapshot.insert("vbv_bufsize", "0");
        entries_snapshot.insert("qp", "0");
    }
    else if(config->getKey("on_vbv") == "1") {
        setCRForVBVorQP("vbv");
        /*VBV Bufsize*/
        if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_1) {
            ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(0);
            entries_snapshot.insert("vbv_maxrate", VBV_BUFSIZE_1);
        }
        else if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_5) {
            ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(1);
            entries_snapshot.insert("vbv_bufsize", VBV_BUFSIZE_5);
        }
        else if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_10) {
            ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(2);
            entries_snapshot.insert("vbv_maxrate", VBV_BUFSIZE_10);

        }
        else if(config->getKey("vbv_bufsize") ==  VBV_BUFSIZE_15) {
            ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(3);
            entries_snapshot.insert("vbv_maxrate",  VBV_BUFSIZE_15);
        }
        entries_snapshot.insert("crf", "0");
        entries_snapshot.insert("qp", "0");
    }
    else if(config->getKey("on_qp") == "1") {
        setCRForVBVorQP("qp");
        /*QP*/
        if(config->getKey("qp") == QP_15) {
            ui->h265_cpu_qp_rate_combobox->setCurrentIndex(0);
            entries_snapshot.insert("qp", QP_15);
        }
        else if(config->getKey("qp") == QP_20) {
            ui->h265_cpu_qp_rate_combobox->setCurrentIndex(1);
            entries_snapshot.insert("qp", QP_20);
        }
        else if(config->getKey("qp") == QP_25) {
            ui->h265_cpu_qp_rate_combobox->setCurrentIndex(2);
            entries_snapshot.insert("qp", QP_25);
        }
        else if(config->getKey("qp") == QP_30) {
            ui->h265_cpu_qp_rate_combobox->setCurrentIndex(3);
            entries_snapshot.insert("qp", QP_30);
        }
        entries_snapshot.insert("crf", "0");
        entries_snapshot.insert("vbv_bufsize", "0");
    }
    entries_snapshot.insert("on_crf", config->getKey("on_crf"));
    entries_snapshot.insert("on_vbv", config->getKey("on_vbv"));
    entries_snapshot.insert("on_qp", config->getKey("on_qp"));

    /*vbv-maxrate*/
    if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_10) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(0);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_10);
    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_20) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(1);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_20);
    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_30) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(2);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_30);

    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_40) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(3);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_40);
    }

    /*FEC*/
    if(config->getKey("fec_percentage") == FEC_10) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(0);
        entries_snapshot.insert("fec_percentage", FEC_10);
    }
    else if(config->getKey("fec_percentage") == FEC_20) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(1);
        entries_snapshot.insert("fec_percentage", FEC_20);
    }
    else if(config->getKey("fec_percentage") == FEC_30) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(2);
        entries_snapshot.insert("fec_percentage", FEC_30);
    }
    else if(config->getKey("fec_percentage") == FEC_40) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(3);
        entries_snapshot.insert("fec_percentage", FEC_40);
    }
}

/**
 * @brief h265ConfigurationDialog::setCRForVBVorQP
 * called to set the initial value of the QRadioButtons in the form.
 * This function is only used by setLoadedValues() for the initial state.
 * @param selected
 */
void h265ConfigurationDialog::setCRForVBVorQP(QString selected){
    if(selected == QString("crf")) {
        ui->h265_cpu_crf_on_radio_button->setChecked(true);
        ui->h265_cpu_vbv_max_rate_off_radio_button->setChecked(true);
        ui->h265_cpu_qp_off_radio_button->setChecked(true);
    }
    else if(selected == "qp") {
        ui->h265_cpu_crf_off_radio_button->setChecked(true);
        ui->h265_cpu_vbv_max_rate_off_radio_button->setChecked(true);
        ui->h265_cpu_qp_on_radio_button->setChecked(true);
    }
    else if(selected == "vbv") {
        ui->h265_cpu_crf_off_radio_button->setChecked(true);
        ui->h265_cpu_vbv_max_rate_on_radio_button->setChecked(true);
        ui->h265_cpu_qp_off_radio_button->setChecked(true);
    }
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_vbv_max_rate_on_radio_button_clicked
 *
 *  When VBV is ON, CRF and QP should be disabled.
 */
void h265ConfigurationDialog::on_h265_cpu_vbv_max_rate_on_radio_button_clicked()
{
    ui->h265_cpu_qp_off_radio_button->setChecked(true);
    ui->h265_cpu_crf_off_radio_button->setChecked(true);
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_crf_on_radio_button_clicked
 *
 * When CRF is ON, VBV and QP should be disabled.
 */
void h265ConfigurationDialog::on_h265_cpu_crf_on_radio_button_clicked()
{
    ui->h265_cpu_qp_off_radio_button->setChecked(true);
    ui->h265_cpu_vbv_max_rate_off_radio_button->setChecked(true);
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_qp_on_radio_button_clicked
 *
 * When QP is ON, VBV and CRF should be disabled.
 */
void h265ConfigurationDialog::on_h265_cpu_qp_on_radio_button_clicked()
{
    ui->h265_cpu_vbv_max_rate_off_radio_button->setChecked(true);
    ui->h265_cpu_crf_off_radio_button->setChecked(true);
}

void h265ConfigurationDialog::on_h265_cpu_cancel_button_clicked()
{
    this->hide();
    this->setLoadedValues();
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_restore_button_clicked
 * restore default values from resources file. It copies the resources
 * file to the assets folder.
 */
void h265ConfigurationDialog::on_h265_cpu_restore_button_clicked()
{
    restoreDefaultsValues();
    config->reloadInMemoryValues();
    setLoadedValues();
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_ok_button_clicked
 *
 * - Go through all the values collected values from the GUI
 * - If some is diferent to the in-memory snapshot values, rewrites the configuration file.
 * - Invokes the event to auto-restart the host if app is running.
 */
void h265ConfigurationDialog::on_h265_cpu_ok_button_clicked()
{
    /*Encoder Speed*/
    QString selected_encoder_speed_value = ui->h265_cpu_encoding_speed_combobox->currentText();
    config->setEntry("sw_preset", selected_encoder_speed_value);
    /*System priority*/
    QString selected_system_priority_label = ui->h265_cpu_process_priority_combobox->currentText();
    if(selected_system_priority_label == SYS_PRIORITY_ABOVE_NORMAL_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if (selected_system_priority_label == SYS_PRIORITY_HIGH_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(selected_system_priority_label == SYS_PRIORITY_REAL_TIME_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_REAL_TIME);
    }
    /*Frame threads*/
    QString selected_frame_threads = ui->h265_cpu_frame_threads_combobox->currentText();
    config->setEntry("min_threads", selected_frame_threads);
    /*Pool threads*/
    QString selected_pool_threads = ui->h265_cpu_pool_threads_combobox->currentText();
    config->setEntry("pools", selected_pool_threads);

    /*VBV maxrate value*/
    QString selected_vbv_maxrate_label = ui->h265_cpu_vbv_max_rate_value_combobox->currentText();
    if(selected_vbv_maxrate_label == VBV_MAX_RATE_10_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_10);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_20_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_20);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_30_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_30);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_40_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_40);
    }
    /*VBV bufsize*/
    QString selected_vbv_bufsize_label = ui->h265_cpu_vbv_bufsize_combobox->currentText();
    if(selected_vbv_bufsize_label == VBV_BUFSIZE_1_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_1);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_5_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_5);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_10) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_10);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_15_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_15);
    }
    /*crf rate*/
    QString selected_crf = ui->h265_cpu_crf_rate_combobox->currentText();
    config->setEntry("crf", selected_crf);
    /*qp*/
    QString selected_qp = ui->h265_cpu_qp_rate_combobox->currentText();
    config->setEntry("qp", selected_qp);
    /*fec percentage*/
    QString selected_fec = ui->h265_cpu_fec_percentage_combobox->currentText();
    config->setEntry("fec_percentage", selected_fec);

    if(ui->h265_cpu_crf_on_radio_button->isChecked()) {
         config->setEntry("on_crf", "1");
         config->setEntry("on_vbv", "0");
         config->setEntry("on_qp", "0");
         config->setEntry("vbv_bufsize", "0");
         config->setEntry("qp", "0");
    }
    else if(ui->h265_cpu_vbv_max_rate_on_radio_button->isChecked()) {
        config->setEntry("on_crf", "0");
        config->setEntry("on_vbv", "1");
        config->setEntry("on_qp", "0");
        config->setEntry("qp", "0");
        config->setEntry("crf", "0");
    }
    else if(ui->h265_cpu_qp_on_radio_button->isChecked()) {
        config->setEntry("on_crf", "0");
        config->setEntry("on_vbv", "0");
        config->setEntry("on_qp", "1");
        config->setEntry("crf", "0");
        config->setEntry("vbv_bufsize", "0");
    } else {
        config->setEntry("on_crf", "0");
        config->setEntry("on_vbv", "0");
        config->setEntry("on_qp", "0");
        config->setEntry("crf", "0");
        config->setEntry("vbv_bufsize", "0");
        config->setEntry("qp", "0");
    }

    config->saveConfiguration();
    emit configuration_changed();
    this->hide();
    return;
}
