<script setup lang="ts">
import { use } from 'echarts/core';
import { LineChart } from 'echarts/charts';
import {
    TooltipComponent,
    LegendComponent,
    GridComponent,
    DataZoomComponent
} from 'echarts/components';
import { CanvasRenderer } from 'echarts/renderers';
use([
    TooltipComponent,
    LegendComponent,
    GridComponent,
    DataZoomComponent,
    LineChart,
    CanvasRenderer
]);

import VChart from "vue-echarts";
import type { Sensor } from '../../models/sensor';
import { formatTime } from '../../utils/time';
import { locate } from '../../router';
import DeleteDialog from '../Dialog/DeleteDialog.vue';
import { ref } from 'vue';

const props = defineProps<{
    sensor: Sensor
}>();
const emits = defineEmits<{
    (e: 'remove'): void
}>();
const dialog = ref(false);

function getUnit() {
    var part = props.sensor.unit?.split("/");
    if (part?.length == 1) return props.sensor.unit;
    return part?.slice(part.length / 2).join("/");
}
</script>

<template>
    <v-card class="SensorOverview pa-4">
        <div class="ml-5 mr-5 mt-3 d-flex align-center justify-space-between">
            <h2 class="ma-0 cursor-pointer" @click="locate(`/sensors/${props.sensor.sid}`)">{{ props.sensor.title }}</h2>
            <div class="d-flex align-center ga-2">
                <v-btn color="primary" text @click="locate(`/sensors/${props.sensor.sid}/edit`)" prepend-icon="$mdiPencil">编辑</v-btn>
                <v-btn color="error" text @click="dialog = true" prepend-icon="$mdiDelete">删除</v-btn>
            </div>
        </div>
        <DeleteDialog v-model:open="dialog" @delete="$emit('remove')"></DeleteDialog>
        <VChart class="chart" :option="{
            tooltip: {
                trigger: 'axis'
            },
            grid: {
                top: '10%',
                bottom: '10%'
            },
            dataZoom: [
                {
                    type: 'slider',
                    show: false,
                    realtime: true,
                    startValue: Math.max(0, props.sensor.createdAt?.length! - 10),
                    endValue: props.sensor.createdAt?.length! - 1,
                    filterMode: 'filter'
                }
            ],
            xAxis: {
                type: 'category',
                data: props.sensor.createdAt?.map(e => formatTime(e, '%H:%M'))
            },
            yAxis: {
                type: 'value',
                axisLabel: {
                    formatter: `{value} ${getUnit()}`
                }
            },
            series: [
                {
                    type: 'line', 
                    data: props.sensor.data,
                    smooth: true
                }
            ]
        }" v-if="sensor.data?.length"/>
        <div class="chart full-width d-flex align-center flex-column justify-center" v-else>
            <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
            <p>暂无数据</p>
        </div>
    </v-card>
</template>

<style lang="css" scoped>
.SensorOverview {
    width: calc(50% - 16px);
}

.chart {
    height: 250px;
}
</style>