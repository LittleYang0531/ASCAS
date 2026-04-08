<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { API_BASE_URL } from '../../config';
import type { Blog, Comment } from '../../models/blog';
import { newFetch } from '../../utils/fetch';
import Markdown from '../../components/Markdown.vue';
import { userId, userInfo } from '../../utils/user';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import type { InfiniteScrollStatus } from 'vuetify/lib/components/VInfiniteScroll/VInfiniteScroll.mjs';
import BlogComment from '../../components/Blog/Comment.vue';
import ImageLists from '../../components/Blog/ImageLists.vue';

async function load(to: any, from: any, next: any) {
    from;
    NProgress.start();
    NProgress.inc();

    var blog = await (await newFetch(`${API_BASE_URL}/blogs/${to.params.id}`)).json();

    next((e: any) => e.loading({
        data: blog.item,
        like: blog.like,
        dislike: blog.dislike,
        star: blog.star
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const item: Ref<Blog> = ref({});
const like = ref(false);
const dislike = ref(false);
const star = ref(false);

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

function loading(data: any) {
    item.value = data.data;
    like.value = data.like;
    dislike.value = data.dislike;
    star.value = data.star;
    loaded.value = true;
}

async function submitLike() {
    if (userId.value == item.value.author?.uid) {
        showMsg(MessageType.Error, '不能给自己点赞');
        return;
    }

    await newFetch(`${API_BASE_URL}/blogs/${item.value.bid}/like`, { method: 'POST' });
    if (dislike.value) dislike.value = false, item.value.dislikes!--;
    if (like.value) {
        like.value = false;
        item.value.likes!--;
    } else {
        like.value = true;
        item.value.likes!++;
    }

    showMsg(MessageType.Success, like.value ? '点赞成功' : '取消点赞成功');
}

async function submitDislike() {
    if (userId.value == item.value.author?.uid) {
        showMsg(MessageType.Error, '不能给自己点踩');
        return;
    }

    await newFetch(`${API_BASE_URL}/blogs/${item.value.bid}/dislike`, { method: 'POST' });
    if (like.value) like.value = false, item.value.likes!--;
    if (dislike.value) {
        dislike.value = false;
        item.value.dislikes!--;
    } else {
        dislike.value = true;
        item.value.dislikes!++;
    }

    showMsg(MessageType.Success, dislike.value ? '点踩成功' : '取消点踩成功');
}

async function submitStar() {
    if (userId.value == item.value.author?.uid) {
        showMsg(MessageType.Error, '不能给自己收藏');
        return;
    }

    await newFetch(`${API_BASE_URL}/blogs/${item.value.bid}/star`, { method: 'POST' });
    if (star.value) {
        star.value = false;
        item.value.stars!--;
    } else {
        star.value = true;
        item.value.stars!++;
    }

    showMsg(MessageType.Success, star.value ? '收藏成功' : '取消收藏成功');
}

const comments: Ref<Comment[]> = ref([]);
const comment = ref("");
const fetching = ref(false);
async function sendComment() {
    if (comment.value.trim() === "") {
        showMsg(MessageType.Error, '评论内容不能为空');
        return;
    }

    fetching.value = true;
    var res = await(await newFetch(`${API_BASE_URL}/blogs/${item.value.bid}/comments/send`, {
        method: 'POST',
        body: JSON.stringify({ msg: comment.value.trim() })
    }, () => { fetching.value = false; })).json();

    fetching.value = false;
    var id = res.id;
    comments.value.unshift({
        cid: id,
        comment: comment.value.trim(),
        author: userInfo.value,
        likes: 0,
        dislikes: 0,
        liked: false,
        disliked: false,
        createdAt: Math.floor(Date.now() / 1000)
    });
    comment.value = "";
    showMsg(MessageType.Success, '评论发送成功');
}

async function loadComments({ done }: { done: (status: InfiniteScrollStatus) => void }) {
    var url = `${API_BASE_URL}/blogs/${item.value.bid}/comments/list`;
    if (comments.value.length > 0) url += `?max=${comments.value[comments.value.length - 1]!.cid}`;
    var res = await (await newFetch(url)).json();
    for (var i = 0; i < res.items.length; i++) comments.value.push(res.items[i]);
    done('ok');
    if (res.items.length < 10) done('empty');
}

function reply(c: Comment) {
    var contents = comment.value.trim().split("\n");
    while (contents.length > 0 && contents[0]?.startsWith('> ')) contents.shift();
    contents = contents.join("\n").trim().split("\n");
    var pre = c.comment?.split("\n").map(line => `> ${line}`).join("\n");
    comment.value = pre + "\n\n" + contents.join("\n");
    location.href = "#comment-area";
}

defineExpose({ loading });
</script>

<template>
    <div v-if="loaded">
        <v-infinite-scroll side="end" @load="loadComments" empty-text="没有更多评论了">
            <h1>{{ item.title }}</h1>
            <div class="d-flex align-center ga-2 mb-4">
                <v-icon icon="$mdiAccount" size="x-small"></v-icon>
                <router-link 
                    :to="`/users/${item.author?.uid}`" 
                    class="text-body-small text-medium-emphasis"
                >{{ item.author?.name }}</router-link>
                <v-icon icon="$mdiComment" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ item.comments }}</span>
                <v-icon icon="$mdiClock" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ formatDate(item.createdAt!) }}</span>
            </div>
            <v-divider></v-divider>
            <div class="pa-2 full-width MarkdownContainer">
                <Markdown :text="item.content!"></Markdown>
            </div>
            <div class="mb-2" v-if="item.images?.length">
                <ImageLists :model="item.images!" :bid="item.bid!"></ImageLists>
            </div>
            <v-divider></v-divider>
            <div class="d-flex align-center justify-center pa-5">
                <div :class="`d-flex align-center hover ${like ? 'selected' : ''}`" @click="submitLike">
                    <v-icon
                        icon="$mdiThumbUp"
                        size="32"
                        class="clickable"
                    ></v-icon>
                    <span class="text-medium-emphasis ml-2">{{ item.likes }}</span>
                </div>
                <v-divider vertical class="ml-10"></v-divider>
                <div :class="`d-flex align-center ml-10 hover ${dislike ? 'selected' : ''}`" @click="submitDislike">
                    <v-icon
                        icon="$mdiThumbDown"
                        size="32"
                        class="clickable"
                    ></v-icon>
                </div>
                <v-divider vertical class="ml-10"></v-divider>
                <div :class="`d-flex align-center ml-10 hover ${star ? 'selected' : ''}`" @click="submitStar">
                    <v-icon
                        icon="$mdiStar"
                        size="32"
                        class="clickable"
                    ></v-icon>
                    <span class="text-medium-emphasis ml-2">{{ item.stars }}</span>
                </div>
            </div>
            <v-divider></v-divider>
            <h2 id="comment-area">评论区</h2>
            <div class="full-width MarkdownContainer">
                <div class="d-flex align-begin full-width ga-4">
                    <v-avatar size="48" :image="`${API_BASE_URL}/users/${userId}/avatar`"></v-avatar>
                    <v-textarea
                        v-model="comment"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        auto-grow
                        rows="3"
                        :placeholder="`请输入评论内容\n支持 Markdown 及 Latex 格式`"
                    ></v-textarea>
                </div>
                <div class="d-flex align-begin justify-end full-width mt-4">
                    <v-btn color="primary" prepend-icon="$mdiSend" @click="sendComment" :disabled="fetching">发表评论</v-btn>
                </div>
            </div>
            <div class="d-flex flex-column ga-4">
                <template v-for="(comment, index) in comments">
                    <v-divider v-if="index != 0"></v-divider>
                    <BlogComment :comment="comment" :bid="item.bid!" @reply="reply(comment)"></BlogComment>
                </template>
            </div>
        </v-infinite-scroll>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.hover {
    transition: color 0.28s;
    cursor: pointer;
}

.hover:hover {
    color: rgb(var(--v-theme-primary));
}

.selected {
    color: rgb(var(--v-theme-primary));
}

.MarkdownContainer {
    word-break: break-word;
    max-width: 100%;
}
</style>