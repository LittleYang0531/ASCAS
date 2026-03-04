<script lang="ts" setup>
import NProgress from 'nprogress';
import { ref, type Ref } from 'vue';
import { API_BASE_URL } from '../../config';
import type { Crop } from '../../models/crop';
import { newFetch } from '../../utils/fetch';

const loaded = ref(false);
const data: Ref<Array<Crop> > = ref([]);

async function loading() {
    NProgress.start();
    NProgress.inc();

    var crops = await (await newFetch(`${API_BASE_URL}/crops/list`)).json();
    data.value = crops.items;

    NProgress.done();

    loaded.value = true;
}

loading();
</script>

<template>
    <div v-if="loaded">
        
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>