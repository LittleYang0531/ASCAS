<script setup lang="ts">
import type { Ref } from 'vue';
import type { Crop } from '../../../models/crop';
import { onBeforeMount, ref } from 'vue';
import PropertyControl from '../../../components/Property/Control.vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import { newFetch } from '../../../utils/fetch';
import { API_BASE_URL } from '../../../config';
import { sleep } from '../../../utils/sleep';

const fetching = ref(false);
const values: Ref<Record<string, string>> = ref({});
const item = defineProps<{
    item: Crop
}>();
async function addRecord(callback = () => {}) {
    for (var i = 0; i < item.item.properties!.length; i++) {
        var prop = item.item.properties![i]!;
        if (prop.required) {
            if (prop.type == "RecordPropertyType::MULTI") {
                if (JSON.parse(values.value[prop.name!]!).length == 0) {
                    showMsg(MessageType.Error, `${prop.title}不能为空`);
                    return;
                }
            } else {
                if (values.value[prop.name!] == "") {
                    showMsg(MessageType.Error, `${prop.title}不能为空`);
                    return;
                }
            }
        }
    }
    fetching.value = true;
    await (await newFetch(`${API_BASE_URL}/crops/${item.item.cid}/records/add`, {
        method: "POST",
        body: JSON.stringify(values.value)
    }, () => { fetching.value = false; }));
    fetching.value = false;
    showMsg(MessageType.Success, "添加成功");
    callback();
}
async function addRecordAndExit() {
    await addRecord(async () => {
        await sleep(1000);
        window.location.href = `/crops/${item.item.cid}?page=simple`;
    });
}
async function addRecordAndContinue() {
    await addRecord(() => {
        for (var i = 0; i < item.item.properties!.length; i++) {
            var prop = item.item.properties![i]!;
            if (prop.type == "RecordPropertyType::SENSOR") values.value[prop.name!] = "";
            else values.value[prop.name!] = prop.def!;
        }
    });
}

onBeforeMount(() => {
    for (var i = 0; i < item.item.properties!.length; i++) {
        var prop = item.item.properties![i]!;
        if (prop.type == "RecordPropertyType::SENSOR") values.value[prop.name!] = "";
        else values.value[prop.name!] = prop.def!;
    }
})
</script>

<template>
    <h2 class="ma-0 mb-4">新记录信息</h2>
    <PropertyControl 
        v-for="prop in item.item.properties"
        v-model:model="values[prop.name!]!"
        :props="prop"
        :label="prop.title!"
        :disabled="false"
        :cropId="item.item.cid!"
        class="mt-4"
    ></PropertyControl>
    <div class="mt-4 d-flex align-center justify-end ga-4">
        <v-btn
            prepend-icon="$mdiExitToApp"
            color="error"
            @click="addRecordAndExit()"
            :disabled="fetching"
        >添加并终止</v-btn>
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="addRecordAndContinue()"
            :disabled="fetching"
        >添加并继续</v-btn>
    </div>
</template>