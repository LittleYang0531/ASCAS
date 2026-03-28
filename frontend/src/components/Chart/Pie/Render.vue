<script setup lang="ts">
import { use } from 'echarts/core';
import { PieChart } from 'echarts/charts';
import { TooltipComponent, LegendComponent } from 'echarts/components';
import { CanvasRenderer } from 'echarts/renderers';
use([TooltipComponent, LegendComponent, PieChart, CanvasRenderer]);

import VChart from "vue-echarts";
import type { RecordProperty } from "../../../models/crop";

const items = defineProps<{
    options: Record<string, unknown>,
    data: Array<Record<string, string>>,
    props: Array<RecordProperty>
}>();

function getProp() {
    var column = items.options['pie.column'] as string;
    return items.props.find((p) => p.name == column);
}

function generateData() {
    var res = [];
    var column = items.options['pie.column'] as string;
    var map: Record<string, number> = {};
    var prop = getProp();
    for (const item of prop?.options!) map[item] = 0;
    for (const item of items.data) {
        var value = item[column];
        if (value) {
            if (map[value]) map[value]!++;
            else map[value] = 1;
        }
    }
    for (const item of prop?.options!) {
        res.push({ name: item, value: map[item] });
    }
    return res;
}
</script>

<template>
    <VChart class="chart" :option="{
        tooltip: {
            trigger: 'item'
        },
        legend: {
            top: '5%',
            left: 'center'
        },
        series: [
            {
                name: getProp()?.title || '',
                type: 'pie',
                radius: ['40%', '70%'],
                avoidLabelOverlap: false,
                itemStyle: {
                    borderRadius: 10,
                    borderColor: '#fff',
                    borderWidth: 2
                },
                label: {
                    show: false,
                    position: 'center'
                },
                labelLine: {
                    show: false
                },
                data: generateData()
            }
        ]
    }" />
</template>

<style lang="css" scoped>
.chart {
  height: 400px;
}
</style>