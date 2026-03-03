<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    next((e: any) => e.loading({

    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);

function loading(data: any) {
    data;
    showMsg(MessageType.Success, "加载完成");
    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">

    </div>
</template>