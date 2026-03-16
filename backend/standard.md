# 协作者编写指南

**只需要修改以下文件，其他文件勿动！**

- `api/**/*.cpp`：所有路由文件
- `resources/tables.h`：MySQL 数据表定义
- `models/*.cpp`：类定义、枚举定义
- `utils/*.cpp`：杂项函数
- `main.cpp`：主函数文件
- `recompile.json`：重编译标志，没有的话需要自行创建

**每次修改完后，编辑 `recompile.json` 文件即可重新编译，修改内容不限。（也就是能让程序检测到有修改即可，不需要遵循 json 格式）**

**如果想要新建路由、类、枚举、杂项文件等，建议直接复制已经写好了的文件稍加修改即可**

头文件引用：

- `api/**/*.cpp`：请在 `main.cpp` 中直接引用
- `models/*.cpp`, `utils/*.cpp`：请在 `ascas.h` 中直接引用

以下是标准细则：

## 路由创建

可以直接从其他路由文件复制，大致格式如下：

```cpp
#include "./ascas.h" // 换成具体 ascas.h 路径，请使用相对路径

auto Route = [](client_conn conn, http_request request, param argv) { // Route 为路由名称，通常为路由文件路径
    // 具体操作执行

};
```

使用 `quickSend` 系列函数发送数据并结束请求：

- `quickSendCode(code)`：发送 HTTP code 并结束请求。
- `quickSendItem(code, item)`：发送 HTTP code，并在 json 根目录中添加键名为 `item` 的对象，对象内容为参数 `item`。
- `quickSendItems(code, items)`：发送 HTTP code，并在 json 根目录中添加键名为 `items` 的数组，数组内容为参数 `items`，你可能需要使用 `packarr` 函数。
- `quickSendData(code, value1, value2, ...)`：发送 HTTP code，并在 json 根目录中添加键名为变量名，键值为变量值的参数。

几个比较重要的内容：

- `request.method`：请求方法，均为全大写
- `request.argv`：请求头，类型为 `std::map<std::string, std::string>`，`key` 为全小写。例如使用 `request.argv["content-type"]` 获取正文类型。
- `request.postdata`：POST 数据
- `argv`：路径数据，类型为 `std::vector<std::string>`。例如路由路径为 `/users/%s/%d`，真实路径为 `/users/guest/1`，则 `argv` 内容为 `{ "guest", "1" }`。

编写好后回到 `main.cpp` 添加路由：

- 引入你所编写的路由文件
- 使用 `app.addRoute(path, route)` 添加路由，其中 `path` 为 format 字符串。服务器会自动根据真实路径匹配路由。

最后修改 `recompile.json` 重新编译即可。

常用 HTTP code:

- `200`：没有问题
- `400`：GET / POST 参数有误（不能为空等，不需要返回原因）
- `401`：用户未登录或权限不足
- `404`：未找到实体
- `405`：请求方法错误
- `409`：实体冲突

## 类、枚举创建

### 类创建标准

例如你想新建一个 `Crop` 类：

- 创建一个 `CropBase` 类，添加各种类成员。
- 实现 `static CropBase fromJsonObject(Json::Value obj)` 和 `Json::Value toJsonObject()` 函数。
- 在类定义后使用 `initModel(Crop)` 正式创建类 `Crop`。

建议直接从已定义好的类中复制粘贴实现即可。

### 枚举创建标准

例如你想新建一个 `RecordPropertyType` 枚举：

- 创建一个 `RecordPropertyType` 枚举，添加各种枚举成员。
- 在枚举定义后使用 `initEnum(RecordPropertyType, firstMember, lastMember)` 初始化静态映射器。

建议直接从已定义好的枚举中复制粘贴实现即可。

### 可能需要用到的函数

- `getEnumFromName(type, name)`：从名称映射回枚举
- `getNameFromEnum(value)`：从枚举映射到名称
- `extarr<T>(jsonarr, extfunc?)`：将 json 数组解包到一个 `std::vector<T>` 中。`extfunc` 为可选参数，默认使用 `json.as<T>()` 进行解包。
- `packarr(arr, packfunc?)`：将一个 `std::vector<T>` 打包到一个 json 数组中。`packfunc` 为可选参数，默认使用显式类型转换进行打包。

## 数据库操作

**所有对数据库的操作请在杂项文件内编写**

编写格式：

```cpp
Func funcname(...) {
    quick_mysqli_connect();

    // 执行其他操作
    // ...
}
```

可能需要用到的函数：

- `hasIntersection(column, jsonarr)`：生成一个 WHERE 子句，用于判断 `column` 列中是否含有 `jsonarr` 中的一项，`column` 列中的格式也应为 `jsonarr`，且数组成员不能为 `jsonobj`。
- `mysqli_query(mysql, format, ...)`：查询数据库，返回格式为 `std::vector<std::map<std::string, std::string> > `，调用格式为 `res[3]["val"]`，第一维为结果的第几行，第二维为对应列名。一定要记得判断行数是否充足！
- `mysqli_execute(mysql, format, ...)`：操作数据库。
- `MyAssert(expr, format, ...)`：`assert` 函数，但可以输出自定义内容。
- `jsonarr(value1, value2, ...)`：快速生成一个 `jsonarr`
- `jsonobj(key1, value1, key2, value2, ...)`：快速生成一个 `jsonobj`

注意事项：

- 对于部分涉及到 `json` 格式的数据（例如 `crops.properties`），请使用 `json_encode` 将其 encode 后存入数据库中。
- 对于任意 `text` 格式的数据，一定要注意使用 `quote_encode` 防止引号出错。

## 函数速查

### HTTP 服务器相关

- `getParam(request)`：获取 get 参数，类型为 `std::map<std::string, std::string>`
- `postParam(request)`：获取 post 参数，类型为 `std::map<std::string, std::string>`
- `cookieParam(request)`：获取 cookie 参数，类型为 `std::map<std::string, std::string>`
- `quickSendCode(code)`：发送 HTTP code 并结束请求。
- `quickSendItem(code, item)`：发送 HTTP code，并在 json 根目录中添加键名为 `item` 的对象，对象内容为参数 `item`。
- `quickSendItems(code, items)`：发送 HTTP code，并在 json 根目录中添加键名为 `items` 的数组，数组内容为参数 `items`，你可能需要使用 `packarr` 函数。
- `quickSendData(code, value1, value2, ...)`：发送 HTTP code，并在 json 根目录中添加键名为变量名，键值为变量值的参数。

### JSON 相关

- `json_encode(json)`：json 转 `std::string`
- `json_pretty_encode(json)`：json 转格式化 `std::string`
- `json_decode(str)`：`std::string` 转 json
- `json_decode(str, json)`：`std::string` 转 json，并存储到 `json` 变量中，返回值为是否成功。
- `jsonarr(value1, value2, ...)`：快速生成一个 `jsonarr`
- `jsonobj(key1, value1, key2, value2, ...)`：快速生成一个 `jsonobj`
- `extarr<T>(jsonarr, extfunc?)`：将 json 数组解包到一个 `std::vector<T>` 中。`extfunc` 为可选参数，默认使用 `json.as<T>()` 进行解包。
- `packarr(arr, packfunc?)`：将一个 `std::vector<T>` 打包到一个 json 数组中。`packfunc` 为可选参数，默认使用显式类型转换进行打包。

### 日志相关

- `writeLog(logLevel, format, ...)`：以 `logLevel` 输出日志。`logLevel` 详见 `include/log.h`。

### 字符串处理相关

- `readFile(path)`：读取文件并存储在一个 `std::string` 中。
- `explode(seperator, source)`：以 `seperator` 为分隔符，将 `source` 切割成多个字符串。
- `trim(source)`：删除 `source` 两端的空白字符 ` \r\n\t\f\t\v`
- `join(seperator, source)`：使用 `seperator` 将 `source` 连接成一个字符串，也可以看作 `explode` 的逆函数。

# 当前规划

用户头像存储在 `/backend/data/avatars/{uid}.png` 下。

图片数据存储在 `/backend/data/images/{sha1}.jpg` 下，图片名称统一使用其 sha1 码，然后将其 sha1 码作为文本内容存储在数据库中。

注意用户头像使用 png 存储，图片数据使用 jpg 存储

协作者请将已经实现了的接口直接删除，建议按照该文档的顺序实现接口。

## GET `/users/%d`

- 请将 route 文件写在 `api/users/details.cpp`
- 获取用户详细信息
- `%d` 参数为用户 uid。
- 需要判断用户是否登录。

例：

```bash
curl "http://localhost:8080/users/1" \
    -H "Authorization: SessionToken <session>"
```

## GET `/users/%d/avatar`

- 请将 route 文件写在 `api/users/avatar.cpp`
- 获取用户头像，直接返回 image/png 格式即可。
- `%d` 参数为用户 uid。
- 需要判断用户是否登录。

例：

```bash
curl "http://localhost:8080/users/1/avatar" \
    -o avatar.png \
    -H "Authorization: SessionToken <session>"
```

## POST `/users/%d/avatar`

- 请将 route 文件写在 `api/users/avatar.cpp`
- 更新用户头像，postdata 内为图像 base64 码。
- `%d` 参数为用户 uid。
- 需要判断是否为本人调用。

例：

```bash
curl "http://localhost:8080/users/1/avatar" \
    -X POST \
    -H "Authorization: SessionToken <session>" \
    -d "iVBORw0KGgoAAAANSUhEUgAAB4AAAAScCAYA..."
```

## POST `/crops/%d/images`

- 请将 route 文件写在 `api/records/uploadImages.cpp`
- 上传一张新图片，postdata 内为图片 base64 码。
- `%d` 参数实际无用处。
- 需要向前端返回图片 sha1 码。
- 要求用户权限为 `UserPermission::OWNER` 或 `UserPermission::EDITOR`

例：

```bash
curl "http://localhost:8080/crops/1/images" \
    -X POST \
    -H "Authorization: SessionToken <session>" \
    -d "iVBORw0KGgoAAAANSUhEUgAAB4AAAAScCAYA..."
```

## GET `/crops/%d/images/%s`

- 请将 route 文件写在 `api/records/images.cpp`
- 根据图片 sha1 码获取图片，直接返回 **image/jpeg** 格式即可。
- `%s` 即为图片 sha1 码，`%d` 参数实际无用处。
- 要求用户权限为 `UserPermission::OWNER` 或 `UserPermission::EDITOR` 或 `UserPermission::VIEWER`

例：

```bash
curl "http://localhost:8080/crops/1/images/adc83b19e793491b1c6ea0fd8b46cd9f32e592fc" \
    -o images.png \
    -H "Authorization: SessionToken <session>"
```

## POST `/crops/%d/records/add`

- 请将 route 文件写在 `api/records/add.cpp`
- 向作物表中添加一行数据
- 要求用户权限为 `UserPermission::OWNER` 或 `UserPermission::EDITOR`

## POST `/crops/%s/records/list`

- 请将 route 文件写在 `api/records/list.cpp`
- 拉取部分作物表中的数据
- 需要支持 `where`, `order`, `limit`, `offset` 等 query 参数，均在 `request.postdata` 中，json 格式
- 要求用户权限为 `UserPermission::OWNER` 或 `UserPermission::EDITOR` 或 `UserPermission::VIEWER`

### where 参数

```cpp
enum class SQLOperator {
    // 通用筛选
    EQUAL, // 等于
    NOTEQUAL, // 不等于
    // 数值筛选
    GREATER, // 大于
    GREATER_OR_EQUAL, // 大于等于
    SMALLER, // 小于
    SMALLER_OR_EQUAL, // 小于等于
    // 字符串筛选
    LIKE, // like
    NOTLIKE, // notlike
    REGEXP, // regexp
    NOTREGEXP, // !regexp
};
class node {
    public:

    bool isLeaf; // 是否为叶子节点
    if (isLeaf) {
        std::string column; // 列名
        SQLOperator op; // 操作符
        std::string value; // 比较值
    } else {
        bool isAnd; // 是否使用 AND 连接
        std::vector<node> params; // 连接参数
    }
};
```

例如输入为：

```json
{
    "where": {
        "isLeaf": false,
        "isAnd": true,
        "params": [
            {
                "isLeaf": true,
                "column": "A",
                "op": "SQLOperator::EQUAL",
                "value": "123.08"
            },
            {
                "isLeaf": false,
                "isAnd": false,
                "params": [
                    {
                        "isLeaf": true,
                        "column": "B",
                        "op": "SQLOperator::LIKE",
                        "value": "%ASCAS%"
                    },
                    {
                        "isLeaf": true,
                        "column": "C",
                        "op": "SQLOperator::GREATER",
                        "value": "10"
                    }
                ]
            }
        ]
    },

}
```

生成的 SQL 为：

```sql
(A == 123.08) AND ((B LIKE "%ASCAS%") OR (C > 10))
```

需要判断 `SQLOperator` 是否匹配数据格式，例如 NUMBER 类型数据就不可以使用 REGEXP。

### order 参数

```cpp
class node {
    public:

    std::string column; // 列名
    bool isASC; // 是否为正序
}
std::vector<node> order;
```

例如输入为：

```json
[
    {
        "column": "A",
        "isASC": true
    },
    {
        "column": "B",
        "isASC": false
    }
]
```

生成的 SQL 为：

```sql
A ASC, B DESC
```