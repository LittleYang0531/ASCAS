<script setup lang="ts">
import type { Message } from '../../models/message';
import { API_BASE_URL } from '../../config';
import Markdown from '../Markdown.vue';
import { userId } from '../../utils/user';

function formatTime(time: number) {
    var curr = new Date();
    var last = new Date(time * 1000);
    if (curr.getFullYear() == last.getFullYear()) { // 同一年
        if (curr.getMonth() == last.getMonth() && curr.getDay() == last.getDay()) { // 同一天
            return last.getHours().toString().padStart(2, '0') + ':' + 
                   last.getMinutes().toString().padStart(2, '0');
        } else {
            return (last.getMonth() + 1).toString().padStart(2, '0') + '-' + 
                   last.getDate().toString().padStart(2, '0') + ' ' + 
                   last.getHours().toString().padStart(2, '0') + ':' + 
                   last.getMinutes().toString().padStart(2, '0');
        }
    } else {
        return last.getFullYear().toString().padStart(4, '0') + '-' + 
               (last.getMonth() + 1).toString().padStart(2, '0') + '-' + 
               last.getDate().toString().padStart(2, '0') + ' ' + 
               last.getHours().toString().padStart(2, '0') + ':' + 
               last.getMinutes().toString().padStart(2, '0');
    }
}

const props = defineProps<{
    message: Message;
    showTime: boolean;
    showTitle: boolean;
}>();
</script>

<template>
    <div class="full-width">
        <div class="full-width d-flex align-center justify-center" v-if="props.showTime">
            <span>{{ formatTime(props.message.createdAt!) }}</span>
        </div>
        <div v-if="props.message.user?.uid != userId" class="full-width d-flex align-begin pa-4 ga-3">
            <div>
                <v-avatar
                    :image="`${API_BASE_URL}/users/${props.message.user?.uid}/avatar`"
                ></v-avatar>
            </div>
            <div>
                <span v-if="props.showTitle">{{ props.message.user?.name }}</span>
                <div class="OthersMessage d-flex align-center pa-2 bg-secondary">
                    <Markdown :text="props.message.message!"></Markdown>
                </div>
            </div>
        </div>
        <div v-else class="full-width d-flex align-begin justify-end pa-4 ga-3">
            <div>
                <span v-if="props.showTitle">{{ props.message.user?.name }}</span>
                <div class="MyMessage d-flex align-center pa-2 bg-primary">
                    <Markdown :text="props.message.message!"></Markdown>
                </div>
            </div>
            <div>
                <v-avatar
                    :image="`${API_BASE_URL}/users/${props.message.user?.uid}/avatar`"
                ></v-avatar>
            </div>
        </div>
    </div>
</template>

<style lang="css" scoped>
.OthersMessage {
    border-top-left-radius: 0px;
    border-top-right-radius: 10px;
    border-bottom-left-radius: 10px;
    border-bottom-right-radius: 10px;
}

.MyMessage {
    border-top-left-radius: 10px;
    border-top-right-radius: 0px;
    border-bottom-left-radius: 10px;
    border-bottom-right-radius: 10px;
}
</style>