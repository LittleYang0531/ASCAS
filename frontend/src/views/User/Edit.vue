<script lang="ts" setup>
import NProgress from 'nprogress';
import { ref, type Ref } from 'vue';
import type { User } from '../../models/user';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { userId } from '../../utils/user';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { sleep } from '../../utils/sleep';
import Markdown from '../../components/Markdown.vue';
import VOutlined from '../../components/VOutlined.vue';

const loaded = ref(false);
const fetching = ref(false);
const user: Ref<User> = ref({});
const title = ref("");
const email = ref("");
const description = ref("");
const t = ref(0);

async function loading() {
    NProgress.start();
    NProgress.inc();

    var data = await (await newFetch(`${API_BASE_URL}/users/${userId.value}`)).json();

    user.value = data.item;
    title.value = data.item.name;
    email.value = data.item.email;
    description.value = data.description;
    loaded.value = true;

    NProgress.done();
}

function onclick() {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = "image/*";
    input.onchange = function() {
        if (input.files && input.files.length > 0) {
            const file = input.files[0]!;
            const reader = new FileReader();
            reader.onload = async function(e) {
                var data = e.target?.result;
                var base64 = window.btoa(data as string);
                await newFetch(`${API_BASE_URL}/users/${userId.value}/avatar`, {
                    method: "POST",
                    body: base64
                });
                showMsg(MessageType.Success, "上传成功");
                t.value = Date.now();
            }
            reader.readAsBinaryString(file);
        }
    };
    input.click();
}

async function submit() {
    if (title.value == "") {
        showMsg(MessageType.Error, "团队名称不能为空");
        return;
    }
    fetching.value = true;

    await (await newFetch(`${API_BASE_URL}/users/edit`, {
        method: "POST",
        body: JSON.stringify({
            name: title.value,
            email: email.value,
            description: description.value
        })
    }, () => { fetching.value = false; })).json();

    fetching.value = false;
    showMsg(MessageType.Success, "修改成功，正在跳转...");
    await sleep(1000);
    window.location.href = `/users/${userId.value}`;
}

loading();
</script>

<template>
    <div v-if="loaded">
        <h1>修改个人信息</h1>
        <div class="d-flex align-center justify-center full-width mb-6">
            <v-avatar size="192" class="Avatar cursor-pointer" @click="onclick">
                <v-img :src="`${API_BASE_URL}/users/${userId}/avatar?t=${t}`"></v-img>
                <div class="v-overlay position-absolute full-width AvatarHovering d-flex align-center justify-center flex-column">
                    <v-icon size="48" color="white" icon="$mdiCamera"></v-icon>
                    <span class="mt-2" style="color: white">修改头像</span>
                </div>
            </v-avatar>
        </div>
        <v-text-field
            v-model="title"
            variant="outlined"
            density="comfortable"
            hide-details
            class="mt-4"
        >
            <template v-slot:label>
                <span>用户名称</span>  
                <span style="color: red">&nbsp;*</span>
            </template>
        </v-text-field>
        <v-text-field
            v-model="email"
            variant="outlined"
            density="comfortable"
            hide-details
            class="mt-4"
        >
            <template v-slot:label>
                <span>用户邮箱</span>  
                <span style="color: red">&nbsp;*</span>
            </template>
        </v-text-field>
        <div class="d-flex mt-4 ga-2 align-stretch">
            <v-textarea
                v-model="description"
                label="个人简介"
                variant="outlined"
                density="comfortable"
                hide-details
                auto-grow
                :placeholder="'支持 Markdown 及 Latex 格式'" 
            ></v-textarea>
            <v-divider vertical></v-divider>
            <div class="MarkdownPreview d-flex align-stretch">
                <VOutlined label="简介预览" hover>
                    <div class="pl-2 pr-2 pt-1 pb-1">
                        <Markdown :text="description || '这个人很懒，什么都没有留下~'"></Markdown>
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
            >修改</v-btn>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.AvatarHovering {
    opacity: 0;
    transition: opacity 0.28s, background-color 0.28s;
    height: 192px;
    background-color: rgba(0, 0, 0, 0);
}
.Avatar:hover .AvatarHovering {
    opacity: 1;
    background-color: rgba(0, 0, 0, var(--v-overlay-opacity));
}

.MarkdownPreview {
    width: calc(50% - 8.33px + 0.5px);
    word-break: break-word;
    max-width: calc(50% - 8.33px + 0.5px);
}
</style>