<script setup lang="ts">
import type { RecordProperty } from '../../../models/crop';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
</script>

<template>
    <v-text-field
        v-model="model"
        variant="outlined"
        density="comfortable"
        hide-details
        :disabled="label.disabled"
        clearable
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
    </v-text-field>
    <p
        v-if="label.props.unit != ''"
        class="ma-0"
    >（{{ label.props.unit }}）</p>
</template>