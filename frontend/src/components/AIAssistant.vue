<script setup lang="ts">
import { onMounted, ref, type Ref } from 'vue';
import type { Message } from '../models/message';
import { userInfo } from '../utils/user';
import MessageText from './Message/Text.vue';
import { API_BASE_URL } from '../config';
import { newFetch } from '../utils/fetch';

const msg = ref('');
const messages: Ref<Message[]> = ref([]);
const loading = ref(false);

function constructAssistant(): Message {
    return {
        mid: 0,
        user: {
            uid: 0,
            name: 'AI 小助手',
            email: '',
            isAdmin: false,
        },
        message: '',
        createdAt: Math.round(new Date().getTime() / 1000),
    }
}

function jump(behavior: ScrollBehavior) {
    setTimeout(() => {
        var board = document.getElementById('messageBoard');
        board?.scrollTo({
            top: board.scrollHeight,
            behavior: behavior,
        });
    }, 10);
}

async function call() {
    messages.value.push({
        mid: messages.value.length + 1,
        user: userInfo.value,
        message: msg.value,
        createdAt: Math.round(new Date().getTime() / 1000),
    });
    msg.value = '';
    jump('smooth');
    var newmsg = constructAssistant();
    newmsg.mid = messages.value.length + 1;
    loading.value = true;

    var client = await newFetch(`${API_BASE_URL}/completion`, {
        method: 'POST',
        body: JSON.stringify({
            messages: [{
                role: 'system',
                content: '你是一个人工智能助手，协助用户解答问题和提供帮助。请友好、耐心地回答用户的问题，并尽可能提供有用的信息。如果你不确定答案，可以说你不知道，但不要编造信息。'
            }].concat(messages.value.map(m => ({
                role: m.user?.uid == 0 ? 'assistant' : 'user',
                content: m.message!,
            }))),
        }),
    });
    messages.value.push(newmsg);

    const reader = client.body?.getReader()!;
    const decoder = new TextDecoder();
    let buffer = '';

    while (true) {
        const { done, value } = await reader.read();
        if (done) break;
        buffer += decoder.decode(value, { stream: true });

        while (buffer.indexOf("\n\n") != -1) {
            const resultData = buffer.substring(0, buffer.indexOf("\n\n"));
            buffer = buffer.substring(buffer.indexOf("\n\n") + 2);

            if (!resultData.startsWith('data: ')) continue;
            const content = resultData.substring(6);
            if (content == '[DONE]') {
                reader.cancel();
                break;
            }

            var obj = JSON.parse(content);
            messages.value[messages.value.length - 1]!.message! += obj.choices[0].delta.content;
            jump('smooth');
        }
    }

    loading.value = false;
}

function onkeydown(event: KeyboardEvent) {
    if (event.key == 'Enter' && !event.shiftKey) {
        event.preventDefault();
        msg.value.length && call();
    }
}

onMounted(() => {
    var msg = constructAssistant();
    msg.mid = 1;
    msg.message = '你好！我是AI小助手，有什么可以帮你的吗？';
    messages.value.push(msg);
})
</script>

<template>
    <v-speed-dial location="top center" transition="fade-transition" :close-on-content-click="false">
        <template v-slot:activator="{ props: activatorProps }">
            <v-fab
                v-bind="activatorProps"
                size="large"
                icon="$mdiAssistant"
                class="AssistantButton"
                @click="jump('instant')"
            ></v-fab>
        </template>

        <v-card class="AssistantCard pa-5 pl-3 pr-3 d-flex flex-column">
            <v-card-title>AI小助手</v-card-title>
            <v-card-text class="d-flex flex-column AssistantCardContent">
                <span class="text-medium-emphasis">以下内容为AI生成，不代表开发者立场</span>
                <div class="AsssitantMessage flex-grow-1 mt-4 mb-4" id="messageBoard">
                    <MessageText 
                        v-for="message in messages"
                        :message="message"
                        :showTime="false"
                        :showTitle="false"
                        :overrideAvatar="message.user?.uid == 0 ? `${API_BASE_URL}/system/avatar/ai-assistant` : ''"
                    ></MessageText>
                </div>
                <v-textarea 
                    v-model="msg"
                    placeholder="输入消息..." 
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    auto-grow
                    rows="1"
                    class="flex-grow-0 TextArea"
                    @keydown="onkeydown"
                >
                    <template v-slot:append-inner>
                        <v-divider vertical class="mr-3 InnerDivider"></v-divider>
                        <v-icon
                            icon="$mdiSend"
                            color="primary"
                            :class="msg.length ? 'Clickable' : ''"
                            @click="msg.length && call()">
                        </v-icon>
                    </template>
                </v-textarea>
            </v-card-text>
        </v-card>
    </v-speed-dial>
</template>

<style lang="css" scoped>
.AssistantButton {
    position: fixed;
    right: 16px;
    bottom: 16px;
    z-index: 1000;
}

.AssistantCardContent {
    height: calc(100vh - 164px - 84px);
    max-height: calc(100vh - 164px - 84px);
    resize: both;
}

.AsssitantMessage {
    overflow-y: auto;
}

.AssistantCard {
    transition: opacity 0.28s;
    width: calc(max(min(512px, 100vw - 24px), 32vw));
    max-width: calc(max(min(512px, 100vw - 24px), 32vw));
    height: calc(100vh - 164px);
    max-height: calc(100vh - 164px);
}

.Clickable {
    opacity: 1;
}
</style>

<style lang="css">
.TextArea .v-field:not(.v-field--center-affix) .v-field__append-inner {
    align-items: flex-end;
    padding-bottom: 12px;
}

.TextArea .v-field--center-affix .InnerDivider {
    margin-top: 12px;
    margin-bottom: 12px;
}

.TextArea .v-icon {
    transition: opacity 0.28s;
}
</style>