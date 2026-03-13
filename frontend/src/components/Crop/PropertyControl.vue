<script lang="ts" setup>
import type { RecordProperty } from '../../models/crop';

const label = defineProps<{ label: string, class: string, disabled: boolean }>();
const props = defineModel<RecordProperty>("props", { required: true });
</script>

<template>
    <v-number-input
        v-if="props.type == 'RecordPropertyType::NUMBER'"
        :model-value="Number(props.def)"
        :label="label.label"
        variant="outlined"
        density="comfortable"
        hide-details
        @update:model-value="(val) => props.def = val.toString()"
        :class="label.class"
        :precision="null"
        :disabled="label.disabled"
    ></v-number-input>
    <v-text-field
        v-if="props.type == 'RecordPropertyType::STRING'"
        v-model="props.def"
        :label="label.label"
        variant="outlined"
        density="comfortable"
        :class="label.class"
        hide-details
        :disabled="label.disabled"
    ></v-text-field>
    <v-select
        v-if="props.type == 'RecordPropertyType::SELECT'"
        v-model="props.def"
        :label="label.label"
        variant="outlined"
        density="comfortable"
        hide-details
        :class="label.class"
        :items="props.options"
        @update:model-value="(val) => props.def = val.toString()"
        :disabled="label.disabled"
    ></v-select>
    <v-select
        v-if="props.type == 'RecordPropertyType::MULTI'"
        :model-value="props.def || props.def == '' ? [] : JSON.parse(props.def!)"
        :label="label.label"
        variant="outlined"
        density="comfortable"
        hide-details
        multiple
        :class="label.class"
        :items="props.options"
        @update:model-value="(val) => props.def = JSON.stringify(val)"
        :disabled="label.disabled"
    ></v-select>
</template>