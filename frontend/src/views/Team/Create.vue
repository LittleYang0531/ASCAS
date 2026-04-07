<script setup lang="ts">
import { ref } from 'vue';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { sleep } from '../../utils/sleep';
import { userId } from '../../utils/user';

const title = ref("");
const description = ref("");
const fetching = ref(false);

async function submit() {
    if (title.value == "") {
        showMsg(MessageType.Error, "团队名称不能为空");
        return;
    }
    fetching.value = true;
    var res = await (await newFetch(`${API_BASE_URL}/teams/create`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            description: description.value,
            members: [ userId.value ]
        })
    }, () => { fetching.value = false; })).json();
    var id = res["id"];
    showMsg(MessageType.Success, "创建成功，正在跳转...");
    await sleep(1000);
    window.location.href = `/teams/${id}`;
}
</script>

<template>
    <h1>新建团队</h1>
    <v-text-field
        v-model="title"
        variant="outlined"
        density="comfortable"
        hide-details
    >
        <template v-slot:label>
            <span>团队名称</span>  
            <span style="color: red">&nbsp;*</span>
        </template>
    </v-text-field>
    <v-textarea
        v-model="description"
        label="团队描述"
        variant="outlined"
        density="comfortable"
        hide-details
        class="mt-4"
        auto-grow
    ></v-textarea>
    <div class="mt-4 d-flex align-center justify-end mb-4">
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="submit()"
            :disabled="fetching"
        >新建</v-btn>
    </div>
</template>