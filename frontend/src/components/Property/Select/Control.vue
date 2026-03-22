<script setup lang="ts">
import type { RecordProperty } from '../../../models/crop';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
</script>

<template>
    <v-select
        v-model="model"
        variant="outlined"
        density="comfortable"
        hide-details
        :items="label.props.options"
        @update:model-value="(val) => model = val.toString()"
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