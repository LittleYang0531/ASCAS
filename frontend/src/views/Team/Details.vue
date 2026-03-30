<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { Team } from '../../models/team';
import { userId } from '../../utils/user';
import { mergeUsers } from '../../models/user';
import UserCard from '../../components/User/Card.vue';
import { locate } from '../../router';

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

<script lang="ts" setup>
const loaded = ref(false);
const item: Ref<Team> = ref({});

const icons = [ "$mdiAccount", "$mdiCrown" ];

function loading(data: any) {
    item.value = data.team;
    loaded.value = true;
}

function showEdit() {
    locate(`/teams/${item.value.tid}/edit`);
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <div class="d-flex align-center ga-4 mt-4 mb-4">
            <v-avatar :image="`${API_BASE_URL}/teams/${item.tid}/avatar`" size="64" rounded="0"></v-avatar>
            <div>
                <div class="d-flex align-center ga-2">
                    <h1 class="ma-0">{{ item.title }}</h1>
                    <v-icon :icon="icons[Number(userId == item.owner!.uid)]"></v-icon>
                    <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">
                        {{ userId == item.owner!.uid ? "OWNER" : "MEMBER" }}
                    </v-chip>
                </div>
                <p class="mt-0 text-medium-emphasis mb-2">{{ item.description }}</p>
            </div>
        </div>
        <v-divider></v-divider>
        <div class="d-flex align-center justify-space-between pa-4">
            <h2 class="ma-0">团队成员</h2>
            <div class="d-flex align-center ga-2">
                <v-btn prepend-icon="$mdiMessage" color="primary" @click="locate(`/messages/list?id=team-${item.tid}`)">进入群聊</v-btn>
                <v-btn prepend-icon="$mdiPencil" color="primary" @click="showEdit()" v-if="userId == item.owner!.uid">编辑团队</v-btn>
            </div>
        </div>
        <v-divider></v-divider>
        <v-list style="width: 100%">
            <UserCard v-for="user in mergeUsers(item.members!, [ item.owner! ])" :user="user" :active-click="true"></UserCard>
        </v-list>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>