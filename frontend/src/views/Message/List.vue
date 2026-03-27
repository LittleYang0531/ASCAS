<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, onMounted, onUnmounted, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import type { Message, Talk } from '../../models/message';
import MessageOverview from '../../components/Message/Overview.vue';
import MessageText from '../../components/Message/Text.vue';
import type { InfiniteScrollStatus } from 'vuetify/lib/components/VInfiniteScroll/VInfiniteScroll.mjs';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var lists = await (await newFetch(`${API_BASE_URL}/messages/list`)).json();

    next((e: any) => e.loading({
        lists: lists.items,
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const lists: Ref<Talk[]> = ref([]);
const currTalk = ref("");
const talkTitle = ref("");
const messages: Ref<Message[]> = ref([]);
const empty = ref(false);
const fetching = ref(false);
const messageArea = ref<HTMLDivElement>();
const textarea = ref<HTMLTextAreaElement>();
const height = ref(0);

function loading(data: any) {
    lists.value = data.lists;

    loaded.value = true;
}

async function open(talk: Talk) {
    messages.value = [];
    currTalk.value = talk.talkId!;
    talkTitle.value = talk.title!;
    talk.unread = 0;
    empty.value = false;
}

async function loadMessages({ done }: { done: (status: InfiniteScrollStatus) => void }) {
    if (empty.value) { done("ok"); done("empty") };
    var url = `${API_BASE_URL}/messages/${currTalk.value}`;
    if (messages.value.length != 0) url += "?max=" + (messages.value[messages.value.length - 1]?.mid! - 1);
    var res = await (await newFetch(url)).json();
    for (var i = 0; i < res.items.length; i++) messages.value.push(res.items[i]);
    if (res.items.length < 20) {
        empty.value = true;
        done("ok");
        done("empty");
        return;
    }
    done("ok");
}

function onkeydown(e: KeyboardEvent) {
    if (e.key == "Enter" && e.shiftKey) {
        sendMessage();
    }
}

async function sendMessage() {
    if (textarea.value?.value.trim() == "") return;
    fetching.value = true;
    var res = await (await newFetch(`${API_BASE_URL}/messages/send`, {
        method: "POST",
        body: JSON.stringify({
            id: currTalk.value,
            message: textarea.value?.value,
        }),
    }, () => { fetching.value = false; })).json();
    textarea.value!.value = "";
    messages.value.unshift(res.item);
    fetching.value = false;
    lists.value.find(t => t.talkId == currTalk.value)!.latest = res.item;
    setTimeout(() => {
        console.log(messageArea.value);
        messageArea.value?.parentElement?.scrollTo({ top: messageArea.value.parentElement.scrollHeight, behavior: "smooth" });
    }, 100);
}

defineExpose({ loading });

onMounted(() => {
    height.value = window.innerHeight;
    window.onresize = () => {
        height.value = window.innerHeight;
    };
});
onUnmounted(() => {
    window.onresize = () => {};
});
</script>

<template>
    <div v-if="loaded" class="SpecialMain">
        <div class="d-flex align-center justify-center">
            <v-divider vertical></v-divider>
            <div class="full-height TalkList">
                <v-list class="full-height full-width pb-0">
                    <div class="d-flex align-center justify-center">
                        <p class="ma-0 mb-2 text-body-medium text-medium-emphasis">最近消息</p>
                    </div>
                    <v-divider></v-divider>
                    <div class="TalkLists">
                        <MessageOverview
                            v-for="item in lists"
                            :talk="item"
                            :currTalk="currTalk"
                            @click="open(item)"
                        ></MessageOverview>
                    </div>
                </v-list>
            </div>
            <v-divider vertical></v-divider>
            <div class="full-height flex-grow-1">
                <v-card class="full-height full-width" elevation="0" rounded="0" v-if="currTalk != ''">
                    <div class="d-flex align-center justify-center">
                        <p class="ma-0 mt-2 mb-2 text-body-medium">{{ talkTitle }}</p>
                    </div>
                    <v-divider></v-divider>
                    <div class="full-height d-flex flex-column">
                        <div class="flex-grow-1 full-width Messages">
                            <v-infinite-scroll
                                :height="height - 236.67"
                                side="start" 
                                @load="loadMessages"
                                empty-text="没有更多消息啦"
                            >
                                <div ref="messageArea">
                                    <MessageText
                                        v-for="(item, index) in messages.slice().reverse()"
                                        :message="item"
                                        :showTime="index == 0 || item.createdAt! - messages[messages.length - index]?.createdAt! > 300"
                                        :showTitle="!currTalk.startsWith('users')"
                                    ></MessageText>
                                </div>
                            </v-infinite-scroll>
                        </div>
                        <v-divider></v-divider>
                        <div class="Textarea full-width d-flex flex-column align-center justify-center pa-3 ga-2">
                            <textarea 
                                :placeholder="'输入消息...\n支持 Markdown 及 Latex 格式\n按 Enter 键换行，按 Shift+Enter 键发送'" 
                                class="flex-grow-1" 
                                ref="textarea"
                                @keydown="onkeydown"
                            ></textarea>
                            <div class="d-flex align-center justify-end full-width">
                                <v-btn 
                                    color="primary" 
                                    prepend-icon="$mdiSend" 
                                    @click="sendMessage()" 
                                    :disabled="fetching"
                                >发送</v-btn>
                            </div>
                        </div>
                    </div>
                </v-card>
                <v-card class="full-height full-width d-flex align-center justify-center flex-column" elevation="0" rounded="0" v-else>
                    <v-icon icon="$mdiProgressAlert" color="warning" size="64px"></v-icon>
                    <p>点击一个聊天开始聊天</p>
                </v-card>
            </div>
            <v-divider vertical></v-divider>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.TalkList {
    width: 250px;
}

.TalkLists {
    max-height: calc(100vh - 36.67px);
    overflow-y: auto;
}

.Messages {
    height: calc(100vh - 236.67px);
    max-height: calc(100vh - 236.67px);
    overflow-y: auto;
}

.Textarea {
    height: 200px;
}

.Textarea textarea {
    width: 100%;
    border: none;
    outline: none;
    resize: none;
    background-color: rgb(var(--v-theme-surface));
    transition: background-color 0.28s;
}
</style>

<style lang="css">
.Main:has(.SpecialMain) {
    max-width: 1280px!important;
}
</style>