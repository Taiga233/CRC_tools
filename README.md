# CRC_tools

CRC tool(include 81 polynomials and custom).

WIKI:[Cyclic redundancy check - Wikipedia](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)

Easy to use and fast C++ CRC library:[CRC++ (d-bahr.github.io)](https://d-bahr.github.io/CRCpp/index.html)

------

## 修改点


> 2023年6月29日新增：
>
> 1. 新增了共81种参数模型可以选择（xml方式）。
> 2. 添加了用xml文件保存参数模型的详细信息，用宏`USING_XML_RECORD_TEXT`来控制受否启用xml方式读取参数。

## 使用说明

- 点击 `CRC.exe`运行，因为是静态编译所以可直接拷贝exe文件到别的系统使用。
- 输入十六进制数据。
- ![usage_01](./usage_pic/usage_01.png)
- 选择参数模型。
- ![usage_02](./usage_pic/usage_02.png)
- 计算。
- ![usage_03](./usage_pic/usage_03.png)
- 可以查看参数模型表。
- ![usage_04](./usage_pic/usage_04.png)
- 有输入的格式校验和提示信息。
- ![usage_05](./usage_pic/usage_05.png)



