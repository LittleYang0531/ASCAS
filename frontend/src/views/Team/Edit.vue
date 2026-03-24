<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent } from 'vue';

async function load(to: any, from: any, next: any) {
    to; from;
    NProgress.start();
    NProgress.inc();

    var team = await (await newFetch(`${API_BASE_URL}/teams/${to.params.id}`)).json();

    next((e: any) => e.loading({
        team: team.item
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script setup lang="ts">
import { ref, type Ref } from 'vue';
import UserMultipleSelect from '../../components/User/MultipleSelect.vue';
import { mergeUsers, type User } from '../../models/user';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { sleep } from '../../utils/sleep';
import { userId } from '../../utils/user';

const loaded = ref(false);
const title = ref("");
const description = ref("");
const members: Ref<User[]> = ref([]);
const tid = ref(0);
const fetching = ref(false);

function loading(data: any) {
    title.value = data.team.title;
    description.value = data.team.description;
    members.value = data.team.members;
    tid.value = data.team.tid;
    loaded.value = true;
}

defineExpose({ loading });

async function submit() {
    if (title.value == "") {
        showMsg(MessageType.Error, "团队名称不能为空");
        return;
    }
    fetching.value = true;
    var members2 = mergeUsers(members.value, [{ uid: userId.value }]);
    await (await newFetch(`${API_BASE_URL}/teams/${tid.value}/edit`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            description: description.value,
            members: members2.map(m => m.uid)
        })
    }, () => { fetching.value = false; })).json();
    showMsg(MessageType.Success, "修改成功，正在跳转...");
    await sleep(1000);
    window.location.href = `/teams/${tid.value}`;
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
    <UserMultipleSelect
        v-model:users="members"
        label="团队成员"
        class="mt-4"
    ></UserMultipleSelect>
    <div class="mt-4 d-flex align-center justify-end">
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="submit()"
            :disabled="fetching"
        >修改</v-btn>
    </div>
</template>