<script setup lang="ts">
import { use } from 'echarts/core';
import { LineChart } from 'echarts/charts';
import {
    TooltipComponent,
    LegendComponent,
    GridComponent
} from 'echarts/components';
import { CanvasRenderer } from 'echarts/renderers';
use([
    TooltipComponent,
    LegendComponent,
    GridComponent,
    LineChart,
    CanvasRenderer
]);

import VChart from "vue-echarts";
import type { RecordProperty } from "../../../models/crop";

const items = defineProps<{
    options: Record<string, unknown>,
    data: Array<Record<string, string>>,
    props: Array<RecordProperty>
}>();

function getProp(column: string) {
    return items.props.find((p) => p.name == column);
}

const monthMap = ["一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"];
const dayMap = ["周一", "周二", "周三", "周四", "周五", "周六", "周日"];

function formatTime(value: string, unit: string) {
    var date = new Date(Number(value) * 1000);
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

function getXAxis() {
    var column = items.options['line.columnX'] as string;
    var unit = items.options['line.unit'] as string;
    return items.data.map((item) => formatTime(item[column]!, unit))
                     .sort()
                     .filter((value, index, self) => self.indexOf(value) == index);
}

function getValue() {
    var res = [];
    var groupColumn = items.options['line.columnType'] as string;
    var xAxis = getXAxis();
    var props = getProp(groupColumn);
    var xColumn = items.options['line.columnX'] as string;
    var yColumn = items.options['line.columnY'] as string;
    var unit = items.options['line.unit'] as string;
    var aggregation = items.options['line.aggregation'] as string;
    console.log(groupColumn);
    if (groupColumn == undefined || groupColumn == '') {
        var data: Record<string, number> = {};
        var cnt: Record<string, number> = {};
        for (var j = 0; j < items.data.length; j++) {
            var x = formatTime(items.data[j]![xColumn]!, unit);
            var y = Number(items.data[j]![yColumn]!);
            switch (aggregation) {
                case 'avg':
                    if (!data[x]) data[x] = 0;
                    data[x]! += y;
                    if (!cnt[x]) cnt[x] = 0;
                    cnt[x]!++;
                    break;
                case 'sum':
                    if (!data[x]) data[x] = 0;
                    data[x]! += y;
                    break;
                case 'max':
                    if (!data[x]) data[x] = 0;
                    data[x]! = Math.max(data[x]!, y);
                    break;
                case 'min':
                    if (!data[x]) data[x] = y;
                    else data[x]! = Math.min(data[x]!, y);
                    break;
                case 'count':
                    if (!data[x]) data[x] = 0;
                    data[x]!++;
                    break;
            }
        }
        if (aggregation == 'avg') for (x in data) data[x]! /= cnt[x]!;
        var realdata = [];
        for (var j = 0; j < xAxis?.length!; j++) {
            var x = xAxis![j]!;
            if (data[x] != undefined) realdata.push(data[x]!);
            else realdata.push(null);
        }
        res.push({ type: 'line', data: realdata });
    }
    else {
        for (var i = 0; i < props?.options?.length!; i++) {
            var data: Record<string, number> = {};
            var cnt: Record<string, number> = {};
            var typeName = props?.options![i]!;
            for (var j = 0; j < items.data.length; j++) {
                var x = formatTime(items.data[j]![xColumn]!, unit);
                var y = Number(items.data[j]![yColumn]!);
                var type = items.data[j]![groupColumn]!;
                if (props?.type == "RecordPropertyType::MULTI") {
                    var arr = JSON.parse(type as string) as Array<string>;
                    if (!arr.includes(typeName)) continue;
                }
                else if (type != typeName) continue;

                switch (aggregation) {
                    case 'avg':
                        if (!data[x]) data[x] = 0;
                        data[x]! += y;
                        if (!cnt[x]) cnt[x] = 0;
                        cnt[x]!++;
                        break;
                    case 'sum':
                        if (!data[x]) data[x] = 0;
                        data[x]! += y;
                        break;
                    case 'max':
                        if (!data[x]) data[x] = 0;
                        data[x]! = Math.max(data[x]!, y);
                        break;
                    case 'min':
                        if (!data[x]) data[x] = y;
                        else data[x]! = Math.min(data[x]!, y);
                        break;
                    case 'count':
                        if (!data[x]) data[x] = 0;
                        data[x]!++;
                        break;
                }
            }
            for (x in data) {
                if (aggregation == 'avg') data[x]! /= cnt[x]!;
            }
            var realdata = [];
            for (var j = 0; j < xAxis?.length!; j++) {
                var x = xAxis![j]!;
                if (data[x] != undefined) realdata.push(data[x]!);
                else realdata.push(null);
            }
            res.push({ name: typeName, type: 'line', data: realdata });
        }
    }
    return res;
}
</script>

<template>
    <VChart class="chart" :option="{
        tooltip: {
            trigger: 'axis'
        },
        xAxis: {
            type: 'category',
            data: getXAxis()
        },
        yAxis: {
            type: 'value'
        },
        series: getValue()
    }" /> 
</template>

<style lang="css" scoped>
.chart {
  height: 400px;
}
</style>