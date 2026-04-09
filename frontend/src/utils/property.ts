export const propertyTypes = [
    { 
        title: "数值型", 
        value: "RecordPropertyType::NUMBER",
        options: false,
        allowDef: true
    },
    { 
        title: "字符串型", 
        value: "RecordPropertyType::STRING",
        options: false,
        allowDef: true
    },
    { 
        title: "单项选择", 
        value: "RecordPropertyType::SELECT",
        options: true,
        allowDef: true
    },
    { 
        title: "多项选择", 
        value: "RecordPropertyType::MULTI",
        options: true,
        allowDef: true
    },
    { 
        title: "定位坐标", 
        value: "RecordPropertyType::GEOMETRY",
        options: false,
        allowDef: false
    }, 
    { 
        title: "扫描二维码", 
        value: "RecordPropertyType::QRCODE",
        options: false,
        allowDef: false
    }, 
    {
        title: "当前时间",
        value: "RecordPropertyType::DATE",
        options: false,
        allowDef: false
    },
    { 
        title: "上传图片", 
        value: "RecordPropertyType::IMAGE",
        options: false,
        allowDef: false
    }
];

export const propertyTypeMap = propertyTypes.reduce((map, item) => {
    map[item.value] = item;
    return map;
}, {} as Record<string, typeof propertyTypes[0]>);

export function isNumeric(value: string) {
    return value == "RecordPropertyType::NUMBER" || value == "RecordPropertyType::DATE";
}