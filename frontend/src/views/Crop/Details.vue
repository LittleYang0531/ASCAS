<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { UserPermission_icons, type Crop } from '../../models/crop';
import DetailsProperties from './DetailsPages/Properties.vue';
import DetailsListRecord from './DetailsPages/ListRecord.vue';
import DetailsAddRecord from './DetailsPages/AddRecord.vue';
import DetailsEditors from './DetailsPages/Editors.vue';
import DetailsViewers from './DetailsPages/Viewers.vue';
import DetailsEdit from './DetailsPages/Edit.vue';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var crop = await (await newFetch(`${API_BASE_URL}/crops/${to.params.id}`)).json();

    next((e: any) => e.loading({
        crop: crop.item,
        param: to.query
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
const tab = ref("properties");

function loading(data: any) {
    item.value = data.crop;
    if (data.param.page != undefined) tab.value = data.param.page;
    loaded.value = true;
}

const titles = {
    "properties": "属性列表",
    "simple": "数据查询",
    "add": "添加数据",
    "editors": "协作编辑",
    "viewers": "协作查看",
    "edit": "作物表编辑"
};
type Page = keyof typeof titles;

window.onpopstate = function() {
    var page = new URLSearchParams(window.location.search).get("page");
    console.log("popstate", page);
    if (page && titles[page as Page]) tab.value = page;
    else tab.value = "login";
    document.title = item.value.title + " - " + titles[tab.value as Page] + " - ASCAS | 田间性状采集辅助系统";
}
function updatePage(newval: string) {
    history.pushState(null, '', `${import.meta.env.BASE_URL}crops/${item.value.cid}?page=${newval}`);
    document.title = item.value.title + " - " + titles[newval as Page] + " - ASCAS | 田间性状采集辅助系统";
};

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <div class="d-flex align-center ga-2">
            <h1>{{ item.title }}</h1>
            <v-icon :icon="UserPermission_icons[item.permission as keyof typeof UserPermission_icons] || '$mdiLock'"></v-icon>
            <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">id：{{ item.name }}</v-chip>
            <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">
                {{ item.permission?.substring("UserPermission::".length) }}
            </v-chip>
        </div>
        <p class="mt-0 text-medium-emphasis">{{ item.description }}</p>
        <v-divider></v-divider>

        <v-tabs v-model="tab" color="primary" @update:model-value="updatePage">
            <v-tab value="properties">属性列表</v-tab>
            <v-tab value="simple">数据查询</v-tab>
            <v-tab value="add" v-if="item.permission != 'UserPermission::VIEWER'">添加数据</v-tab>
            <v-tab value="editors">协作编辑</v-tab>
            <v-tab value="viewers">协作查看</v-tab>
            <v-tab value="edit" v-if="item.permission == 'UserPermission::OWNER'">作物表编辑</v-tab>
        </v-tabs>
        <v-divider></v-divider>

        <v-tabs-window v-model="tab">
            <!-- 属性列表 -->
            <v-tabs-window-item value="properties">
                <DetailsProperties :item="item"></DetailsProperties>
            </v-tabs-window-item>
            <!-- 数据查询 -->
            <v-tabs-window-item value="simple">
                <DetailsListRecord :crop="item"></DetailsListRecord>
            </v-tabs-window-item>
            <!-- 添加数据 -->
            <v-tabs-window-item value="add" class="pa-4" v-if="item.permission != 'UserPermission::VIEWER'">
                <DetailsAddRecord :item="item"></DetailsAddRecord>
            </v-tabs-window-item>
            <!-- 协作编辑 -->
            <v-tabs-window-item value="editors">
                <DetailsEditors :item="item"></DetailsEditors>
            </v-tabs-window-item>
            <!-- 协作查看 -->
            <v-tabs-window-item value="viewers">
                <DetailsViewers :item="item"></DetailsViewers>
            </v-tabs-window-item>
            <!-- 作物表编辑 -->
            <v-tabs-window-item value="edit" class="pa-4" v-if="item.permission == 'UserPermission::OWNER'">
                <DetailsEdit :item="item"></DetailsEdit>
            </v-tabs-window-item>
        </v-tabs-window>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>