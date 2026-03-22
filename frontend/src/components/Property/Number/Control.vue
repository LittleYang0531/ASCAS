<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });

onMounted(() => {
    if (model.value == "") model.value = "0";
});
</script>

<template>
    <v-number-input
        :model-value="Number(model)"
        variant="outlined"
        density="comfortable"
        hide-details
        @update:model-value="(val) => model = val.toString()"
        :precision="null"
        :disabled="label.disabled"
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
    </v-number-input>
    <p
        v-if="label.props.unit != ''"
        class="ma-0"
    >（{{ label.props.unit }}）</p>
</template>