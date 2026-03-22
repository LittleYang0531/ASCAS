<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const model = defineModel<Record<string, unknown> >('model', { required: true });
const props = defineProps<{
    properties: Array<RecordProperty>
}>();

function propFilter(p: RecordProperty) {
    return p.type == 'RecordPropertyType::SELECT' || p.type == 'RecordPropertyType::MULTI';
}

onMounted(() => {
    if (!model.value['bar.column']) {
        var prop = props.properties.find(propFilter);
        if (prop) model.value['bar.column'] = prop.name;
    }
});
</script>

<template>
    <v-select
        class="mt-4"
        v-model="model['bar.column']"
        :items="properties.filter(propFilter).map((p) => ({ title: p.title, value: p.name }))"
        label="选择一列"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
</template>