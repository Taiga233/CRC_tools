
#ifndef CRC_H
#define CRC_H

#include <QBitArray>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
//#include <QDomDocument> //别的头文件里面有了

#include <vector>

#include "qtmmlwidget.h"

#define USING_XML_RECORD_TEXT ////using xml file set edit text

QT_BEGIN_NAMESPACE
namespace Ui { class crc; }
QT_END_NAMESPACE

#ifdef USING_XML_RECORD_TEXT
#pragma pack(4) //因为是32位编译的，所以qt类为4字节
typedef struct CRC_INFO {
private:
    QString parameter_name;
    QString formula;
    quint32 width;
    QBitArray poly;
    QString poly_hex;
    QBitArray init;
    QString init_hex;
    QBitArray xorout;
    QString xorout_hex;
    qint16 id;
    bool reverse_in;
    bool reverse_out;

public:
    //结构体的初始化
//    CRC_INFO() = default; //warning: defaulted and deleted functions only available with -std=c++11 or -std=gnu++11
    CRC_INFO() {} //默认的构造函数
    explicit CRC_INFO(const qint16 &id, const QString &parameter_name, const QString &formula, const quint32 &width, const QBitArray &poly, const QString &poly_hex, const QBitArray &init, const QString &init_hex, const QBitArray &xorout, const QString &xorout_hex, const bool &reverse_in, const bool &reverse_out) {
        this->id = id;
        this->parameter_name = parameter_name;
        this->formula = formula;
        this->width = width;
        this->poly = poly;
        this->poly_hex = poly_hex;
        this->init = init;
        this->init_hex = init_hex;
        this->xorout = xorout;
        this->xorout_hex = xorout_hex;
        this->reverse_in = reverse_in;
        this->reverse_out = reverse_out;
    }

//    qint16 & operator [](const int i) { Q_ASSERT(i >= 0); if (!i) return this->id;}
    const qint16 & at(int i) const { Q_ASSERT(i == 0); return this->id;}

    void clear() {
        id = -1;
        parameter_name.clear();
        formula.clear();
        width = 0;
        poly.clear();
        poly_hex.clear();
        init.clear();
        init_hex.clear();
        xorout.clear();
        xorout_hex.clear();
        reverse_in = false;
        reverse_out = false;
    }
    const QString getParameter_name() const {return parameter_name;}
    QString getFormula() const {return formula;}
    quint32 getWidth() const {return width;}
    QBitArray getPoly() const {return poly;}
    QString getPoly_hex() const {return poly_hex;}
    QBitArray getInit() const {return init;}
    QString getInit_hex() const {return init_hex;}
    QBitArray getXorout() const {return xorout;}
    QString getXorout_hex() const {return xorout_hex;}
    bool getReverse_in() const {return reverse_in;}
    bool getReverse_out() const {return reverse_out;}
} crc_info;
#pragma pack()
#endif

class crc : public QWidget

{
    Q_OBJECT

public:
    crc(QWidget *parent = NULL);
    ~crc();
    void initialize_xml();
    void generate_formula();
#ifdef USING_XML_RECORD_TEXT
    void init_xml();
#endif

    static bool crc_algorithm(const QBitArray* const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result);
    static bool crc_algorithm(const std::vector<bool>* const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result);
    static bool reverse_all(QBitArray &data_array, int data_size = -1);
    static bool reverse_byte(QBitArray &data_array, int data_size = -1);
    static void simplify(QBitArray &bit_array); //remove QBitArray front zero
    static bool QStringBin_to_QBitArrayBin(const QString &strings, QBitArray &bit) {
        if (strings.isEmpty()) return false;
        bit.resize(strings.size());
        for (int i = 0; i < strings.size(); ++i) {
            if (strings.at(i) != '1' && strings.at(i) != '0') return false;
            bit.setBit(i, (strings.at(i) == '1'));
        }
        return true;
    }

    bool empty_check();
    bool specification_check();
    void save_all_data();
    void QStringHex_to_QBitArrayBin(const QString &strings, QBitArray &bit_array);
    void show_result();

    QtMmlWidget *Math_ML;

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clear_clicked();

    void on_pushButton_calculate_clicked();

    void on_pushButton_table_clicked();

    void on_pushButton_file_clicked();

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

#ifdef USING_XML_RECORD_TEXT
    typedef QVector<crc_info> VectorInfo;
    VectorInfo all_data;
    qint16 current_id;
    void save_data_from_all_data()
    {
        refin_flag = all_data.at(current_id).getReverse_in();
        refout_flag = all_data.at(current_id).getReverse_out();
        width_crc = all_data.at(current_id).getWidth();
        poly_binary = all_data.at(current_id).getPoly();
        init_binary = all_data.at(current_id).getInit();
        xorout_binary = all_data.at(current_id).getXorout();
    }
    bool file_ready; //校验文件的标志
#endif
};

#endif // CRC_H
