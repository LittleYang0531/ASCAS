<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();
    
    var keyword = new URLSearchParams(to.query).get("keyword") || "";
    var perm = new URLSearchParams(to.query).get("perm") || "";
    var order = new URLSearchParams(to.query).get("order") || "";
    var crops = await (await newFetch(`${API_BASE_URL}/crops/list?keyword=${keyword}&perm=${perm}&order=${order}`)).json();

    next((e: any) => e.loading({
        data: crops.items,
        keyword: keyword,
        perm: perm,
        order: order
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
import type { Ref } from 'vue';
import type { Crop } from '../../models/crop';
import CropCard from '../../components/Crops/Card.vue';
import { locate } from '../../router';

const perms = {
    "UserPermission::VIEWER": "VIEWER",
    "UserPermission::EDITOR": "EDITOR",
    "UserPermission::OWNER": "OWNER"
};
const orders = {
    "CropSortOrder::TITLE": "标题",
    "CropSortOrder::CREATEDAT": "创建时间",
    "CropSortOrder::UPDATEDAT": "最后更新"
};

const loaded = ref(false);
const data: Ref<Array<Crop> > = ref([]);
const keyword = ref("");
const perm = ref("");
const order = ref("");
const showFilter = ref(false);
const filterStrings: Ref<Array<string> > = ref([]);

async function loading(val: any) {
    data.value = val.data;
    keyword.value = val.keyword;
    perm.value = val.perm;
    order.value = val.order;

    showFilter.value = (perm.value != "" || order.value != "" || keyword.value != "");
    var filters = [];
    if (keyword.value != "") filters.push("关键词：" + keyword.value);
    if (perm.value != "") filters.push("权限：" + perms[perm.value as keyof typeof perms]);
    if (order.value != "") filters.push("排序：" + orders[order.value as keyof typeof orders]);
    filterStrings.value = filters;

    loaded.value = true;
}

defineExpose({ loading });

function search() {
    locate('/crops/list?keyword=' + keyword.value + '&perm=' + perm.value + '&order=' + order.value);
}

function clear() {
    keyword.value = "";
    perm.value = "";
    order.value = "";
    search();
    return;
}
</script>

<template>
    <div v-if="loaded" class="CropMain">
        <div class="d-flex align-center mt-4 ga-2">
            <v-text-field
                v-model="keyword"
                variant="outlined"
                density="compact"
                hide-details
                placeholder="搜索"
                clearable
            ></v-text-field>
            <v-select
                v-model="perm"
                class="flex-grow-0"
                variant="outlined"
                density="compact"
                label="权限"
                hide-details
                :items="[{
                    title: '全部',
                    value: ''
                }, {
                    title: 'VIEWER',
                    value: 'UserPermission::VIEWER'
                }, { 
                    title: 'EDITOR',
                    value: 'UserPermission::EDITOR'
                }, { 
                    title: 'OWNER',
                    value: 'UserPermission::OWNER'
                }]"
            ></v-select>
            <v-select
                v-model="order"
                class="flex-grow-0"
                variant="outlined"
                density="compact"
                label="排序方式"
                hide-details
                :items="[{
                    title: '默认',
                    value: ''
                }, {
                    title: '标题',
                    value: 'CropSortOrder::TITLE'
                }, { 
                    title: '创建时间',
                    value: 'CropSortOrder::CREATEDAT'
                }, { 
                    title: '最后更新',
                    value: 'CropSortOrder::UPDATEDAT'
                }]"
            ></v-select>
            <v-btn color="primary" prepend-icon="$mdiMagnify" @click="search">搜索</v-btn>
            <v-divider vertical></v-divider>
            <v-btn color="primary" prepend-icon="$mdiPlus">新建</v-btn>
        </div>
        <v-divider class="mt-4"></v-divider>
        <div v-if="showFilter">
            <div class="d-flex align-center justify-space-between pa-4">
                <p class="ma-0 d-flex align-center ga-2">
                    <v-icon icon="$mdiFilterMultiple"></v-icon>
                    <span v-for="(data, index) in filterStrings" class="d-flex align-center ga-2">
                        <v-divider vertical v-if="index != 0"></v-divider>
                        <span>{{ data }}</span>
                    </span>
                </p>
                <v-btn
                    color="primary"
                    prepend-icon="$mdiCloseCircle"
                    @click="clear()"
                >清除筛选器</v-btn>
            </div>
            <v-divider></v-divider>
        </div>
        <div v-if="data.length">
            <CropCard v-for="crop in data" :crop="crop"></CropCard>
        </div>
        <div v-else class="d-flex justify-center align-center flex-column" style="width: 100%; height: calc(100vh -  145.34px)">
            <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
            <p>没有找到符合条件的作物数据</p>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.CropMain {
    max-width: 960px;
    width: 100%;
    margin: auto;
}

button {
    height: 40px;
    padding: 0 20px;
}
</style>