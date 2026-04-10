const monthMap = ["一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"];
const dayMap = ["周一", "周二", "周三", "周四", "周五", "周六", "周日"];

export function formatTime(value: number, unit: string) {
    var date = new Date(value * 1000);
    var year = date.getFullYear().toString();
    var month = (date.getMonth() + 1).toString().padStart(2, '0');
    var day = date.getDate().toString().padStart(2, '0');
    var hour = date.getHours().toString().padStart(2, '0');
    var hour12 = (date.getHours() % 12 || 12).toString().padStart(2, '0');
    var apm = date.getHours() < 12 ? 'AM' : 'PM';
    var minute = date.getMinutes().toString().padStart(2, '0');
    var second = date.getSeconds().toString().padStart(2, '0');
    var week = (date.getDay() == 0 ? 7 : date.getDay()).toString();
    var monthName = monthMap[Number(month) - 1];
    var weekName = dayMap[Number(week) - 1];
    var full = `${year}-${month}-${day} ${weekName} ${hour}:${minute}:${second}`;

    unit = unit.replace(/%a/g, weekName!)
                .replace(/%A/g, weekName!)
                .replace(/%b/g, monthName!)
                .replace(/%B/g, monthName!)
                .replace(/%c/g, full)
                .replace(/%d/g, day)
                .replace(/%H/g, hour)
                .replace(/%I/g, hour12)
                .replace(/%m/g, month)
                .replace(/%M/g, minute)
                .replace(/%p/g, apm)
                .replace(/%S/g, second)
                .replace(/%Y/g, year);

    return unit;
}
