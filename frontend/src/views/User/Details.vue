<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { User } from '../../models/user';

async function load(to: any, from: any, next: any) {
    from;

    NProgress.start();
    NProgress.inc();

    var data = await (await newFetch(`${API_BASE_URL}/users/${to.params.id}`)).json();

    next((e: any) => e.loading({
        data: data.item
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

function loading(data: any) {
    user.value = data.data;
    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1>{{ user.name }}</h1>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>