<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { sleep } from '../../utils/sleep';
import { locate } from '../../router';
import VOutlined from '../../components/VOutlined.vue';
import Markdown from '../../components/Markdown.vue';
import MultipleImages from '../../components/Blog/MultipleImages.vue';

async function load(to: any, from: any, next: any) {
    from;
    NProgress.start();
    NProgress.inc();

    var blog = await (await newFetch(`${API_BASE_URL}/blogs/${to.params.id}`)).json();

    next((e: any) => e.loading({
        data: blog.item,
        bid: Number(to.params.id)
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
const content = ref("");
const images = ref<string[]>([]);
const fetching = ref(false);
const bid = ref(0);

function loading(data: any) {
    title.value = data.data.title;
    content.value = data.data.content;
    images.value = data.data.images;
    bid.value = data.bid;
    loaded.value = true;
}

async function submit() {
    if (title.value.trim() == "") {
        showMsg(MessageType.Error, "标题不能为空");
        return;
    }
    fetching.value = true;
    await (await newFetch(`${API_BASE_URL}/blogs/${bid.value}/edit`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            content: content.value,
            images: images.value
        })
    }, () => { fetching.value = false; })).json();

    showMsg(MessageType.Success, "博客修改成功");
    await sleep(1000);
    locate(`/blogs/${bid.value}`);
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <h1>修改博客</h1>
        <v-text-field
            v-model="title"
            variant="outlined"
            density="comfortable"
            hide-details
        >
            <template v-slot:label>
                <span>博客标题</span>  
                <span style="color: red">&nbsp;*</span>
            </template>
        </v-text-field>
        <div class="d-flex mt-4 ga-2 align-stretch">
            <v-textarea
                v-model="content"
                label="博客内容"
                variant="outlined"
                density="comfortable"
                hide-details
                auto-grow
                rows="15"
                :placeholder="'支持 Markdown 及 Latex 格式'" 
            ></v-textarea>
            <v-divider vertical></v-divider>
            <div class="MarkdownPreview d-flex align-stretch">
                <VOutlined label="博客预览" hover>
                    <div class="pl-2 pr-2 pt-1 pb-1">
                        <Markdown :text="content"></Markdown>
                    </div>
                </VOutlined>
            </div>
        </div>
        <MultipleImages v-model:model="images" :bid="bid" class="mt-4"></MultipleImages>
        <div class="mt-4 d-flex align-center justify-end mb-4">
            <v-btn
                prepend-icon="$mdiCheck"
                color="primary"
                @click="submit()"
                :disabled="fetching"
            >修改</v-btn>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.MarkdownPreview {
    width: calc(50% - 8.33px + 0.5px);
    word-break: break-word;
    max-width: calc(50% - 8.33px + 0.5px);
}
</style>