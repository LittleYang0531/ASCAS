<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { locate } from '../../router';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { Blog } from '../../models/blog';
import BlogCard from '../../components/Blog/Card.vue';

async function load(to: any, from: any, next: any) {
    from;
    NProgress.start();
    NProgress.inc();
    
    var keyword = new URLSearchParams(to.query).get("keyword") || "";
    var order = new URLSearchParams(to.query).get("order") || "";
    var blogs = await (await newFetch(`${API_BASE_URL}/blogs/list?keyword=${keyword}&order=${order}`)).json();

    next((e: any) => e.loading({
        data: blogs.items,
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
const items: Ref<Blog[]> = ref([]);
const keyword = ref("");
const showFilter = ref(false);
const order = ref("");
const filterStrings: Ref<string[]> = ref([]);

function loading(data: any) {
    items.value = data.data;
    keyword.value = data.keyword;
    order.value = data.order;
    filterStrings.value = [];
    if (keyword.value.trim() != "") {
        filterStrings.value.push(`关键词：${keyword.value}`);
    }
    if (order.value != "") {
        var orderString = "";
        switch (order.value) {
            case "BlogSortOrder::TITLE": orderString = "标题"; break;
            case "BlogSortOrder::CREATEDAT": orderString = "创建时间"; break
            case "BlogSortOrder::LIKES": orderString = "点赞数量"; break;
            case "BlogSortOrder::COMMENTS": orderString = "评论数量"; break;
            case "BlogSortOrder::STARS": orderString = "收藏数量"; break;
        }
        filterStrings.value.push(`排序方式：${orderString}`);
    }
    loaded.value = true;
}

function search() {
    locate('/blogs/list?keyword=' + keyword.value + '&order=' + order.value);
}

function clear() {
    keyword.value = "";
    order.value = "";
    search();
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1 class="mb-4">我的博客</h1>
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
                    value: 'BlogSortOrder::TITLE'
                }, { 
                    title: '创建时间',
                    value: 'BlogSortOrder::CREATEDAT'
                }, { 
                    title: '点赞数量',
                    value: 'BlogSortOrder::LIKES'
                }, {
                    title: '评论数量',
                    value: 'BlogSortOrder::COMMENTS'
                }, {
                    title: '收藏数量',
                    value: 'BlogSortOrder::STARS'
                }]"
            ></v-select>
            <v-btn color="primary" prepend-icon="$mdiMagnify" @click="search()">搜索</v-btn>
            <v-divider vertical></v-divider>
            <v-btn color="primary" prepend-icon="$mdiPlus" @click="locate('/blogs/create')">新建</v-btn>
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
        <div v-if="items.length > 0">
            <BlogCard v-for="blog in items" :blog="blog"></BlogCard>
        </div>
        <div v-else class="d-flex justify-center align-center flex-column" style="width: 100%; height: calc(100vh -  232px)">
            <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
            <p>没有找到符合条件的博客数据</p>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>