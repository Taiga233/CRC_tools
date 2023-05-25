
#include "crc.h"
#include "ui_crc.h"


crc::crc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::crc)
{
    ui->setupUi(this);
    ui->label_tips->setStyleSheet("color: red");
    QSizePolicy size_policy = ui->label_tips->sizePolicy();
    //set label tips retain position when hidden.
    size_policy.setRetainSizeWhenHidden(true);
    ui->label_tips->setSizePolicy(size_policy);
    ui->label_tips->hide();

    input_file = new std::vector<bool>;
    Math_ML = new QtMmlWidget;
    Math_ML->setAttribute(Qt::WA_QuitOnClose, false); //Makes Qt quit the application when the last widget with the attribute set has accepted closeEvent().
}

crc::~crc()
{
    delete input_file;
    input_file = Q_NULLPTR;
    Math_ML->close();
    delete Math_ML;
    delete ui;
}

bool crc::crc_algorithm(const QBitArray * const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result)
{
    qInfo() << "input:" << (*input);
    bool result_flag = true;
    QBitArray input_modified((*input));
    //If resize is greater than the current size, the bit array is extended to make it resize bits with the extra bits added to the end. The new bits are initialized to false (0).
    input_modified.resize(input->size() + width);

    //according to bytes reverse data by reverse in flag
    (reverse_in && (result_flag = crc::reverse_byte(input_modified, input->size())));
    Q_ASSERT(result_flag);

    //XOR with INIT
    if (init.count(true)) {
        QBitArray init_modified(init);
        crc::simplify(init_modified); //remove redundant 0
//        qDebug() << "init_modified:" << init_modified;
        if ((uint)init_modified.size() > width) {
            qWarning() << "init size longer than crc width!";
        }
        //if init_modified size less than width, add 0 in front.
        if ((uint)init_modified.size() < width) {
            crc::reverse_all(init_modified);
            init_modified.resize(width);
            crc::reverse_all(init_modified);
        }
        if (__builtin_expect((init_modified.size() <= input_modified.size()), 1)) {
            //The result has the length of the longest of the two bit arrays, with any missing bits (if one array is shorter than the other) taken to be 0.
            input_modified ^= init_modified;
        } else {
            qWarning() << "init size longer than input_modified!";
            for (int i = 0; i < input_modified.size(); i++) {
                input_modified.setBit(i, (input_modified.at(i) ^ init_modified.at(i)));
            }
        }
    }

    //start
    QBitArray poly_modified(poly);
    crc::simplify(poly_modified);
    //在这里多项式可以根据位宽在前面添加一个1
    int pos = 0;
    while (pos < input_modified.size() && !input_modified.testBit(pos)) {
        pos++; //find the first 1.
    }
//    qDebug() << "pos:" << pos;
    if (poly_modified.size() > (input_modified.size() - pos) && pos != input_modified.size()) {
        qCritical() << "poly_modified size longer than input data at first XOR!";
        result_flag = false;
        Q_ASSERT(result_flag);
        return result_flag;
    }
    while (pos + poly_modified.size() <= input_modified.size()) {
        for (int i = 0, j = pos; j < input_modified.size() && i < poly_modified.size(); i++, j++) {
            input_modified.setBit(j, (input_modified.at(j) ^ poly_modified.at(i)));
        }
        while (pos < input_modified.size() && !input_modified.testBit(pos)) {
            pos++;
        }
//        qDebug() << "pos:" << pos << "input_modified:" << input_modified;
    }
    result.resize(width); //agree with crc width
    for (int i = width - 1; ~i; i--) {
        result.setBit(i, input_modified.at(input_modified.size() - (width - i)));
    }

    //reverse all result data by reverse out flag
    (reverse_out && (result_flag = crc::reverse_all(result)));
    Q_ASSERT(result_flag);

    //xor with xorout
    if (xorout.count(true)) {
        QBitArray xorout_modified(xorout);
        crc::simplify(xorout_modified);
        //if xorout_modified size less than width, add 0 in front.
        if ((uint)xorout_modified.size() < width) {
            crc::reverse_all(xorout_modified);
            xorout_modified.resize(width);
            crc::reverse_all(xorout_modified);
        }
        if (__builtin_expect((xorout_modified.size() <= result.size()), 1)) {
            result ^= xorout_modified;
        } else {
            qWarning() << "xorout size longer than result(as same as width)!";
            for (int i = 0; i < result.size(); i++) {
                result.setBit(i, (result.at(i) ^ xorout_modified.at(i)));
            }
        }
    }
    qInfo() << "result:" << result;

    return result_flag;
}

//用于大文件的处理，暂时还没写（。＾▽＾）
bool crc::crc_algorithm(const std::vector<bool> * const input, const uint &width, const QBitArray &poly, const QBitArray &init, const QBitArray &xorout, const bool &reverse_in, const bool &reverse_out, QBitArray &result)
{
    Q_UNUSED(input);
    Q_UNUSED(width);
    Q_UNUSED(poly);
    Q_UNUSED(init);
    Q_UNUSED(xorout);
    Q_UNUSED(reverse_in);
    Q_UNUSED(reverse_out);
    Q_UNUSED(result);
    return true;
}

bool crc::reverse_all(QBitArray &data_array, int data_size)
{
    if (!data_size) return false;
    if (!~data_size) {
        data_size = data_array.size();
    }
//    qDebug() << "before:" << data_array;
    data_size--;
    bool _a, _b;
    for (int i = 0; i < data_size; ++i, --data_size) {
        _a = data_array.at(i);
        _b = data_array.at(data_size);
        _a ^= _b ^= _a ^= _b;
        data_array.setBit(i, _a);
        data_array.setBit(data_size, _b);
    }
//    qDebug() << "after:" << data_array;
    return true;
}

//according to bytes to reverse
bool crc::reverse_byte(QBitArray &data_array, int data_size)
{
    if (!data_size) return false;
    if (!~data_size) {
        data_size = data_array.size();
    }
    qInfo() << "before:" << data_array;
    bool _a, _b;
    for (int i = 0; i < (data_size >> 3); i++) {
        for (int j = (i << 3), k = (i << 3) + 8 - 1; j < k; j++, k--) {
            _a = data_array.at(j);
            _b = data_array.at(k);
            _a ^= _b ^= _a ^= _b;
            data_array.setBit(j, _a);
            data_array.setBit(k, _b);
        }
    }
    qInfo() << "after:" << data_array;
    return true;
}

//remove QBitArray zero in front
void crc::simplify(QBitArray &bit_array)
{
    if (!bit_array.size()) return ;
    crc::reverse_all(bit_array);
    for (int i = bit_array.size() - 1; !bit_array.at(i) && i; i--) {
        bit_array.truncate(i); //remove redundant 0
    }
    crc::reverse_all(bit_array);
}

bool crc::empty_check()
{
    if(checked_input.isEmpty()) {
        ui->label_tips->setText(QString("请输入需要校验的数据"));
        return true;
    }
    if(checked_poly.isEmpty()) {
        ui->label_tips->setText(QString("请输入多项式的值（十六进制）"));
        return true;
    }
    if(checked_init.isEmpty()) {
        ui->label_tips->setText(QString("请输入初始值（十六进制）"));
        return true;
    }
    if(checked_xorout.isEmpty()) {
        ui->label_tips->setText(QString("请输入结果异或值（十六进制）"));
        return true;
    }
    return false;
}

bool crc::specification_check()
{
    QRegularExpression r_e("[^A-Fa-f0-9]");
//    qDebug() << "checked_input count: " << checked_input.count();
    checked_input.remove(QRegularExpression("\\s"));
//    qDebug() << "After: " << checked_input.count() << "content: " << checked_input;
//    qDebug() << "正则表达式匹配: " << r_e.match(checked_input).capturedLength();
    if (r_e.match(checked_input).capturedLength()) {
        ui->label_tips->setText(QString("输入数据格式错误"));
        return true;
    }

    checked_poly.remove(QRegularExpression("\\s"));
    if (r_e.match(checked_poly).capturedLength()) {
        ui->label_tips->setText(QString("多项式（简写）格式错误"));
        return true;
    }

    checked_init.remove(QRegularExpression("\\s"));
    if (r_e.match(checked_init).capturedLength()) {
        ui->label_tips->setText(QString("初始值格式错误"));
        return true;
    }

    checked_xorout.remove(QRegularExpression("\\s"));
    if (r_e.match(checked_xorout).capturedLength()) {
        ui->label_tips->setText(QString("结果异或值格式错误"));
        return true;
    }

    return false;
}

void crc::save_all_data()
{
    refin_flag = ui->checkBox_refin->isChecked();
    refout_flag = ui->checkBox_refout->isChecked();
    width_crc = ui->spinBox_width->value();

    if (checked_input.size() & 1) {
        checked_input.prepend("0");
    }
    QString_to_QBitArray(checked_input, input_binary);
    if (width_crc >= 8) {
        checked_poly.prepend(QChar('1'));
    } //polynomial will change when width less than 8.
    QString_to_QBitArray(checked_poly, poly_binary);
    QString_to_QBitArray(checked_init, init_binary);
    QString_to_QBitArray(checked_xorout, xorout_binary);
}

void crc::QString_to_QBitArray(const QString &strings, QBitArray &bit_array)
{
    bit_array.clear();
    bit_array.resize(strings.size() * 4);
    quint8 temp_char = 0;
    char str[1] = {0};
    int bit_array_position = 0;
    QByteArray temp_array = strings.toLocal8Bit();
    for (int i = 0; i < temp_array.size(); ++i) {
        str[0] = temp_array.at(i);
        temp_char = (quint8)QByteArray::fromRawData(str, sizeof(char)).toShort(Q_NULLPTR, 16);
//        temp_char = (quint8)temp_array.at(i); //如果是纯字符串用这个，并且设置大小应该为strings.size() * 8、与值设置为0x80、循环8次、不需要str[]。
//        qDebug() << "i:" << i << "temp_char:" << (quint8)temp_char;
        for (int j = 0; j < 4; j++) {
            bit_array.setBit(bit_array_position++, (temp_char & 0x8));
//            qDebug() << j << "& j:" << (temp_char & 0x8);
            temp_char <<= 1;
        }
    }
//    qDebug() << "strings:" << strings << bit_array;
}


//也可以用xml文件来记录值
void crc::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        //自定义
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
        ui->lineEdit_poly->setText(QString("13"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 2:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("29"));
        ui->lineEdit_init->setText(QString("09"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Unchecked);
        ui->checkBox_refout->setCheckState(Qt::Unchecked);
        break;
    case 3:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("35"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 4:
        ui->spinBox_width->setValue(5);
        ui->lineEdit_poly->setText(QString("25"));
        ui->lineEdit_init->setText(QString("1F"));
        ui->lineEdit_xorout->setText(QString("1F"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 5:
        ui->spinBox_width->setValue(6);
        ui->lineEdit_poly->setText(QString("43"));
        ui->lineEdit_init->setText(QString("00"));
        ui->lineEdit_xorout->setText(QString("00"));
        ui->checkBox_refin->setCheckState(Qt::Checked);
        ui->checkBox_refout->setCheckState(Qt::Checked);
        break;
    case 6:
        ui->spinBox_width->setValue(7);
        ui->lineEdit_poly->setText(QString("89"));
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
        ui->lineEdit_init->setText(QString("0000"));
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

void crc::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
//    ui->lineEdit_result_bin->clear();
//    ui->lineEdit_result_hex->clear();
    if (!ui->comboBox->currentIndex()) {
        ui->lineEdit_init->clear();
        ui->lineEdit_poly->clear();
        ui->lineEdit_xorout->clear();
    }
    ui->label_tips->clear();
}

void crc::on_pushButton_calculate_clicked()
{
    ui->lineEdit_result_hex->clear();
    ui->lineEdit_result_bin->clear();
    checked_input.clear();
    checked_input = ui->textEdit->toPlainText().toUpper();
    checked_poly.clear();
    checked_poly = ui->lineEdit_poly->text().toUpper();
    checked_init.clear();
    checked_init = ui->lineEdit_init->text().toUpper();
    checked_xorout.clear();
    checked_xorout = ui->lineEdit_xorout->text().toUpper();

    //input empty check
    if (empty_check()) {
        ui->label_tips->setHidden(false);
        return ;
    }

    //input specification check
    if (specification_check()) {
        ui->label_tips->setHidden(false);
        return ;
    }

    //input empty check again
    if (empty_check()) {
        ui->label_tips->setHidden(false);
        return ;
    }

    //save all data
    save_all_data();

    //ready
    bool crc_flag;
    crc_flag = crc::crc_algorithm(&input_binary, width_crc, poly_binary, init_binary, xorout_binary, refin_flag, refout_flag, crc_result);
    if (!crc_flag) {
        ui->label_tips->setText(QString("CRC错误"));
        ui->label_tips->setHidden(false);
        return ;
    }

    //show result
    QString binary_result;
    for (int i = 0; i < crc_result.size(); i++) {
        binary_result.append(crc_result.at(i) ? "1" : "0");
    }
    ui->lineEdit_result_bin->setText(binary_result);
    QString hex_result(QString::number(binary_result.toUInt(Q_NULLPTR, 2), 16).toUpper());
    for (int i = hex_result.size(); i < (binary_result.size() >> 2); i++) {
        hex_result.prepend("0");
    }
    hex_result.prepend("0x");
    ui->lineEdit_result_hex->setText(hex_result);
    ui->label_tips->hide();
}

void crc::on_pushButton_table_clicked()
{
    QString crc_table(":/CRC_parameter_table.xml");
    QFile file(crc_table);
    if (!file.open(QIODevice::ReadOnly)) {
        ui->label_tips->setText(QString("Open file error!"));
        ui->label_tips->setHidden(false);
        return ;
    }
    QTextStream stream(&file);
    //stream.setEncoding(QTextStream::UnicodeUTF8);
    QString MathML_content = stream.readAll();
    file.close();

    Math_ML->clear();
    QString error_msg;
    int error_line, error_column;
    bool result;
    result = Math_ML->setContent(MathML_content, &error_msg, &error_line, &error_column);
    if (!result) {
        ui->label_tips->setText(QString("Parse error at line %1 column %2.\nError message:").arg(error_line).arg(error_column) + error_msg);
        ui->label_tips->setHidden(false);
        return ;
    }
    ui->label_tips->hide();
    Math_ML->setBaseFontPointSize(14);
    Math_ML->setWindowTitle(QString("CRC polynomial table"));
    Math_ML->resize(QSize(1850, 830));
    Math_ML->move(QApplication::desktop()->screen()->rect().center() - Math_ML->rect().center());
    Math_ML->show();
}
