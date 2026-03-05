<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';

async function load(to: any, from: any, next: any) {
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
const title = ref("");
const description = ref("");

function loading(data: any) {

    loaded.value = true;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1>新建作物表</h1>
        <v-timeline align="start" side="end" class="justify-start Timeline">
            <v-timeline-item icon="$mdiCog" dot-color="red-lighten-1">
                <div>
                    <h2 class="ma-0 font-weight-light mb-4">基本信息</h2>
                    <v-text-field
                        v-model="title"
                        label="作物名称"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                    ></v-text-field>
                    <v-text-field
                        v-model="description"
                        label="作物描述"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        class="mt-4"
                    ></v-text-field>
                </div>
            </v-timeline-item>
            <v-timeline-item icon="$mdiDatabase" dot-color="purple-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">数据列</h2>
            </v-timeline-item>
            <v-timeline-item icon="$mdiAccount" dot-color="green-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">成员权限</h2>
            </v-timeline-item>
        </v-timeline>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.Timeline {
    grid-template-columns: auto min-content 1fr;
}
</style>

<style lang="css">
.v-timeline-item__body {
    width: 100%;
}
</style>