export const API_BASE_URL = import.meta.env.PROD ? 'https://ascas.littleyang.com.cn/api' : 'http://localhost:8080';
export const WS_BASE_URL = import.meta.env.PROD ? 'wss://ascas.littleyang.com.cn/api' : 'ws://localhost:8080';
export const WEB_NAME = 'ASCAS';
export const imageMaxSize = 512;
export const avatarMaxSize = 128;

export const login_background_urls = [
    `${import.meta.env.BASE_URL}bg1.jpg`,
    `${import.meta.env.BASE_URL}bg2.jpg`,
];

export const index_background_urls = [
    `${import.meta.env.BASE_URL}index1.jpg`,
    `${import.meta.env.BASE_URL}index2.jpg`,
    `${import.meta.env.BASE_URL}index3.jpg`,
    `${import.meta.env.BASE_URL}index4.jpg`,
    `${import.meta.env.BASE_URL}index5.jpg`,
];

export const recommendPropertyName = [
    "株高",
    "茎粗",
    "主茎节数",
    "分枝数",
    "叶长",
    "叶宽",
    "叶面积",
    "叶色",
    "叶柄长",
    "冠幅",
    "株型",
    "株幅",
    "节间长度",
    "主蔓长",
    "侧蔓数",
    "叶片数",
    "穗长",
    "穗粗",
    "穗行数",
    "行粒数",
    "单穗粒重",
    "千粒重",
    "果长",
    "果宽",
    "果径",
    "果形",
    "单果重",
    "果肉厚",
    "果皮厚度",
    "心室数",
    "坐果率",
    "结荚数",
    "荚长",
    "荚宽",
    "单株荚数",
    "根长",
    "根鲜重",
    "根干重",
    "根系活力",
    "出苗率",
    "成活率",
    "生育期",
    "播种期",
    "开花期",
    "结果期",
    "收获期",
    "抗病性",
    "抗虫性",
    "抗旱性",
    "抗寒性",
    "耐涝性",
    "耐盐碱性",
    "耐肥性",
    "倒伏率",
    "裂果率",
    "畸形果率",
    "可溶性固形物",
    "维生素C含量",
    "含糖量",
    "酸度",
    "粗纤维含量",
    "蛋白质含量",
    "脂肪含量",
    "淀粉含量",
    "水分含量",
    "硬度",
    "亩产",
    "单株产量",
    "总产量",
    "出籽率",
    "出仁率",
    "发芽率",
    "净度",
    "耐储运性",
    "株鲜重",
    "株干重",
    "叶片鲜重",
    "叶片干重",
    "茎鲜重",
    "茎干重",
    "果实耐压力",
    "可溶性糖",
    "可滴定酸",
    "氨基酸总量",
    "锌含量",
    "铁含量",
    "钙含量",
    "镁含量",
    "钾含量",
    "磷含量"
];

export const recommendPropertyUnit = [
    // 长度
    "厘米/cm",
    "毫米/mm",
    "米/m",
    // 重量
    "克/g",
    "千克/kg",
    "吨/t",
    "毫克/mg",
    "微克/μg",
    "公斤",
    // 数量
    "个",
    "片",
    "条",
    "根",
    "朵",
    "粒",
    "枚",
    "行",
    "株",
    "荚",
    "穗",
    "簇",
    // 时间
    "天/d",
    "小时/h",
    // 温度
    "摄氏度/°C",
    // 面积/体积
    "平方厘米/cm²",
    "平方米/m²",
    "立方厘米/cm³",
    // 容积
    "升/L",
    "毫升/mL",
    // 浓度/含量
    "百分比/%",
    "毫克/千克/mg/kg",
    "毫克/100克",
    // 环境/气象
    "勒克斯/lx",
    "兆帕/MPa",
    "百帕/hPa",
    "毫摩尔/升/mmol/L",
    // 特殊
    "级",
    "pH值"
];

// const defaultConfig = {
//     apiBase: API_BASE_URL
// };

// export function getConfig() {
    
// }