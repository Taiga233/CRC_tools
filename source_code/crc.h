
#ifndef CRC_H
#define CRC_H

#include <QBitArray>
#include <QDesktopWidget>

#include <vector>

#include "qtmmlwidget.h"



QT_BEGIN_NAMESPACE
namespace Ui { class crc; }
QT_END_NAMESPACE

class crc : public QWidget

{
    Q_OBJECT

public:
    crc(QWidget *parent = NULL);
    ~crc();

    static bool crc_algorithm(const QBitArray* const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result);
    static bool crc_algorithm(const std::vector<bool>* const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result);
    static bool reverse_all(QBitArray &data_array, int data_size = -1);
    static bool reverse_byte(QBitArray &data_array, int data_size = -1);

    bool empty_check();
    bool specification_check();
    void save_all_data();
    void QString_to_QBitArray(const QString &strings, QBitArray &bit_array);

    QtMmlWidget *Math_ML;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clear_clicked();

    void on_pushButton_calculate_clicked();

    void on_pushButton_table_clicked();

private:
    Ui::crc *ui;

    QString checked_input;
    std::vector<bool> *input_file; //大文件的输入数据（二进制）
    QBitArray input_binary; //需要校验的数据二进制表示
    QBitArray crc_result; //校验计算结果二进制表示

    uint width_crc; //位宽
    QString checked_poly;
    QBitArray poly_binary; //生成多项式二进制表示，如 100000011 为0x103
    QString checked_init;
    QBitArray init_binary; //初始值的二进制表示，如 00000000 为0x00
    QString checked_xorout;
    QBitArray xorout_binary; //结果异或值的二进制表示
    bool refin_flag; //输入数据反转
    bool refout_flag; //输出数据反转
};

#endif // CRC_H
