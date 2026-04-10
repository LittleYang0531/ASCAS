<script setup lang="ts">
import { ref } from 'vue';
import { API_BASE_URL, recommendPropertyUnit } from '../../config';
import { newFetch } from '../../utils/fetch';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { locate } from '../../router';
import SensorIdDialog from '../../components/Dialog/SensorIdDialog.vue';

const title = ref("");
const unit = ref("");
const fetching = ref(false);
const sid = ref(0);
const sensorId = ref("");
const dialog = ref(false);

async function submit() {
    if (title.value == "") {
        alert("传感器名称不能为空");
        return;
    }
    fetching.value = true;

    var res = await(await newFetch(`${API_BASE_URL}/sensors/create`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            unit: unit.value
        })
    }, () => { fetching.value = false; })).json();

    sid.value = res.id;
    sensorId.value = res.sensorId;
    dialog.value = true;
    showMsg(MessageType.Success, "传感器创建成功");
}

function jump() {
    locate('/sensors/' + sid.value);
}
</script>

<template>
    <h1>新建传感器</h1>
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
    <div class="mt-4 d-flex align-center justify-end mb-4">
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="submit()"
            :disabled="fetching"
        >新建</v-btn>
    </div>
    <SensorIdDialog v-model:open="dialog" :title="title" :sensorId="sensorId" @submit="jump"></SensorIdDialog>
</template>