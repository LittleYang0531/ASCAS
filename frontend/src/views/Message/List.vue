<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, onMounted, onUnmounted, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL, WS_BASE_URL } from '../../config';
import type { Message, Talk } from '../../models/message';
import MessageOverview from '../../components/Message/Overview.vue';
import MessageText from '../../components/Message/Text.vue';
import type { InfiniteScrollStatus } from 'vuetify/lib/components/VInfiniteScroll/VInfiniteScroll.mjs';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var lists = await (await newFetch(`${API_BASE_URL}/messages/list`)).json();
    var id = to.query.id;
    var target = id ? lists.items.find((item: Talk) => item.talkId == id) : undefined;
    if (!target && id) {
        target = (await (await newFetch(`${API_BASE_URL}/messages/${id}/info`)).json()).item;
        lists.items.unshift(target);
    }

    next((e: any) => e.loading({
        lists: lists.items,
        id: id
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
const infiniteScroll = ref();
const messageArea = ref<HTMLDivElement>();
const textarea = ref<HTMLTextAreaElement>();
const height = ref(0);

function loading(data: any) {
    lists.value = data.lists;
    if (data.id) {
        var talk = lists.value.find(t => t.talkId == data.id)!;
        open(talk, false);
    }

    loaded.value = true;
}

var ws2: WebSocket;
async function open(talk: Talk, push = true) {
    if (ws2) {
        ws2.send("bye");
        ws2.close();
    }

    messages.value = [];
    currTalk.value = talk.talkId!;
    talkTitle.value = talk.title!;
    talk.unread = 0;
    empty.value = false;
    infiniteScroll.value?.reset();
    if (push) {
        console.log("push", talk.talkId);
        history.pushState(null, '', `${import.meta.env.BASE_URL}messages/list?id=${talk.talkId}`);
    }

    ws2 = new WebSocket(`${WS_BASE_URL}/messages/${currTalk.value}/websocket`);
    ws2.onmessage = (event) => {
        var isBottom = messageArea.value?.parentElement?.scrollHeight! - messageArea.value?.parentElement?.scrollTop! - messageArea.value?.parentElement?.clientHeight! < 10;
        var message = JSON.parse(event.data) as Message;
        messages.value.unshift(message);
        if (isBottom) {
            setTimeout(() => {
                messageArea.value?.parentElement?.scrollTo({ top: messageArea.value.parentElement.scrollHeight, behavior: "smooth" });
            }, 100);
        }
    }
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
    var talk = lists.value.find(t => t.talkId == currTalk.value)!;
    talk.latest = res.item;
    lists.value = [ talk, ...lists.value.filter(t => t.talkId != talk.talkId) ];
    setTimeout(() => {
        console.log(messageArea.value);
        messageArea.value?.parentElement?.scrollTo({ top: messageArea.value.parentElement.scrollHeight, behavior: "smooth" });
    }, 100);
}

defineExpose({ loading });

window.onpopstate = function() {
    var id = new URLSearchParams(window.location.search).get("id");
    console.log("pop", id);
    if (!id) {
        currTalk.value = "";
        talkTitle.value = "";
        messages.value = [];
    } else {
        var talk = lists.value.find(t => t.talkId == id)!;
        if (talk) open(talk, false);
    }
}

var ws: WebSocket;
onMounted(() => {
    height.value = window.innerHeight;
    window.onresize = () => {
        height.value = window.innerHeight;
    };

    ws = new WebSocket(`${WS_BASE_URL}/messages/list/websocket`);
    ws.onmessage = (event) => {
        var [ talkId, msg ] = event.data.split("\r\n");
        var talk = lists.value.find(t => t.talkId == talkId)!;
        var message = JSON.parse(msg) as Message;
        if (talk) {
            if (talk.talkId != currTalk.value) talk.unread! += 1;
            talk.latest = message;
            lists.value = [ talk, ...lists.value.filter(t => t.talkId != talk.talkId) ];
        }
    }
});
onUnmounted(() => {
    window.onresize = () => {};
    ws.send("bye");
    ws.close();
    if (ws2) {
        ws2.send("bye");
        ws2.close();
    }
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
                    <div class="TalkLists" v-if="lists.length">
                        <MessageOverview
                            v-for="item in lists"
                            :talk="item"
                            :currTalk="currTalk"
                            @click="open(item)"
                        ></MessageOverview>
                    </div>
                    <div class="TalkLists" v-else>
                        <div class="d-flex flex-column align-center justify-center" style="height: calc(100vh - 36.67px)">
                            <v-icon icon="$mdiProgressAlert" color="warning" size="36px"></v-icon>
                            <p class="ma-0 mt-1 ml-1">没有消息记录</p>
                        </div>
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
                                ref="infiniteScroll"
                                style="overflow-x: hidden;"
                            >
                                <div ref="messageArea">
                                    <MessageText
                                        v-for="(item, index) in messages.slice().reverse()"
                                        :message="item"
                                        :showTime="index == 0 || item.createdAt! - messages[messages.length - index]?.createdAt! > 300"
                                        :showTitle="!currTalk.startsWith('users')"
                                        :overrideAvatar="currTalk.startsWith('system-') ? `${API_BASE_URL}/system/avatar/${currTalk.substr(7)}` : undefined"
                                    ></MessageText>
                                </div>
                            </v-infinite-scroll>
                        </div>
                        <v-divider></v-divider>
                        <div class="Textarea full-width d-flex flex-column align-center justify-center pa-3 ga-2" v-if="!currTalk.startsWith('system-')">
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
                        <div class="Textarea full-width d-flex flex-column align-center justify-center pa-3 ga-2" v-else>
                            <div class="d-flex align-center justify-center full-width">
                                <p class="ma-0 mt-1 mb-1 text-body-medium text-medium-emphasis">系统消息，无法回复</p>
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
    max-width: calc(min(100vw, 1280px) - 306.67px);
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