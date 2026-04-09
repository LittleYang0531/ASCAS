<script setup lang="ts">
import { use } from 'echarts/core';
import { ScatterChart } from 'echarts/charts';
import { GridComponent, TooltipComponent } from 'echarts/components';
import { CanvasRenderer } from 'echarts/renderers';
use([GridComponent, TooltipComponent, ScatterChart, CanvasRenderer]);

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

function generateData() {
    var xColumn = items.options['scatter.columnX'] as string;
    var yColumn = items.options['scatter.columnY'] as string;
    var groupColumn = items.options['scatter.columnType'] as string;
    var res = [];
    if (groupColumn == undefined || groupColumn == '') {
        var tmp = [];
        for (var i = 0; i < items.data.length; i++) {
            var x = Number(items.data[i]![xColumn]!);
            var y = Number(items.data[i]![yColumn]!);
            tmp.push([x, y]);
        }
        res.push({
            symbolSize: 15,
            data: tmp,
            type: 'scatter',
            emphasis: {
                focus: 'series'
            },
            encode: { tooltip: [0, 1] }
        });
    } else {
        var props = getProp(groupColumn);
        for (var i = 0; i < props?.options?.length!; i++) {
            var typeName = props?.options![i]!;
            var tmp = [];
            for (var j = 0; j < items.data.length; j++) {
                var x = Number(items.data[j]![xColumn]!);
                var y = Number(items.data[j]![yColumn]!);
                var type = items.data[j]![groupColumn]!;
                if (props?.type == 'RecordPropertyType::MULTI') {
                    var arr = JSON.parse(type as string) as Array<string>;
                    if (!arr.includes(typeName)) continue;
                } 
                else if (typeName != type) continue;
                tmp.push([x, y]);
            }
            res.push({
                name: typeName,
                symbolSize: 15,
                data: tmp,
                type: 'scatter',
                emphasis: {
                    focus: 'series'
                },
                encode: { tooltip: [0, 1] }
            });
        }
    }
    return res;
}
</script>

<template>
    <VChart class="chart" :option="{
        xAxis: {},
        yAxis: {},
        tooltip: {
            position: 'top',
            axisPointer: {
                show: true,
                type: 'cross',
                lineStyle: {
                    type: 'dashed',
                    width: 1
                }
            }
        },
        series: generateData()
    }" />
</template>

<style lang="css" scoped>
.chart {
  height: 400px;
}
</style>