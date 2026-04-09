<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });

function refreshTime() {
    model.value = Math.round((new Date()).getTime() / 1000).toString();
}

function formatTime(timestamp: number) {
    const date = new Date(timestamp * 1000);
    return date.toLocaleString();
}

onMounted(() => {
    if (model.value == "") refreshTime();
});
</script>

<template>
    <v-text-field
        :model-value="formatTime(Number(model))"
        variant="outlined"
        density="comfortable"
        hide-details
        disabled
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>

    </v-text-field>
    <v-btn
        icon="$mdiRefresh"
        color="primary"
        @click="refreshTime()"
    ></v-btn>
</template>