<script setup lang="ts">
import { API_BASE_URL } from '../../config';
import type { Comment } from '../../models/blog';
import { MessageType } from '../../models/message';
import { locate } from '../../router';
import { newFetch } from '../../utils/fetch';
import { showMsg } from '../../utils/message';
import { userId } from '../../utils/user';
import Markdown from '../Markdown.vue';

const props = defineProps<{
    comment: Comment,
    bid: number
}>();
const emits = defineEmits<{
    (e: 'reply'): void
}>();

function formatDate(date: number) {
    var now = new Date();
    var diff = now.getTime() / 1000.0 - date;
    if (diff < 60) {
        return '刚刚';
    } else if (diff < 60 * 60) {
        return Math.floor(diff / 60) + '分钟前';
    } else if (diff < 60 * 60 * 24) {
        return Math.floor(diff / 60 / 60) + '小时前';
    } else if (diff < 60 * 60 * 24 * 30) {
        return Math.floor(diff / 60 / 60 / 24) + '天前';
    } else {
        var dateObj = new Date(date * 1000);
        return dateObj.getFullYear().toString().padStart(4, '0') + '-' + 
               (dateObj.getMonth() + 1).toString().padStart(2, '0') + '-' + 
               dateObj.getDate().toString().padStart(2, '0') + ' ' + 
               dateObj.getHours().toString().padStart(2, '0') + ':' + 
               dateObj.getMinutes().toString().padStart(2, '0') + ':' + 
               dateObj.getSeconds().toString().padStart(2, '0');
    }
}

async function like() {
    // if (userId.value == props.comment.author?.uid) {
    //     showMsg(MessageType.Error, '不能给自己点赞');
    //     return;
    // }

    await newFetch(`${API_BASE_URL}/blogs/${props.bid}/comments/${props.comment.cid}/like`, { method: 'POST' });
    if (props.comment.disliked) props.comment.disliked = false, props.comment.dislikes!--;
    if (props.comment.liked) {
        props.comment.liked = false;
        props.comment.likes!--;
    } else {
        props.comment.liked = true;
        props.comment.likes!++;
    }

    showMsg(MessageType.Success, props.comment.liked ? '点赞成功' : '取消点赞成功');
}

async function dislike() {
    // if (userId.value == props.comment.author?.uid) {
    //     showMsg(MessageType.Error, '不能给自己点踩');
    //     return;
    // }

    await newFetch(`${API_BASE_URL}/blogs/${props.bid}/comments/${props.comment.cid}/dislike`, { method: 'POST' });
    if (props.comment.liked) props.comment.liked = false, props.comment.likes!--;
    if (props.comment.disliked) {
        props.comment.disliked = false;
        props.comment.dislikes!--;
    } else {
        props.comment.disliked = true;
        props.comment.dislikes!++;
    }

    showMsg(MessageType.Success, props.comment.disliked ? '点踩成功' : '取消点踩成功');
}
</script>

<template>
    <div class="d-flex align-begin full-width ga-4">
        <v-avatar 
            size="48" 
            :image="`${API_BASE_URL}/users/${props.comment.author?.uid}/avatar`"
            class="cursor-pointer"
            @click="locate(`/users/${props.comment.author?.uid}`)"
        ></v-avatar>
        <div class="d-flex flex-column">
            <router-link 
                class="font-weight-bold" 
                :to="`${API_BASE_URL}/users/${props.comment.author?.uid}/avatar`"
            >{{ props.comment.author?.name }}</router-link>
            <Markdown :text="props.comment.comment!" class="mt-1"></Markdown>
            <div class="ma-0 text-body-small text-medium-emphasis mt-2 d-flex align-center ga-1">
                <span>{{ formatDate(props.comment.createdAt!) }}</span>
                <div :class="`d-flex align-center ga-1 hover ${props.comment.liked ? 'clicked' : ''}`" @click="like">
                    <v-icon icon="$mdiThumbUp" size="x-small" class="ml-2"></v-icon>
                    <span>{{ props.comment.likes }}</span>
                </div>
                <div :class="`d-flex align-center ga-1 hover ${props.comment.disliked ? 'clicked' : ''}`" @click="dislike">
                    <v-icon icon="$mdiThumbDown" size="x-small" class="ml-2"></v-icon>
                </div>
                <div class="d-flex align-center ga-1 hover" @click="emits('reply')">
                    <v-icon icon="$mdiReply" size="x-small" class="ml-2"></v-icon>
                    <span>回复</span>
                </div>
            </div>
        </div>
    </div>
</template>

<style lang="css" scoped>
.hover {
    cursor: pointer;
    transition: color 0.28s;
}

.hover:hover {
    color: rgb(var(--v-theme-primary));
}

.clicked {
    color: rgb(var(--v-theme-primary));
}
</style>