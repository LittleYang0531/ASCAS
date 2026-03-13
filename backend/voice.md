# 语音识别模块

## 目标

根据用户语音内容，自动填入表中某些参数，例如用户输入语音 `高度 120 厘米，有分支`，该模块需要输出 `{"高度": 120，"分支": true}`。

## 输入

输入由两部分组成，一部分为表结构，另一部分为用户语音。

表结构由数组构成，以 JSON 格式给出，数组内元素定义如下：

```cpp
enum class RecordPropertyType {
    NUMBER, // 输入数字
    STRING, // 输入字符串
    SELECT, // 单选
    MULTI, // 多选
    GEOMETRY, // GPS坐标
    IMAGE // 图像
};

class RecordPropertyBase {
    public:

    std::string name = ""; // 属性编号，由系统自动生成
    std::string title = ""; // 属性名称，例如 `长度`
    std::string unit = ""; // 属性单位，例如 `厘米/cm`
    bool required = true; // 是否为必填项
    std::string def = ""; // 默认值
    std::vector<std::string> options = {}; // 单选/多选的选项，其他类型均为空
    RecordPropertyType type = RecordPropertyType::NUMBER; // 属性类型
};
```

例如：

```cpp
[
    {
        "name": "0000000000000000",
        "title": "高度",
        "unit": "厘米/cm",
        "required": true,
        "def": "0",
        "options": [],
        "type": "RecordPropertyType::NUMBER"
    }, {
        "name": "0000000000111111",
        "title": "穗长",
        "unit": "毫米/mm",
        "required": true,
        "def": "0",
        "options": [],
        "type": "RecordPropertyType::NUMBER"
    }
]
```

## 输出

输出同样需要为 JSON 格式，键名为属性编号，键值为模型输出。

例如：

```json
{
    "0000000000000000": "120.00",
    "0000000000111111": "5.6"
}
```