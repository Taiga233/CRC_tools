
#include "crc.h"
#include "ui_crc.h"
#define LEARNING_XML 0 ////learning XML class
#define GENERATE_FORMULA 0 ////generate formula from binary polynomial by xml file
//#define DEBUG_INFO ////日志宏
#define MODIFY_VALUE_OF_NODE 1 ////修改xml中节点的值
#define WRITE_WITH_TRUNCATE 1 ////用Truncate方式写入xml文件中

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
    ui->comboBox->setCurrentIndex(-1);
    ui->pushButton_file->hide();

    input_file = new std::vector<bool>;
    Math_ML = new QtMmlWidget;
    Math_ML->setAttribute(Qt::WA_QuitOnClose, false); //Makes Qt quit the application when the last widget with the attribute set has accepted closeEvent().
#if LEARNING_XML
    initialize_xml(); //learning XML class.
#endif
#if GENERATE_FORMULA
    generate_formula(); //generate formula from binary polynomial by xml file.
#endif
#ifdef USING_XML_RECORD_TEXT
    current_id = -1;
    file_ready = false;
//    ui->pushButton_file->show(); //暂时还没写
    all_data.clear();
    init_xml(); //读取xml文件里面的值用于comboBox的显示
#endif
}

crc::~crc()
{
    delete input_file;
    input_file = Q_NULLPTR;
    Math_ML->close();
    delete Math_ML;
    delete ui;
}

//学习一下对XML的操作
void crc::initialize_xml()
{
    QFile file("./crc_parameter.xml");
    if (!file.open(QFileDevice::ReadWrite | QFileDevice::Truncate)) {
        QMessageBox::information(Q_NULLPTR, "Warning", "XML file failed to open/create!");
        return ;
    }
    QDomDocument xml_document; //xml的主操作类
    //添加xml第一行的说明
    QDomProcessingInstruction instruction;
    instruction = xml_document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    xml_document.appendChild(instruction);
    //根节点
    QDomElement root = xml_document.createElement(QString("Arguments"));
    root.setAttribute("total", 0);
    root.setAttribute("Description", "crc arguments of detail");
    xml_document.appendChild(root);

    QDomElement child = xml_document.createElement(QString("Parametric_model"));
    child.setAttribute("name", "CRC-4-ITU");
    child.setAttribute("ID", 1);
    root.appendChild(child); //可以先添加子节点再添加子节点的节点

    //child的孩子节点，用于记录CRC参数的详细信息
    {
        QDomElement details = xml_document.createElement(QString("Details"));
        details.setAttribute("width", 4);
        details.setAttribute("polynomial_hex_abbreviation", "03");
        details.setAttribute("initial_XOR_value_hex", "00");
        details.setAttribute("result_XOR_value_hex", "00");
        details.setAttribute("input_reverse_flag", true);
        details.setAttribute("output_reverse_flag", true);
        child.appendChild(details);
    }

    //child的孩子节点，记录其他信息
    {
        QDomElement poly_binary; //多项式（非简写）的二进制表示形式和真正的十六进制形式
        poly_binary = xml_document.createElement(QString("Polynomial_binary"));
        poly_binary.setAttribute("hex_completeness", "0x13");
        poly_binary.setAttribute("hex_abbreviation", "0x03");
        QDomText text;
        text = xml_document.createTextNode(QString("10011"));
        poly_binary.appendChild(text);
        child.appendChild(poly_binary);
    }
    {
        QDomElement init_xor_binary; //初始异或值的二进制表示形式
        init_xor_binary = xml_document.createElement(QString("Initial_XOR_value_binary"));
        init_xor_binary.appendChild(xml_document.createTextNode(QString("0000")));
        child.appendChild(init_xor_binary);
    }
    {
        QDomElement result_xor_binary; //结果异或值的二进制表示形式
        result_xor_binary = xml_document.createElement(QString("Result_XOR_value_binary"));
        result_xor_binary.appendChild(xml_document.createTextNode(QString("0000")));
        child.appendChild(result_xor_binary);
    }
    {
        QDomElement poly_formula; //多项式公式
        poly_formula = xml_document.createElement(QString("Polynomial_formula"));
        QDomText text;
        text = xml_document.createTextNode(QString("x4 + x + 1"));
        poly_formula.appendChild(text);
        child.appendChild(poly_formula);
    }
    {
        QDomElement poly_formula_LaTeX; //多项式表达式的LaTex公式表示形式
        poly_formula_LaTeX = xml_document.createElement(QString("Polynomial_formula_LaTeX"));
        poly_formula_LaTeX.appendChild(xml_document.createTextNode(QString("x^{4} + x + 1")));
        child.appendChild(poly_formula_LaTeX);
        /* 这里和上面添加孩子节点的方式是一样的，
         * 但是不能全部这么用，因为函数appendChild()的返回值不是本身的引用
         * 不能这么用：
         *child.appendChild(xml_document.createElement(QString("Polynomial_formula_LaTeX")).appendChild(xml_document.createTextNode(QString("x^{4} + x + 1"))));
         */
    }
    {
        QDomElement poly_formula_MathML; //多项式表达式的MathML表示形式
        poly_formula_MathML = xml_document.createElement(QString("Polynomial_formula_MathML"));
        poly_formula_MathML.appendChild(xml_document.createTextNode(QString("<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"><msup><mi>x</mi><mrow><mn>4</mn></mrow></msup><mo>+</mo><mi>x</mi><mo>+</mo><mn>1</mn></math>")));
        child.appendChild(poly_formula_MathML);
    }
    {
        QDomElement Uses_description; //该参数模型用在哪些算法或国际标准中
        Uses_description = xml_document.createElement(QString("Uses"));
        Uses_description.appendChild(xml_document.createTextNode(QString("International Telecommunication Union")));
        child.appendChild(Uses_description);
    }

    QTextStream stream(&file);
    xml_document.save(stream, 2); //缩进2格
    file.close();
}

//通过xml文件中的Polynomial_binary节点的值来生成两种形式的多项式公式
void crc::generate_formula()
{
    // 打开文件
    QFile file("./crc_parameter.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(Q_NULLPTR, "Error!", "XML file failed to open.");
        qDebug() << "error code:" << file.error() << "\terror string:" << file.errorString();
        return ;
    }
#ifdef DEBUG_INFO
    qInfo() << "file size:" << file.size();
    qInfo() << "sizeof file:" << sizeof(file);
#endif

    //从IO设备中读取XML文件，如果内容被成功解析返回true，否则返回false
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(Q_NULLPTR, "WARNNING!", "It's not XML file.");
        file.close(); //别忘了关闭文件
        return ;
    }
    file.close();
#ifdef DEBUG_INFO
    qInfo() << "sizeof doc:" << sizeof(doc);
#endif

    //获得根节点
    QDomElement root = doc.documentElement();
#ifdef DEBUG_INFO
    qInfo() << "root name:" << root.nodeName();
    qInfo() << "sizeof root:" << sizeof(root);
#endif

    //获取所有Parametric_model的节点
    QDomNodeList list = root.elementsByTagName(root.firstChildElement().nodeName());
    #ifdef DEBUG_INFO
{
    qInfo() << "root.firstChildElement().nodeName():" << root.firstChildElement().nodeName();
    qInfo() << "list size():" << list.size();
    qInfo() << "list count():" << list.count();
    qInfo() << "sizeof list:" << sizeof(list);
    for (int i = 0; i < list.count(); i++) {
            QDomElement element = list.at(i).toElement().childNodes().at(1).toElement();
            qDebug() << "element name:" << element.nodeName();
            qDebug() << "element value:" << element.nodeValue();
            qDebug() << "element type:" << element.nodeType();
            qInfo() << list.at(i).toElement().namedItem(QString("Polynomial_binary")).firstChild().nodeName();
            qInfo() << list.at(i).toElement().namedItem(QString("Polynomial_binary")).firstChild().nodeValue();;
            qInfo() << list.at(i).toElement().namedItem(QString("Polynomial_binary")).firstChild().nodeType();
        }
        /*
        //c++11：
        for (QDomNode node : list) {
            qInfo() << node.toElement().nodeName();
            qInfo() << node.toElement().nodeValue();
        }
        //尝试使用foreach，发现这样不行：
        foreach (QDomNode node, list) {
            qInfo() << node.toElement().nodeName();
            qInfo() << node.toElement().nodeValue();
        }
        */
}
    #endif

    //获取节点的值
    for (int i = 1; i < list.count(); i++) {
        QDomElement element = list.at(i).toElement();
        QByteArray poly = element.namedItem(QString("Polynomial_binary")).firstChild().nodeValue().toLocal8Bit();
        QString Polynomial_formula, Polynomial_formula_LaTeX; //两个最终的表达式
        for (int j = 0, k = poly.size() - 1; j < poly.size(); j++, k--) {
            if (poly.at(j) == '1') {
                if (k) {
                    Polynomial_formula.append("x");
                    Polynomial_formula_LaTeX.append("x");
                    if (1 ^ k) {
                        Polynomial_formula.append(QString::number(k));
                        Polynomial_formula_LaTeX.append("^{");
                        Polynomial_formula_LaTeX.append(QString::number(k));
                        Polynomial_formula_LaTeX.append("}");
                    }
                    Polynomial_formula_LaTeX.append("+");
                    Polynomial_formula.append(" + ");
                } else {
                    Polynomial_formula.append("1");
                    Polynomial_formula_LaTeX.append("1");
                }
            }
        }
#ifdef DEBUG_INFO
        qInfo() << "i:" << i << "poly:" << poly;
        QString xml_formula = list.at(i).toElement().namedItem(QString("Polynomial_formula")).firstChild().nodeValue();
        QString xml_formula_LaTeX = list.at(i).toElement().namedItem(QString("Polynomial_formula_LaTeX")).firstChild().nodeValue();
        qInfo() << "Polynomial_formula:" << Polynomial_formula;
        qInfo() << "xml formula:" << xml_formula;
        qInfo() << "is equal?" << (xml_formula == Polynomial_formula);

        qInfo() << "Polynomial_formula_LaTeX:" << Polynomial_formula_LaTeX;
        qInfo() << "xml formula LaTeX:" << xml_formula_LaTeX;
        qInfo() << "is equal?" << (xml_formula_LaTeX == Polynomial_formula_LaTeX);
#endif

#if (MODIFY_VALUE_OF_NODE & GENERATE_FORMULA) //以下为修改节点的值
{
        //替换Polynomial_formula节点的值：
        QDomNode node = element.namedItem(QString("Polynomial_formula"));
        QDomNode old_node, new_node; //用于新旧内容字节的的替换，通过这种方式来修改xml尖括号里面的值。
        old_node = node.firstChild();
        #ifdef DEBUG_INFO
        qDebug() << "before change,the value is" << old_node.nodeValue();
        qDebug() << "old value:" << node.toElement().text(); //这两种是一样的，只不过是把QDomNode转为QDomElement，然后再取element的text。
        #endif
        node.firstChild().setNodeValue(Polynomial_formula);
        ////注意！，这里如果节点值为空，例如：<Polynomial_formula></Polynomial_formula>中间是没有值的，这样的话setNodeValue()设置不上去。
        #ifdef DEBUG_INFO
        qDebug() << "after change,the value is" << node.toElement().text();
        #endif
        new_node = node.firstChild();
        #ifdef DEBUG_INFO
        qDebug() << "new value:" << new_node.nodeValue();
        #endif
        node.replaceChild(new_node, old_node);
        //替换Polynomial_formula_LaTeX节点的值：
        node = element.namedItem(QString("Polynomial_formula_LaTeX"));
        old_node = node.firstChild();
        node.firstChild().setNodeValue(Polynomial_formula_LaTeX);
        new_node = node.firstChild();
        node.replaceChild(new_node, old_node);
}
#endif
    }
#if (WRITE_WITH_TRUNCATE & GENERATE_FORMULA) //写入xml文件
{
    #ifdef DEBUG_INFO
    //如果打开qt的资源文件先读后写这里会打开失败
    qInfo() << "file.exists():" << file.exists() << "\tfile.fileName():" << file.fileName() << "\tfile.error():" << file.error() << "\tfile.errorString():" << file.errorString() << "\tfile.isOpen():" << file.isOpen();
    #endif
    //写入xml文件
    if (!file.open(QFileDevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(Q_NULLPTR, "Error!", "XML file failed to open.");
        qDebug() << "error code:" << file.error() << "\terror string:" << file.errorString();
        return ;
    }
    QTextStream stream(&file);
    doc.save(stream, 2);
    file.close();
    file.flush();
}
#endif
}

#ifdef USING_XML_RECORD_TEXT
//使用xml里面记录的值来设置comboBox的显示和lineEdit里面的内容
void crc::init_xml()
{
    QFile file(QString(":/xml_file/resources/crc_parameter.xml"));
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(Q_NULLPTR, "Error!", "XML file failed to open.");
        qDebug() << "error code:" << file.error() << "\terror string:" << file.errorString();
        return ;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(Q_NULLPTR, "WARNNING!", "It's not XML file.");
        file.close(); //别忘了关闭文件
        return ;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName(root.firstChildElement().nodeName());

    //read data from xml file
    all_data.clear();
//    all_data.resize(list.count()); //all_data[0]为自定义数据
    for (int i = 0; i < list.count(); i++) {
        QDomElement element = list.at(i).toElement();
        QDomElement details = element.namedItem(QString("Details")).toElement();
        QString fomula = element.namedItem(QString("Polynomial_formula_LaTeX")).firstChild().nodeValue();
        fomula.remove(QChar('{')).remove(QChar('}'));
        QBitArray poly, init, xorout;
        bool flag = false;
        flag = crc::QStringBin_to_QBitArrayBin(element.namedItem(QString("Polynomial_binary")).firstChild().nodeValue(), poly);
        flag = crc::QStringBin_to_QBitArrayBin(element.namedItem(QString("Initial_XOR_value_binary")).firstChild().nodeValue(), init);
        flag = crc::QStringBin_to_QBitArrayBin(element.namedItem(QString("Result_XOR_value_binary")).firstChild().nodeValue(), xorout);
        Q_ASSERT_X(flag, "QStringBin_to_QBitArrayBin", "convert failed!");
        crc_info info(
                    element.attribute(QString("ID")).toShort(),
                    element.attribute(QString("name")),
                    fomula,
                    details.attribute(QString("width")).toUInt(),
                    poly,
                    details.attribute(QString("polynomial_hex_abbreviation")),
                    init,
                    details.attribute(QString("initial_XOR_value_hex")),
                    xorout,
                    details.attribute(QString("result_XOR_value_hex")),
                    (details.attribute(QString("input_reverse_flag")) == QString("1") ? true : false),
                    (details.attribute(QString("output_reverse_flag")) == QString("1") ? true : false));
        all_data.push_back(info); //自定义容器的append和push_back时，会调用默认的拷贝构造函数
        info.clear();
    }

    qint32 fix_width = 0;
    for (VectorInfo::const_iterator it = all_data.constBegin(); it != all_data.constEnd(); ++it) {
        if (fix_width < it->getParameter_name().length()) {
            fix_width = it->getParameter_name().length();
        }
        #ifdef DEBUG_INFO
        qInfo() << "combobox string:" << str_cbx;
        qInfo() << "ID:" << it->at(0) << " width:" << it->getWidth();
        qInfo() << "parameter name:" << it->getParameter_name();
        qInfo() << "formula:" << it->getFormula();
        qInfo() << "poly_bin:" << it->getPoly();
        qInfo() << "poly_hex:" << it->getPoly_hex();
        qInfo() << "init_bin:" << it->getInit();
        qInfo() << "init_hex:" << it->getInit_hex();
        qInfo() << "xorout_bin:" << it->getXorout();
        qInfo() << "xorout_hex:" << it->getXorout_hex();
        qInfo() << "reverse_in:" << it->getReverse_in() << " reverse_out:" << it->getReverse_out() << endl;
        #endif
    }
    QStringList stringlist;
    for (VectorInfo::const_iterator it = all_data.constBegin(); it != all_data.constEnd(); ++it) {

        QString str_cbbx;
        str_cbbx.append(it->getParameter_name());
        forever {
            if (str_cbbx.length() > fix_width) break;
            str_cbbx.append(QChar(' '));
        }
        if (it != all_data.constBegin()) str_cbbx.append(it->getFormula());
        stringlist.append(str_cbbx);
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(stringlist);
    int max_size = 0;
    for (int i = 0; i < ui->comboBox->count(); i++) {
        if (max_size < ui->comboBox->itemText(i).length()) max_size = ui->comboBox->itemText(i).length();
    }
    int point_value = ui->comboBox->font().pointSize(); //获取字体的磅值
    ui->comboBox->setFixedWidth(1024); //设置ComboBox本身的宽度
    //设置ComboBox的下拉框显示的宽度，适应于最长的一项显示
    ui->comboBox->view()->setFixedWidth(point_value * max_size * 0.75);
}
#endif

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
    //想法是把数据的中间当作一个处理区域，这个区域就是进行异或运算的，然后随着计算的进行向后移动
    //step 1
    //数据后面添0，和宽度一致
    //step 2
    //输入值按字节翻转，因为数据过大，可以考虑边计算边翻转，只要翻转的长度大于“处理区域”（用于计算）的长度就可以了。
    //想法：可以按照宽度的整数倍来翻转字节。
    //注意：翻转到最后，setp1数据后面添的0不需要翻转。
    //step 3
    //和初始值异或，如果全0就跳过。（这里先翻转后再初始异或）
    //step 4
    //找到数据的第一个1，从这里开始和多项式进行异或，然后循环找到数据的第一个1。
    //这里当数据后面补充到处理区域，需要处理是否继续往后面翻转。
    //step 5
    //获得结果后，根据结果翻转flag，是否需要按每一位来翻转。
    //step 6
    //和结果异或值异或，如果全0就跳过。（这里先翻转后再异或）
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
    #ifdef DEBUG_INFO
    qInfo() << "before:" << data_array;
    #endif
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
    #ifdef DEBUG_INFO
    qInfo() << "after:" << data_array;
    #endif
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
    if (checked_input.size() & 1) {
        checked_input.prepend("0");
    }
    QStringHex_to_QBitArrayBin(checked_input, input_binary);

#ifdef USING_XML_RECORD_TEXT
    if (!current_id) { //自定义时
        refin_flag = ui->checkBox_refin->isChecked();
        refout_flag = ui->checkBox_refout->isChecked();
        width_crc = ui->spinBox_width->value();
        if (width_crc >= 8) {
            checked_poly.prepend(QChar('1'));
        }
        QStringHex_to_QBitArrayBin(checked_poly, poly_binary);
        QStringHex_to_QBitArrayBin(checked_init, init_binary);
        QStringHex_to_QBitArrayBin(checked_xorout, xorout_binary);
        return ;
    }
    save_data_from_all_data();
#else
    refin_flag = ui->checkBox_refin->isChecked();
    refout_flag = ui->checkBox_refout->isChecked();
    width_crc = ui->spinBox_width->value();
    if (width_crc >= 8) {
        checked_poly.prepend(QChar('1'));
    } //polynomial will change when width less than 8.
    QStringHex_to_QBitArrayBin(checked_poly, poly_binary);
    QStringHex_to_QBitArrayBin(checked_init, init_binary);
    QStringHex_to_QBitArrayBin(checked_xorout, xorout_binary);
#endif
}

void crc::QStringHex_to_QBitArrayBin(const QString &strings, QBitArray &bit_array)
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

void crc::show_result()
{

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


//也可以用xml文件来记录值
void crc::on_comboBox_currentIndexChanged(int index)
{
#ifdef USING_XML_RECORD_TEXT
    current_id = index;
    if (!~index) return ; //初次启动会使得index为-1
    if (!index) {
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
        return ;
    }
    Q_ASSERT_X(index == (int)all_data.at(index).at(0), "comboBox", "index is not equal to id!");
    ui->spinBox_width->setValue(all_data.at(index).getWidth());
    ui->lineEdit_poly->setText(all_data.at(index).getPoly_hex());
    ui->lineEdit_init->setText(all_data.at(index).getInit_hex());
    ui->lineEdit_xorout->setText(all_data.at(index).getXorout_hex());
    ui->checkBox_refin->setCheckState((all_data.at(index).getReverse_in() ? Qt::Checked : Qt::Unchecked));
    ui->checkBox_refout->setCheckState((all_data.at(index).getReverse_out() ? Qt::Checked : Qt::Unchecked));
#else
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
#endif

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
#ifdef USING_XML_RECORD_TEXT
    file_ready = false;
#endif
}

void crc::on_pushButton_calculate_clicked()
{
#ifdef USING_XML_RECORD_TEXT
    if (file_ready) {
        file_ready = false;
        //文件CRC校验专用：
        if (!current_id) {
            ui->label_tips->setText(QString("文件校验暂不支持自定义参数模型！"));
            ui->label_tips->setHidden(false);
            return ;
        }
        save_data_from_all_data();
        bool crc_flag = crc_algorithm(input_file, width_crc, poly_binary, init_binary, xorout_binary, refin_flag, refout_flag, crc_result);
        if (!crc_flag) {
            ui->label_tips->setText(QString("CRC错误"));
            ui->label_tips->setHidden(false);
            return ;
        }
        //show result;
        show_result();
        return ;
    }
#endif
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
    show_result();
}

void crc::on_pushButton_table_clicked()
{
    QString crc_table(":/xml_file/resources/CRC_parameter_table.xml");
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

void crc::on_pushButton_file_clicked()
{
#ifdef USING_XML_RECORD_TEXT
    file_ready = false;
    ui->textEdit->clear();
    QString file_path = QFileDialog::getOpenFileName(this, QString("Open File"));
    if (file_path.isEmpty()) return ;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(Q_NULLPTR, "Error!", "File open failed.");
        qDebug() << "error code:" << file.error() << "\terror string:" << file.errorString();
        return ;
    }
    if (file.size() > (1024 * 1024 * 100)) {
        QMessageBox::information(Q_NULLPTR, "Warning!", "File size great than 100MB!");
        //暂时只计算100MB以下的文件
        file.close();
        return ;
    }

    if (file.size() > (3 * 1024 * 1024)) {
        ui->textEdit->append(QString("<html><head/><body style=\" font-family:'Consolas'; font-size:30pt; color:red;\"><p>Please waiting...</p></body></html>"));
    }
    QCoreApplication::processEvents();
    input_file->clear(); //clear the vector
    QByteArray file_byte(file.readAll());
    quint64 file_size = file_byte.size();
    quint64 i_pos = 0;
    char temp_char = 0;
    QTime time;
    time.start();
    do {
        temp_char = file_byte.at(i_pos);
        for (qint8 j = 7; ~j; --j) {
            input_file->push_back(((temp_char >> j) & 0x01));
        }
        ++i_pos;
    } while (--file_size);
    #ifdef DEBUG_INFO
    qInfo() << "spending time:[" << time.elapsed() << "]ms";
    qInfo() << "file path:" << file_path;
    qInfo() << "file size:" << file.size();
    qInfo() << "file_byte.size()" << file_byte.size();
    qInfo() << "vector size:" << input_file->size();
    #endif
    if (file.size() > (3 * 1024 * 1024)) {
        ui->textEdit->append(QString("spending time:[%1]ms").arg(time.elapsed()));
    }
    ui->textEdit->append(QString("Read file completely."));
    ui->textEdit->append(QString("if you don't want to calculate, click \"clear input data\" button."));
    file_ready = true;
#endif
}
