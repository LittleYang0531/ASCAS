<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { Crop } from '../../models/crop';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var crop = await (await newFetch(`${API_BASE_URL}/crops/${to.params.id}`)).json();

    next((e: any) => e.loading({
        crop: crop.item
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const item: Ref<Crop> = ref({});

function loading(data: any) {
    item.value = data.crop;
    
    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">

    </div>
</template>