<script setup lang="ts">
import { ref, type Ref } from 'vue';
import PieOptions from './Pie/Options.vue';
import PieRender from './Pie/Render.vue';
import BarOptions from './Bar/Options.vue';
import BarRender from './Bar/Render.vue';
import LineOption from './Line/Options.vue';
import LineRender from './Line/Render.vue';
import ScatterOptions from './Scatter/Options.vue';
import ScatterRender from './Scatter/Render.vue';
import type { RecordProperty } from '../../models/crop';

const items = defineProps<{
    properties: Array<RecordProperty>
}>();
const emits = defineEmits<{
    (e: 'getData', callback: (data: Array<Record<string, string>>) => void): void
}>();

const charts = [
    {
        title: "饼图",
        value: "pie",
        options: PieOptions,
        render: PieRender
    },
    {
        title: "柱状图",
        value: "bar",
        options: BarOptions,
        render: BarRender
    }, 
    {
        title: "折线图",
        value: "line",
        options: LineOption,
        render: LineRender
    },
    {
        title: "散点图",
        value: "scatter",
        options: ScatterOptions,
        render: ScatterRender
    }
];
const chartsMap = charts.reduce((map, obj) => {
    map[obj.value] = obj;
    return map;
}, {} as Record<string, typeof charts[0]>);

const options: Ref<Record<string, unknown>> = ref({});
const resultOptions: Ref<Record<string, unknown>> = ref({});
const chart = ref(charts[0]?.value);
const resultChart = ref(charts[0]?.value);
const data: Ref<Array<Record<string, string>>> = ref([]);
const rendered = ref(false);

function render() {
    emits('getData', (data2) => {
        resultOptions.value = JSON.parse(JSON.stringify(options.value));
        resultChart.value = chart.value;
        data.value = data2;
        rendered.value = true;
    });
}
</script>

<template>
    <v-select
        class="mt-2"
        v-model="chart"
        :items="charts.map((c) => ({ title: c.title, value: c.value }))"
        label="选择图表类型"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <component 
        :is="chartsMap[chart!]?.options" 
        v-model:model="options"
        :properties="items.properties"
    ></component>
    <div class="d-flex align-center justify-space-between mt-4">
        <v-btn color="error" @click="rendered = false" prepend-icon="$mdiClose" v-if="rendered">关闭图表</v-btn>
        <div v-else></div>
        <v-btn color="primary" @click="render" prepend-icon="$mdiChartBoxOutline">生成图表</v-btn>
    </div>
    <component 
        :is="chartsMap[resultChart!]?.render" 
        :options="resultOptions"
        :data="data"
        :props="items.properties"
        v-if="rendered"
    ></component>
</template>