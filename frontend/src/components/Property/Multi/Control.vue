<script setup lang="ts">
import type { RecordProperty } from '../../../models/crop';
import { isJSON } from '../../../utils/json';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
</script>

<template>
    <v-select
        :model-value="!isJSON(model!) ? [] : JSON.parse(model!)"
        variant="outlined"
        density="comfortable"
        hide-details
        multiple
        :items="label.props.options"
        @update:model-value="(val) => model = JSON.stringify(val.sort())"
        :disabled="label.disabled"
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
    </v-select>
    <p
        v-if="label.props.unit != ''"
        class="ma-0"
    >（{{ label.props.unit }}）</p>
</template>