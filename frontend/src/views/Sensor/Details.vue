<script lang="ts">
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
import NProgress from 'nprogress';
import { defineComponent, onMounted, onUnmounted, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL, WS_BASE_URL } from '../../config';
import type { Sensor } from '../../models/sensor';
import { formatTime } from '../../utils/time';

async function load(to: any, from: any, next: any) {
    from;

    NProgress.start();
    NProgress.inc();

    var sensor = await (await newFetch(`${API_BASE_URL}/sensors/${to.params.id}`)).json();
    var data = await (await newFetch(`${API_BASE_URL}/sensors/${to.params.id}/query`)).json();
    for (var i = 0; i < data.items.length; i++) 
        sensor.item.data.push(data.items[i][0]),
        sensor.item.createdAt.push(data.items[i][1]);

    next((e: any) => e.loading({
        sensor: sensor.item,
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const sensor: Ref<Sensor> = ref({});
const lastStart = ref(100);

function loading(value: any) {
    sensor.value = value.sensor;
    loaded.value = true;
}

function getUnit() {
    var part = sensor.value.unit?.split("/");
    if (part?.length == 1) return sensor.value.unit;
    return part?.slice(part.length / 2).join("/");
}

async function fetchData() {
    var data = await (await newFetch(`${API_BASE_URL}/sensors/${sensor.value.sid}/query?t=${sensor.value.createdAt![0]! - 1}`)).json();
    for (var i = data.items.length - 1; i >= 0; i--)
        sensor.value.data?.unshift(data.items[i][0]),
        sensor.value.createdAt?.unshift(data.items[i][1]);
    
}

function ondatazoom(e: any) {
    if (lastStart.value != 0 && e.start == 0) fetchData();
    lastStart.value = e.start;
}

defineExpose({ loading });

var ws: WebSocket;
onMounted(() => {
    function connectWebSocket() {
        ws = new WebSocket(`${WS_BASE_URL}/sensors/${sensor.value.sid}/websocket`);
        ws.onmessage = (e) => {
            var data = e.data.split(" ");
            sensor.value.data?.push(Number(data[0]));
            sensor.value.createdAt?.push(Number(data[1]));
        }
        ws.onclose = (e) => {
            console.log("WebSocket closed");
            if (e.code != 1000) connectWebSocket();
        };
        ws.onerror = (error) => {
            console.error("WebSocket error:", error);
            ws.close();
        };
    }
    connectWebSocket();
})

onUnmounted(() => {
    ws.send("bye");
    ws.close();
});
</script>

<template>
    <div v-if="loaded">
        <h1>{{ sensor.title }}</h1>
        <VChart class="chart" @datazoom="ondatazoom" :option="{
            tooltip: {
                trigger: 'axis'
            },
            grid: {
                top: '10%',
                left: '3%',
                right: '3%',
            },
            dataZoom: [
                {
                    type: 'slider',
                    show: true,
                    realtime: true,
                    filterMode: 'none'
                }
            ],
            xAxis: {
                type: 'category',
                data: sensor.createdAt?.map(e => formatTime(e, '%H:%M'))
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
                    data: sensor.data,
                    smooth: true
                }
            ]
        }" v-if="sensor.data?.length"/>
        <div class="chart full-width d-flex align-center flex-column justify-center" v-else>
            <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
            <p>暂无数据</p>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.chart {
    height: 500px;
}
</style>