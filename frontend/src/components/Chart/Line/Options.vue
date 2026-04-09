<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const model = defineModel<Record<string, unknown> >('model', { required: true });
const props = defineProps<{
    properties: Array<RecordProperty>
}>();

function propFilterX(p: RecordProperty) {
    return p.type == 'RecordPropertyType::DATE';
}

function propFilterY(p: RecordProperty) {
    return p.type == 'RecordPropertyType::NUMBER';
}

function propFilterType(p: RecordProperty) {
    return p.type == 'RecordPropertyType::SELECT' || p.type == 'RecordPropertyType::MULTI';
}

onMounted(() => {
    if (!model.value['line.columnX']) {
        var prop = props.properties.find(propFilterX);
        if (prop) model.value['line.columnX'] = prop.name;
    }
    if (!model.value['line.columnY']) {
        var prop = props.properties.find(propFilterY);
        if (prop) model.value['line.columnY'] = prop.name;
    }
    if (!model.value['line.unit']) {
        model.value['line.unit'] = 'day';
    }
    if (!model.value['line.aggregation']) {
        model.value['line.aggregation'] = 'avg';
    }
});
</script>

<template>
    <v-select
        class="mt-4"
        v-model="model['line.columnX']"
        :items="properties.filter(propFilterX).map((p) => ({ title: p.title, value: p.name }))"
        label="选择X轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.columnY']"
        :items="properties.filter(propFilterY).map((p) => ({ title: p.title, value: p.name }))"
        label="选择Y轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.unit']"
        :items="[
            { title: '秒', value: 'second' },
            { title: '分钟', value: 'minute' },
            { title: '小时', value: 'hour' },
            { title: '天', value: 'day' },
            { title: '月', value: 'month' },
            { title: '年', value: 'year' }
        ]"
        label="选择时间单位"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.columnType']"
        :items="properties.filter(propFilterType).map((p) => ({ title: p.title, value: p.name }))"
        label="选择分组（可选）"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
        clearable
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.aggregation']"
        :items="[
            { title: '平均值', value: 'avg' },
            { title: '总和', value: 'sum' },
            { title: '最大值', value: 'max' },
            { title: '最小值', value: 'min' },
            { title: '计数', value: 'count' }
        ]"
        label="选择聚合方式"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
</template>