/********************************************************************************
** Form generated from reading UI file 'crc.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CRC_H
#define UI_CRC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_crc
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QPushButton *pushButton_clear;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout;
    QSpinBox *spinBox_width;
    QLabel *label_5;
    QLineEdit *lineEdit_poly;
    QLabel *label_6;
    QLineEdit *lineEdit_init;
    QLabel *label_7;
    QLineEdit *lineEdit_xorout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_table;
    QLabel *label_4;
    QVBoxLayout *verticalLayout;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_refin;
    QCheckBox *checkBox_refout;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_calculate;
    QLabel *label_tips;
    QFormLayout *formLayout_2;
    QLabel *label_13;
    QLineEdit *lineEdit_result_hex;
    QLabel *label_14;
    QLineEdit *lineEdit_result_bin;

    void setupUi(QWidget *crc)
    {
        if (crc->objectName().isEmpty())
            crc->setObjectName(QStringLiteral("crc"));
        crc->resize(1274, 719);
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(16);
        crc->setFont(font);
        verticalLayout_2 = new QVBoxLayout(crc);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(crc);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_2 = new QLabel(crc);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(16);
        label_2->setFont(font2);

        horizontalLayout_5->addWidget(label_2);

        pushButton_clear = new QPushButton(crc);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setWeight(75);
        pushButton_clear->setFont(font3);

        horizontalLayout_5->addWidget(pushButton_clear);

        horizontalLayout_5->setStretch(0, 3);
        horizontalLayout_5->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_5);

        textEdit = new QTextEdit(crc);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(textEdit);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(crc);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        QFont font4;
        font4.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font4.setPointSize(16);
        font4.setBold(false);
        font4.setWeight(50);
        label_3->setFont(font4);

        horizontalLayout_4->addWidget(label_3);

        comboBox = new QComboBox(crc);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font5.setPointSize(14);
        comboBox->setFont(font5);

        horizontalLayout_4->addWidget(comboBox);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBox_width = new QSpinBox(crc);
        spinBox_width->setObjectName(QStringLiteral("spinBox_width"));
        spinBox_width->setMinimum(1);
        spinBox_width->setMaximum(32);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinBox_width);

        label_5 = new QLabel(crc);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font4);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        lineEdit_poly = new QLineEdit(crc);
        lineEdit_poly->setObjectName(QStringLiteral("lineEdit_poly"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_poly);

        label_6 = new QLabel(crc);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font4);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_6);

        lineEdit_init = new QLineEdit(crc);
        lineEdit_init->setObjectName(QStringLiteral("lineEdit_init"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_init);

        label_7 = new QLabel(crc);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font4);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_7);

        lineEdit_xorout = new QLineEdit(crc);
        lineEdit_xorout->setObjectName(QStringLiteral("lineEdit_xorout"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_xorout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_table = new QPushButton(crc);
        pushButton_table->setObjectName(QStringLiteral("pushButton_table"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        pushButton_table->setFont(font6);

        horizontalLayout->addWidget(pushButton_table);

        label_4 = new QLabel(crc);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font4);

        horizontalLayout->addWidget(label_4);


        formLayout->setLayout(0, QFormLayout::LabelRole, horizontalLayout);


        horizontalLayout_2->addLayout(formLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_8 = new QLabel(crc);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout->addWidget(label_8);

        label_9 = new QLabel(crc);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout->addWidget(label_9);

        label_10 = new QLabel(crc);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout->addWidget(label_10);

        label_11 = new QLabel(crc);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout->addWidget(label_11);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        checkBox_refin = new QCheckBox(crc);
        checkBox_refin->setObjectName(QStringLiteral("checkBox_refin"));
        checkBox_refin->setFont(font2);

        horizontalLayout_3->addWidget(checkBox_refin);

        checkBox_refout = new QCheckBox(crc);
        checkBox_refout->setObjectName(QStringLiteral("checkBox_refout"));
        checkBox_refout->setFont(font2);

        horizontalLayout_3->addWidget(checkBox_refout);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pushButton_calculate = new QPushButton(crc);
        pushButton_calculate->setObjectName(QStringLiteral("pushButton_calculate"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_calculate->sizePolicy().hasHeightForWidth());
        pushButton_calculate->setSizePolicy(sizePolicy2);
        pushButton_calculate->setMinimumSize(QSize(70, 60));
        QFont font7;
        font7.setFamily(QString::fromUtf8("\345\215\216\346\226\207\347\220\245\347\217\200"));
        font7.setPointSize(24);
        pushButton_calculate->setFont(font7);

        horizontalLayout_6->addWidget(pushButton_calculate);

        label_tips = new QLabel(crc);
        label_tips->setObjectName(QStringLiteral("label_tips"));
        QFont font8;
        font8.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font8.setPointSize(16);
        font8.setBold(true);
        font8.setItalic(true);
        font8.setWeight(75);
        label_tips->setFont(font8);
        label_tips->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label_tips);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 3);

        verticalLayout_2->addLayout(horizontalLayout_6);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_13 = new QLabel(crc);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font2);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_13);

        lineEdit_result_hex = new QLineEdit(crc);
        lineEdit_result_hex->setObjectName(QStringLiteral("lineEdit_result_hex"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEdit_result_hex);

        label_14 = new QLabel(crc);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font2);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_14);

        lineEdit_result_bin = new QLineEdit(crc);
        lineEdit_result_bin->setObjectName(QStringLiteral("lineEdit_result_bin"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit_result_bin);


        verticalLayout_2->addLayout(formLayout_2);


        retranslateUi(crc);

        QMetaObject::connectSlotsByName(crc);
    } // setupUi

    void retranslateUi(QWidget *crc)
    {
        crc->setWindowTitle(QApplication::translate("crc", "crc", 0));
        label->setText(QApplication::translate("crc", "CRC\357\274\210\345\276\252\347\216\257\345\206\227\344\275\231\346\240\241\351\252\214\357\274\211\350\256\241\347\256\227\345\267\245\345\205\267", 0));
        label_2->setText(QApplication::translate("crc", "\351\234\200\350\246\201\346\240\241\351\252\214\347\232\204\346\225\260\346\215\256\357\274\210\345\215\201\345\205\255\350\277\233\345\210\266\357\274\214\344\276\213\345\246\202: AF1234 \357\274\211\357\274\232", 0));
        pushButton_clear->setText(QApplication::translate("crc", "\346\270\205\347\251\272\350\276\223\345\205\245", 0));
        textEdit->setDocumentTitle(QString());
        label_3->setText(QApplication::translate("crc", "\345\217\202\346\225\260\346\250\241\345\236\213 NAME:", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("crc", "\350\207\252\345\256\232\344\271\211", 0)
         << QApplication::translate("crc", "CRC-4/ITU           x^4 + x + 1", 0)
         << QApplication::translate("crc", "CRC-5/EPC           x^5 + x^3 + 1", 0)
         << QApplication::translate("crc", "CRC-5/ITU           x^5 + x^4 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-5/USB           x^5 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-6/ITU           x^6 + x + 1", 0)
         << QApplication::translate("crc", "CRC-7/MMC           x^7 + x^3 + 1", 0)
         << QApplication::translate("crc", "CRC-8               x^8 + x^2 + x + 1", 0)
         << QApplication::translate("crc", "CRC-8/ITU           x^8 + x^2 + x + 1", 0)
         << QApplication::translate("crc", "CRC-8/ROHC          x^8 + x^2 + x + 1", 0)
         << QApplication::translate("crc", "CRC-8/MAXIM         x^8 + x^5 + x^4 + 1", 0)
         << QApplication::translate("crc", "CRC-16/IBM          x^16 + x^15 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-16/MAXIM        x^16 + x^15 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-16/USB          x^16 + x^15 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-16/MODBUS       x^16 + x^15 + x^2 + 1", 0)
         << QApplication::translate("crc", "CRC-16/CCITT        x^16 + x^12 + x^5 + 1", 0)
         << QApplication::translate("crc", "CRC-16/CCITT-FALSE  x^16 + x^12 + x^5 + 1", 0)
         << QApplication::translate("crc", "CRC-16/X25          x^16 + x^12 + x^5 + 1", 0)
         << QApplication::translate("crc", "CRC-16/XMODEM       x^16 + x^12 + x^5 + 1", 0)
         << QApplication::translate("crc", "CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1", 0)
         << QApplication::translate("crc", "CRC-32              x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1", 0)
         << QApplication::translate("crc", "CRC-32/MPEG-2       x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1", 0)
        );
        label_5->setText(QApplication::translate("crc", "\345\244\232\351\241\271\345\274\217 POLY(Hex):", 0));
        label_6->setText(QApplication::translate("crc", "\345\210\235\345\247\213\345\200\274 INIT(Hex):", 0));
        label_7->setText(QApplication::translate("crc", "\347\273\223\346\236\234\345\274\202\346\210\226\345\200\274 XOROUT(Hex):", 0));
        pushButton_table->setText(QApplication::translate("crc", "\345\217\202\346\225\260\346\250\241\345\236\213\350\241\250", 0));
        label_4->setText(QApplication::translate("crc", "\345\256\275\345\272\246 WIDTH\357\274\232", 0));
        label_8->setText(QApplication::translate("crc", "\344\276\213\345\246\202\357\274\2328", 0));
        label_9->setText(QApplication::translate("crc", "\344\276\213\345\246\202\357\274\232ABCD", 0));
        label_10->setText(QApplication::translate("crc", "\344\276\213\345\246\202\357\274\232FFFF", 0));
        label_11->setText(QApplication::translate("crc", "\344\276\213\345\246\202\357\274\23200", 0));
        checkBox_refin->setText(QApplication::translate("crc", "\350\276\223\345\205\245\346\225\260\346\215\256\345\217\215\350\275\254\357\274\210REFIN\357\274\211", 0));
        checkBox_refout->setText(QApplication::translate("crc", "\350\276\223\345\207\272\346\225\260\346\215\256\345\217\215\350\275\254\357\274\210REFOUT\357\274\211", 0));
        pushButton_calculate->setText(QApplication::translate("crc", "\350\256\241\347\256\227", 0));
        label_tips->setText(QApplication::translate("crc", "\350\257\267\350\276\223\345\205\245\351\234\200\350\246\201\346\240\241\351\252\214\347\232\204\346\225\260\346\215\256 ", 0));
        label_13->setText(QApplication::translate("crc", "\346\240\241\351\252\214\347\273\223\346\236\234\357\274\210Hex\357\274\211\357\274\232", 0));
        label_14->setText(QApplication::translate("crc", "\346\240\241\351\252\214\347\273\223\346\236\234\357\274\210Bin\357\274\211\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class crc: public Ui_crc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CRC_H
