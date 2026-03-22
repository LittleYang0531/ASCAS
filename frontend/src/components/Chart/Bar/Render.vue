<script setup lang="ts">
import "echarts";
import VChart from "vue-echarts";
import type { RecordProperty } from "../../../models/crop";

const items = defineProps<{
    options: Record<string, unknown>,
    data: Array<Record<string, string>>,
    props: Array<RecordProperty>
}>();

function getProp() {
    var column = items.options['bar.column'] as string;
    return items.props.find((p) => p.name == column);
}

function generateData() {
    var res = [];
    var column = items.options['bar.column'] as string;
    var map: Record<string, number> = {};
    var prop = items.props.find((p) => p.name == column);
    for (const item of prop?.options!) map[item] = 0;
    for (const item of items.data) {
        if (prop?.type == "RecordPropertyType::MULTI") {
            var values = JSON.parse(item[column]!);
            for (const value of values) {
                if (value) {
                    if (map[value]) map[value]!++;
                    else map[value] = 1;
                }
            }
        } else {
            var value = item[column];
            if (value) {
                if (map[value]) map[value]!++;
                else map[value] = 1;
            }
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
            trigger: 'axis',
            axisPointer: {
                type: 'shadow'
            }
        },
        grid: {
            left: '3%',
            right: '4%',
            bottom: '3%',
            containLabel: true
        },
        xAxis: [
            {
                type: 'category',
                data: generateData().map((item) => item.name),
                axisTick: {
                    alignWithLabel: true
                }
            }
        ],
        yAxis: [
            {
                type: 'value'
            }
        ],
        series: [
            {
                name: getProp()?.title || '',
                type: 'bar',
                barWidth: '60%',
                data: generateData().map((item) => item.value)
            }
        ]
    }" />
</template>

<style lang="css" scoped>
.chart {
  height: 400px;
}
</style>