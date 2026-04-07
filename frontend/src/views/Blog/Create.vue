<script setup lang="ts">
import { ref } from 'vue';
import VOutlined from '../../components/VOutlined.vue';
import Markdown from '../../components/Markdown.vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { sleep } from '../../utils/sleep';
import { locate } from '../../router';

const title = ref("");
const content = ref("");
const fetching = ref(false);

async function submit() {
    if (title.value.trim() == "") {
        alert("标题不能为空");
        return;
    }
    fetching.value = true;
    var res = await (await newFetch(`${API_BASE_URL}/blogs/create`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            content: content.value
        })
    }, () => { fetching.value = false; })).json();

    var id = res["id"];
    showMsg(MessageType.Success, "博客创建成功");
    await sleep(1000);
    locate(`/blogs/${id}`);
}
</script>

<template>
    <h1>新建博客</h1>
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
            rows="20"
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
    <div class="mt-4 d-flex align-center justify-end mb-4">
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="submit()"
            :disabled="fetching"
        >新建</v-btn>
    </div>
</template>

<style lang="css" scoped>
.MarkdownPreview {
    width: calc(50% - 8.33px + 0.5px);
    word-break: break-word;
    max-width: calc(50% - 8.33px + 0.5px);
}
</style>