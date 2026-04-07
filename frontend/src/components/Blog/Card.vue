<script lang="ts" setup>
import { API_BASE_URL } from '../../config';
import type { Blog } from '../../models/blog';
import { MessageType } from '../../models/message';
import { locate } from '../../router';
import { newFetch } from '../../utils/fetch';
import { showMsg } from '../../utils/message';
import { sleep } from '../../utils/sleep';
import { userId } from '../../utils/user';

const props = defineProps<{
    blog: Blog
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

async function remove() {
    if (!confirm('确定要删除吗？')) return;
    await (await newFetch(`${API_BASE_URL}/blogs/${props.blog.bid}/remove`, {
        method: 'POST'
    })).json();
    showMsg(MessageType.Success, '删除成功');
    await sleep(1000);
    location.href = location.href;
}
</script>

<template>
    <div class="pa-4 d-flex full-width">
        <div class="flex-grow-1" :style="`width: calc(100% - ${props.blog.author?.uid == userId ? '82.3' : '0'}px);`">
            <div class="d-flex align-center ga-2">
                <router-link :to="`/blogs/${props.blog.bid}`" class="text-title-large">{{ props.blog.title }}</router-link>
            </div>
            <p class="text-body-small text-medium-emphasis text-truncate">{{ props.blog.content }}</p>
            <div class="d-flex align-center ga-1">
                <v-icon icon="$mdiAccount" size="x-small"></v-icon>
                <router-link 
                    :to="`/users/${props.blog.author?.uid}`" 
                    class="text-body-small text-medium-emphasis"
                >{{ props.blog.author?.name }}</router-link>
                <v-icon icon="$mdiThumbUp" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ props.blog.likes }}</span>
                <v-icon icon="$mdiStar" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ props.blog.stars }}</span>
                <v-icon icon="$mdiComment" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ props.blog.comments }}</span>
                <v-icon icon="$mdiClock" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ formatDate(props.blog.createdAt!) }}</span>
            </div>
        </div>
        <div class="flex-grow-0 d-flex flex-column ga-2">
            <v-btn
                color="primary"
                prepend-icon="$mdiPencil"
                @click="locate(`/blogs/${props.blog.bid}/edit`)"
                v-if="props.blog.author?.uid == userId"
            >编辑</v-btn>
            <v-btn
                color="error"
                prepend-icon="$mdiTrashCan"
                @click="remove"
                v-if="props.blog.author?.uid == userId"
            >删除</v-btn>
        </div>
    </div>
    <v-divider></v-divider>
</template>