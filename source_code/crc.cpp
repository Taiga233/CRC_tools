
#include "crc.h"
#include "ui_crc.h"


crc::crc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::crc)
{
    ui->setupUi(this);
    QSizePolicy size_policy = ui->label_tips->sizePolicy();
    //set label tips retain position when hidden.
    size_policy.setRetainSizeWhenHidden(true);
    ui->label_tips->hide();
}

crc::~crc()
{
    delete ui;
}



void crc::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        //自定义
        ui->spinBox_width->setValue(1);
        width_crc = (uint)ui->spinBox_width->value();
        ui->lineEdit_poly->clear();
        poly_binary.clear();
        ui->lineEdit_init->clear();
        init_binary.clear();
        ui->lineEdit_xorout->clear();
        xorout_binary.clear();
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        refin_flag = ui->checkBox_refin->isChecked();
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        refout_flag = ui->checkBox_refout->isChecked();
        break;
    case 1:
        ui->spinBox_width->setValue(4);
        ui->lineEdit_poly->setText(QString("03"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 2:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("09"));
        ui->lineEdit_init->setText(QString("09"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 3:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("15"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 4:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("05"));
        ui->lineEdit_init->setText(QString("1F"));
        ui->lineEdit_xorout->setText(QString("1F"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 5:
        ui->spinBox_width->setValue(6);
        ui->lineEdit_poly->setText(QString("03"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 6:
        ui->spinBox_width->setValue(7);
        ui->lineEdit_poly->setText(QString("09"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 7:
        ui->spinBox_width->setValue(8);
        ui->lineEdit_poly->setText(QString("07"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 8:
        ui->spinBox_width->setValue(8);
        ui->lineEdit_poly->setText(QString("07"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("55"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 9:
        ui->spinBox_width->setValue(8);
        ui->lineEdit_poly->setText(QString("07"));
        ui->lineEdit_init->setText(QString("FF"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 10:
        ui->spinBox_width->setValue(8);
        ui->lineEdit_poly->setText(QString("31"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 11:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("8005"));
        ui->lineEdit_init->setText(QString("0000"));
        ui->lineEdit_xorout->setText(QString("0000"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 12:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("8005"));
        ui->lineEdit_init->setText(QString("0000"));
        ui->lineEdit_xorout->setText(QString("FFFF"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 13:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("8005"));
        ui->lineEdit_init->setText(QString("FFFF"));
        ui->lineEdit_xorout->setText(QString("FFFF"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 14:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("8005"));
        ui->lineEdit_init->setText(QString("FFFF"));
        ui->lineEdit_xorout->setText(QString("0000"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 15:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("1021"));
        ui->lineEdit_init->setText(QString("FFFF"));
        ui->lineEdit_xorout->setText(QString("0000"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 16:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("1021"));
        ui->lineEdit_init->setText(QString("FFFF"));
        ui->lineEdit_xorout->setText(QString("0000"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 17:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("1021"));
        ui->lineEdit_init->setText(QString("FFFF"));
        ui->lineEdit_xorout->setText(QString("FFFF"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 18:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("1021"));
        ui->lineEdit_init->setText(QString("0000"));
        ui->lineEdit_xorout->setText(QString("0000"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 19:
        ui->spinBox_width->setValue(16);
        ui->lineEdit_poly->setText(QString("3D65"));
        ui->lineEdit_init->setText(QString("0000"));
        ui->lineEdit_xorout->setText(QString("FFFF"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 20:
        ui->spinBox_width->setValue(32);
        ui->lineEdit_poly->setText(QString("04C11DB7"));
        ui->lineEdit_init->setText(QString("FFFFFFFF"));
        ui->lineEdit_xorout->setText(QString("FFFFFFFF"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 21:
        ui->spinBox_width->setValue(32);
        ui->lineEdit_poly->setText(QString("04C11DB7"));
        ui->lineEdit_init->setText(QString("FFFFFFFF"));
        ui->lineEdit_xorout->setText(QString("00000000"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    default:
        break;
    }

    if (__builtin_expect(!!(index), 1)) {
        ui->spinBox_width->setCursor(Qt::ForbiddenCursor);
        ui->spinBox_width->setReadOnly(true);
        ui->spinBox_width->setEnabled(false);
        ui->lineEdit_poly->setCursor(Qt::ForbiddenCursor);
        ui->lineEdit_poly->setReadOnly(true);
        ui->lineEdit_poly->setEnabled(false);
        ui->lineEdit_init->setCursor(Qt::ForbiddenCursor);
        ui->lineEdit_init->setReadOnly(true);
        ui->lineEdit_init->setEnabled(false);
        ui->lineEdit_xorout->setCursor(Qt::ForbiddenCursor);
        ui->lineEdit_xorout->setReadOnly(true);
        ui->lineEdit_xorout->setEnabled(false);

        ui->checkBox_refin->setCursor(Qt::ForbiddenCursor);
        ui->checkBox_refin->setEnabled(false);
        ui->checkBox_refout->setCursor(Qt::ForbiddenCursor);
        ui->checkBox_refout->setEnabled(false);
    } else {
        ui->spinBox_width->setReadOnly(false);
        ui->spinBox_width->setEnabled(true);
        ui->spinBox_width->setCursor(Qt::IBeamCursor);
        ui->lineEdit_poly->setReadOnly(false);
        ui->lineEdit_poly->setEnabled(true);
        ui->lineEdit_poly->setCursor(Qt::IBeamCursor);
        ui->lineEdit_init->setReadOnly(false);
        ui->lineEdit_init->setEnabled(true);
        ui->lineEdit_init->setCursor(Qt::IBeamCursor);
        ui->lineEdit_xorout->setReadOnly(false);
        ui->lineEdit_xorout->setEnabled(true);
        ui->lineEdit_xorout->setCursor(Qt::IBeamCursor);

        ui->checkBox_refin->setEnabled(true);
        ui->checkBox_refin->setCursor(Qt::ArrowCursor);
        ui->checkBox_refout->setEnabled(true);
        ui->checkBox_refout->setCursor(Qt::ArrowCursor);
    }
}

