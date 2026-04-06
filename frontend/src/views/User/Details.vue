<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { User } from '../../models/user';
import Markdown from '../../components/Markdown.vue';
import { userId } from '../../utils/user';
import { locate } from '../../router';

async function load(to: any, from: any, next: any) {
    from;

    NProgress.start();
    NProgress.inc();

    var data = await (await newFetch(`${API_BASE_URL}/users/${to.params.id}`)).json();

    next((e: any) => e.loading({
        data: data.item,
        description: data.description  
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const user: Ref<User> = ref({});
const description = ref("");

function loading(data: any) {
    user.value = data.data;
    description.value = data.description;
    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <div class="d-flex flex-column align-center justify-center full-width mt-6">
            <v-avatar size="192" class="Avatar cursor-pointer" :image="`${API_BASE_URL}/users/${user.uid}/avatar`"></v-avatar>
            <h1 class="mb-0">{{ user.name }}</h1>
            <span class="text-medium text-medium-emphasis mb-4">{{ user.email }}</span>
        </div>
        <v-divider></v-divider>
        <div class="d-flex align-center justify-space-between">
            <h2>个人简介</h2>
            <v-btn 
                color="primary" 
                prepend-icon="$mdiPencil" 
                v-if="userId == user.uid"
                @click="locate('/users/edit')"
            >编辑个人信息</v-btn>
        </div>
        <Markdown :text="description || '这个人很懒，什么都没有留下~'"></Markdown>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>