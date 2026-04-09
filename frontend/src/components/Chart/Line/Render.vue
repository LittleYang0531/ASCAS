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

function format(value: string, unit: string) {
    var date = new Date(Number(value) * 1000);
    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    var day = date.getDate();
    var hour = date.getHours();
    var minute = date.getMinutes();
    var second = date.getSeconds();
    if (unit == 'second') return `${year}-${month}-${day} ${hour}:${minute}:${second}`;
    else if (unit == 'minute') return `${year}-${month}-${day} ${hour}:${minute}`;
    else if (unit == 'hour') return `${year}-${month}-${day} ${hour}h`;
    else if (unit == 'day') return `${year}-${month}-${day}`;
    else if (unit == 'month') return `${year}-${month}`;
    else if (unit == 'year') return `${year}`;
    else return value;
}

function getXAxis() {
    var column = items.options['line.columnX'] as string;
    var prop = getProp(column);
    var unit = items.options['line.unit'] as string;
    if (prop?.type == "RecordPropertyType::DATE") {
        return items.data.map((item) => format(item[column]!, unit))
                         .sort()
                         .filter((value, index, self) => self.indexOf(value) == index);
    }
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
            var x = format(items.data[j]![xColumn]!, unit);
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
                var x = format(items.data[j]![xColumn]!, unit);
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