<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, onMounted, onUnmounted, ref, type Ref } from 'vue';
import type { Sensor } from '../../models/sensor';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL, WS_BASE_URL } from '../../config';
import SensorOverview from '../../components/Sensor/Overview.vue';
import draggable from 'vuedraggable';
import { locate } from '../../router';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';

async function load(to: any, from: any, next: any) {
    to; from;
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/sensors/list`)).json();

    next((e: any) => e.loading({
        sensors: res.items
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const sensors: Ref<Sensor[]> = ref([]);

function loading(data: any) {
    sensors.value = data.sensors;
    loaded.value = true;
}

defineExpose({ loading });

var ws: WebSocket;
onMounted(() => {
    ws = new WebSocket(`${WS_BASE_URL}/sensors/list/websocket`);
    ws.onmessage = (event) => {
        var data = event.data.split(' ');
        var sid = Number(data[0]);
        var value = Number(data[1]);
        var createdAt = Number(data[2]);
        var sensor = sensors.value.find(e => e.sid == sid);
        if (sensor) {
            sensor.data?.push(value);
            sensor.createdAt?.push(createdAt);
        }
    };
})

async function remove(sid: number) {
    await (await newFetch(`${API_BASE_URL}/sensors/${sid}/remove`, {
        method: 'POST'
    })).json();
    showMsg(MessageType.Success, '删除成功');
    sensors.value = sensors.value.filter(e => e.sid != sid);
}

onUnmounted(() => {
    ws.close();
})
</script>

<template>
    <div v-if="loaded">
        <div class="d-flex align-center justify-space-between mb-4 mt-4">
            <h1 class="ma-0">传感器</h1>
            <v-btn color="primary" @click="locate('/sensors/create')" prepend-icon="$mdiPlus">新建传感器</v-btn>
        </div>
        <v-divider></v-divider>
        <draggable v-model="sensors" animation="200" item-key="name" class="d-flex flex-wrap ga-8 mt-4">
            <template v-slot:item="{ element }">
                <SensorOverview :sensor="element" @remove="remove(element.sid)"/>
            </template>
        </draggable>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>