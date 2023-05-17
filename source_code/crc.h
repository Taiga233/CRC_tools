
#ifndef CRC_H
#define CRC_H

#include <QWidget>
#include <QBitArray>



QT_BEGIN_NAMESPACE
namespace Ui { class crc; }
QT_END_NAMESPACE

class crc : public QWidget

{
    Q_OBJECT

public:
    crc(QWidget *parent = NULL);
    ~crc();

    bool empty_check();
    bool specification_check();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clear_clicked();

    void on_pushButton_calculate_clicked();

private:
    Ui::crc *ui;

    QBitArray data; //需要校验的数据二进制表示
    QBitArray crc_result; //校验计算结果二进制表示

    uint width_crc; //位宽
    QBitArray poly_binary; //生成多项式二进制表示，如 100000011 为0x103
    QBitArray init_binary; //初始值的二进制表示，如 00000000 为0x00
    QBitArray xorout_binary; //结果异或值的二进制表示
    bool refin_flag; //输入数据反转
    bool refout_flag; //输出数据反转
};

#endif // CRC_H
