<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL, recommendPropertyUnit } from '../../config';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { sleep } from '../../utils/sleep';
import { locate } from '../../router';
import RegenDialog from '../../components/Dialog/RegenDialog.vue';
import SensorIdDialog from '../../components/Dialog/SensorIdDialog.vue';

async function load(to: any, from: any, next: any) {
    from;
    NProgress.start();
    NProgress.inc();

    var sensor = await (await newFetch(`${API_BASE_URL}/sensors/${to.params.id}`)).json();

    next((e: any) => e.loading({
        sensor: sensor.item,
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const title = ref("");
const unit = ref("");
const fetching = ref(false);
const sid = ref(0);
const sensorId = ref("");
const regenDialog = ref(false);
const dialog = ref(false);

function loading(data: any) {
    title.value = data.sensor.title;
    unit.value = data.sensor.unit;
    sid.value = data.sensor.sid;
    loaded.value = true;
}

async function submit() {
    if (title.value == "") {
        alert("传感器名称不能为空");
        return;
    }
    fetching.value = true;

    await(await newFetch(`${API_BASE_URL}/sensors/${sid.value}/edit`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            unit: unit.value
        })
    }, () => { fetching.value = false; })).json();

    showMsg(MessageType.Success, "传感器修改成功");
    await sleep(1000);
    locate('/sensors/' + sid.value);
}

async function gen() {
    fetching.value = true;

    var res = await (await newFetch(`${API_BASE_URL}/sensors/${sid.value}/generate`, {
        method: "POST",
    }, () => { fetching.value = false; })).json();

    fetching.value = false;
    sensorId.value = res.sensorId;
    dialog.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1>编辑传感器</h1>
        <v-text-field
            v-model="title"
            variant="outlined"
            density="comfortable"
            hide-details
        >
            <template v-slot:label>
                <span>传感器名称</span>  
                <span style="color: red">&nbsp;*</span>
            </template>
        </v-text-field>
        <v-combobox
            v-model="unit"
            label="数据单位"
            variant="outlined"
            density="comfortable"
            hide-details
            class="mt-4"
            :items="recommendPropertyUnit"
        ></v-combobox>
        <div class="mt-4 d-flex align-center justify-space-between mb-4">
            <v-btn
                prepend-icon="$mdiRefresh"
                color="error"
                @click="regenDialog = true"
                :disabled="fetching"
            >重新生成 Token</v-btn>
            <v-btn
                prepend-icon="$mdiCheck"
                color="primary"
                @click="submit()"
                :disabled="fetching"
            >修改</v-btn>
        </div>
        <RegenDialog v-model:open="regenDialog" @generate="gen"></RegenDialog>
        <SensorIdDialog v-model:open="dialog" :title="title" :sensorId="sensorId" @submit=""></SensorIdDialog>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>