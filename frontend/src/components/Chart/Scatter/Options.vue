<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const model = defineModel<Record<string, unknown> >('model', { required: true });
const props = defineProps<{
    properties: Array<RecordProperty>
}>();

function propFilter(p: RecordProperty) {
    return p.type == 'RecordPropertyType::NUMBER';
}

function propFilterType(p: RecordProperty) {
    return p.type == 'RecordPropertyType::SELECT' || p.type == 'RecordPropertyType::MULTI';
}

onMounted(() => {
    if (!model.value['scatter.columnX']) {
        var prop = props.properties.find(propFilter);
        if (prop) model.value['scatter.columnX'] = prop.name;
    }
    if (!model.value['scatter.columnY']) {
        var prop = props.properties.find(propFilter);
        if (prop) model.value['scatter.columnY'] = prop.name;
    }
});
</script>

<template>
    <v-select
        class="mt-4"
        v-model="model['scatter.columnX']"
        :items="properties.filter(propFilter).map((p) => ({ title: p.title, value: p.name }))"
        label="选择X轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['scatter.columnY']"
        :items="properties.filter(propFilter).map((p) => ({ title: p.title, value: p.name }))"
        label="选择Y轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['scatter.columnType']"
        :items="properties.filter(propFilterType).map((p) => ({ title: p.title, value: p.name }))"
        label="选择分组（可选）"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
        clearable
    ></v-select>
</template>