<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';

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

async function loading(val: any) {
    val;

    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">

    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>

</style>