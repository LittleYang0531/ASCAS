<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';
import { newFetch } from '../../../utils/fetch';
import { API_BASE_URL } from '../../../config';
import { watch } from 'vue';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });

onMounted(() => {
    if (model.value == "") model.value = "0";
});

async function refreshSensor() {
    var data = await (await newFetch(`${API_BASE_URL}/sensors/${label.props.def}/query?t=${Math.round(Date.now() / 1000)}&count=1`)).json();
    model.value = data.items[0][0].toString();
}

watch(model, (val) => {
    if (val == "") refreshSensor();
});

onMounted(() => {
    if (model.value == "") refreshSensor();
});
</script>

<template>
    <v-number-input
        :model-value="Number(model)"
        variant="outlined"
        density="comfortable"
        hide-details
        :precision="null"
        disabled
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
    <v-btn
        icon="$mdiRefresh"
        color="primary"
        @click="refreshSensor()"
    ></v-btn>
</template>