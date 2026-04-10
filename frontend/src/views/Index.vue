<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { API_BASE_URL, index_background_urls } from '../config';
import { locate } from '../router';
import { newFetch } from '../utils/fetch';
import QuickRecordDialog from '../components/Dialog/QuickRecordDialog.vue';

async function load(to: any, from: any, next: any) {
    to; from;
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/overview`)).json();

    next((e: any) => e.loading({
        crop_count: res.crop_count,
        record_count: res.record_count,
        user_count: res.user_count,
        crops: res.crops
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const data: Ref<any> = ref({});
const quickRecordDialog = ref(false);

async function loading(val: any) {
    data.value = val;
    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded" class="IndexMain">
        <v-carousel 
            :show-arrows="false"
            cycle 
            height="100vh" 
            class="login-carousel"
            crossfade
            hide-delimiter-background
        >
            <v-carousel-item v-for="(url, i) in index_background_urls" :key="i">
                <v-img :src="url" cover height="100vh"></v-img>
            </v-carousel-item>
        </v-carousel>
        <div class="d-flex flex-column full-height">
            <div class="d-flex ga-15 pa-15" style="flex-grow: 2">
                <v-card class="d-flex flex-column align-center justify-center ga-4 flex-grow-1" @click="locate('/crops/create')">
                    <v-icon icon="$mdiTablePlus" size="64"></v-icon>
                    <h3 class="ma-0">新建作物表</h3>
                </v-card>
                <v-card class="d-flex flex-column align-center justify-center ga-4 flex-grow-1" @click="quickRecordDialog = true">
                    <v-icon icon="$mdiBookmarkPlusOutline" size="64"></v-icon>
                    <h3 class="ma-0">快速采集数据</h3>
                </v-card>
                <v-card class="d-flex flex-column align-center justify-center ga-4 flex-grow-1" @click="locate('/teams/list')">
                    <v-icon icon="$mdiAccountMultipleOutline" size="64"></v-icon>
                    <h3 class="ma-0">查看我的团队</h3>
                </v-card>
                <QuickRecordDialog v-model:open="quickRecordDialog" :crops="data.crops"></QuickRecordDialog>
            </div>
            <div class="d-flex ga-15 pa-15 pt-0 flex-grow-1">
                <v-card class="d-flex align-center justify-center ga-4 flex-grow-1">
                    <v-icon icon="$mdiDatabase" size="64"></v-icon>
                    <div>
                        <h3 class="ma-0">系统作物表总数</h3>
                        <h1 class="ma-0">{{ data.crop_count }}</h1>
                    </div>
                </v-card>
                <v-card class="d-flex align-center justify-center ga-4 flex-grow-1">
                    <v-icon icon="$mdiChartBar" size="64"></v-icon>
                    <div>
                        <h3 class="ma-0">今日采集数据量</h3>
                        <h1 class="ma-0">{{ data.record_count }}</h1>
                    </div>
                </v-card>
                <v-card class="d-flex align-center justify-center ga-4 flex-grow-1">
                    <v-icon icon="$mdiAccount" size="64"></v-icon>
                    <div>
                        <h3 class="ma-0">团队成员数</h3>
                        <h1 class="ma-0">{{ data.user_count }}</h1>
                    </div>
                </v-card>
            </div>
            <div class="d-flex ga-15 pa-15 pt-0 flex-grow-0">
                <v-card class="full-width pa-3">
                    <div class="d-flex align-center ga-5 justify-center full-width">
                        <v-btn
                            prepend-icon="$mdiPencil"
                            color="primary"
                            size="x-large"
                            rounded="xl"
                            @click="locate('/blogs/create')"
                        >写博客</v-btn>
                        <v-btn
                            prepend-icon="$mdiAlarmLight"
                            color="primary"
                            size="x-large"
                            rounded="xl"
                            @click="locate('/sensors/create')"
                        >添加传感器</v-btn>
                        <v-btn
                            prepend-icon="$mdiAccountMultiplePlus"
                            color="primary"
                            size="x-large"
                            rounded="xl"
                            @click="locate('/teams/create')"
                        >新建团队</v-btn>
                        <v-btn
                            prepend-icon="$mdiAccountCog"
                            color="primary"
                            size="x-large"
                            rounded="xl"
                            @click="locate('/users/edit')"
                        >个人设置</v-btn>
                    </div>
                </v-card>
            </div>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
</style>

<style lang="css">
.Main:has(.IndexMain) {
    max-width: 100%!important;
    --v-layout-top: 0px!important;
}
</style>