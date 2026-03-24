<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { Team } from '../../models/team';
import { locate } from '../../router';
import TeamCard from '../../components/Team/Card.vue';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var keyword = new URLSearchParams(to.query).get("keyword") || "";
    var order = new URLSearchParams(to.query).get("order") || "";
    var teams = await (await newFetch(`${API_BASE_URL}/teams/list?keyword=${keyword}&order=${order}`)).json();

    next((e: any) => e.loading({
        data: teams.items,
        keyword: keyword,
        order: order
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const data: Ref<Array<Team> > = ref([]);
const keyword = ref("");
const order = ref("");
const showFilter = ref(false);
const filterStrings: Ref<Array<string> > = ref([]);

const orders = {
    "TeamSortOrder::TITLE": "标题",
    "TeamSortOrder::CREATEDAT": "创建时间",
    "TeamSortOrder::UPDATEDAT": "最后更新"
};

function loading(val: any) {
    data.value = val.data;
    keyword.value = val.keyword;
    order.value = val.order;

    showFilter.value = (order.value != "" || keyword.value != "");
    var filters = [];
    if (keyword.value != "") filters.push("关键词：" + keyword.value);
    if (order.value != "") filters.push("排序：" + orders[order.value as keyof typeof orders]);
    filterStrings.value = filters;

    loaded.value = true;
}

function search() {
    locate('/teams/list?keyword=' + keyword.value + '&order=' + order.value);
}

function clear() {
    keyword.value = "";
    order.value = "";
    search();
    return;
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1 class="mb-4">我的团队</h1>
        <v-divider></v-divider>
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
                    value: 'TeamSortOrder::TITLE'
                }, { 
                    title: '创建时间',
                    value: 'TeamSortOrder::CREATEDAT'
                }]"
            ></v-select>
            <v-btn color="primary" prepend-icon="$mdiMagnify" @click="search">搜索</v-btn>
            <v-divider vertical></v-divider>
            <v-btn color="primary" prepend-icon="$mdiPlus" @click="locate('/teams/create')">新建</v-btn>
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
            <TeamCard v-for="team in data" :team="team"></TeamCard>
        </div>
        <div v-else class="d-flex justify-center align-center flex-column" style="width: 100%; height: calc(100vh -  145.34px)">
            <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
            <p>没有找到符合条件的团队数据</p>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>