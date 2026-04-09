<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue';
import { versionString } from '../version';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';
import { useTheme } from 'vuetify';
import { unread, userId } from '../utils/user';
import { sleep } from '../utils/sleep';
import LogoutDialog from './Dialog/LogoutDialog.vue';
import { fa } from 'vuetify/locale';

const showAppBar = defineModel<boolean>("showAppBar");
const props = defineProps<{
    title: string,
    transparent?: boolean
}>();
const emits = defineEmits<{
    (e: 'c'): void
}>();
function closeAppBar() {
    showAppBar.value = false;
    localStorage.setItem("showAppBar", "false");
}

const theme = useTheme();
function themeToggle() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

function setCookie(name: string, value: string, days: number) {
    var expires = "";
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        expires = "; expires=" + date.toUTCString();
    }
    document.cookie = name + "=" + value + expires + "; path=/";
}

const base_url = import.meta.env.BASE_URL;
const logoutDialog = ref(false);
async function logout() {
    setCookie("session", "", -1);
    showMsg(MessageType.Success, "退出登录成功");
    await sleep(1000);
    location.href = location.href;
}

const scrolled = ref(false);
function onScroll() {
    const { scrollTop, scrollHeight, clientHeight } = document.documentElement;
    scrolled.value = scrollTop > 0 || scrollHeight == clientHeight; // 特判打开对话框
}

onMounted(() => {
    var version = localStorage.getItem("appVersion");
    if (version == null) version = "";
    if (version != versionString) {
        showMsg(MessageType.Success, "已更新至版本 " + versionString);
        localStorage.removeItem("showAppBar");
        localStorage.setItem("appVersion", versionString);
    }

    if (!showAppBar.value) return;

    var show = localStorage.getItem("showAppBar");
    if (show == null) showAppBar.value = true;
    else showAppBar.value = false;

    window.onscroll = onScroll;
});

onUnmounted(() => {
    window.onscroll = () => {};
});
</script>

<template>
    <v-app-bar color="warning" height="64" v-if="showAppBar && !transparent">
        <div class="d-flex align-center justify-center ga-1" style="width: 100%;">
            <v-icon icon="$mdiProgressAlert" color="error"></v-icon>
            <span>该平台目前为预览版本，所产生的数据随时可能会被清除，仅供测试使用</span>
        </div>
        <template v-slot:append>
            <v-icon 
                icon="$mdiCloseCircle" 
                class="mr-2"
                @click="closeAppBar"
            ></v-icon>
        </template>
    </v-app-bar>
    <v-app-bar 
        :elevation="transparent && !scrolled ? 0 : 4" 
        :class="transparent && !scrolled ? 'MainBar HomeAppBar' : 'MainBar'"
        height="64" 
        v-else
    >
        <v-app-bar-nav-icon @click="emits('c')" v-if="!transparent"></v-app-bar-nav-icon>
        <v-app-bar-title v-if="!transparent">{{ props.title }}</v-app-bar-title>
        <div class="d-flex align-center ga-1 flex-grow-1 justify-begin" v-else>
            <v-img :src="`${base_url}favicon.ico`" :width="32" :height="32" class="flex-grow-0 ml-5 AppBarIcon"></v-img>
            <v-app-bar-title class="ml-5">{{ props.title }}</v-app-bar-title>
        </div>
        <template v-slot:append>
            <div class="d-flex align-center">
                <div class="d-flex align-center ga-10 mr-10">
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" to="/">
                        <v-icon icon="$mdiHome"></v-icon>
                        <p>网站主页</p>
                    </router-link>
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" to="/crops/list">
                        <v-icon icon="$mdiDatabase"></v-icon>
                        <p>我的数据</p>
                    </router-link>
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" to="/blogs/list">
                        <v-icon icon="$mdiBook"></v-icon>
                        <p>我的博客</p>
                    </router-link>
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" to="/messages/list">
                        <v-badge v-if="unread > 0" :content="unread" color="error" location="top right">
                            <v-icon icon="$mdiMessage"></v-icon>
                        </v-badge>
                        <v-icon v-else icon="$mdiMessage"></v-icon>
                        <p>我的消息</p>
                    </router-link>
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" to="/teams/list">
                        <v-icon icon="$mdiAccountGroup"></v-icon>
                        <p>我的团队</p>
                    </router-link>
                    <router-link class="d-flex align-center cursor-pointer ga-1 RouterLink" :to="'/users/' + userId">
                        <v-icon icon="$mdiAccount"></v-icon>
                        <p>个人空间</p>
                    </router-link>
                    <div class="d-flex align-center cursor-pointer ga-1 RouterLink" @click="logoutDialog = true;">
                        <v-icon icon="$mdiLogout"></v-icon>
                        <p>退出登录</p>
                    </div>
                </div>
                <v-btn
                    :icon="theme.name.value == 'light' ? '$mdiWeatherNight' : '$mdiWhiteBalanceSunny'"
                    @click="themeToggle()"
                ></v-btn>
            </div>
        </template>
    </v-app-bar>
    <LogoutDialog v-model:open="logoutDialog" @exit="logout"></LogoutDialog>
</template>

<style lang="css" scoped>
.AppBarIcon {
    position: relative;
    top: -3px;
}

.HomeAppBar {
    color: white!important;
    background: linear-gradient(#222F, #2220)!important;
}

.MainBar {
    color: white!important;
    background: rgb(var(--v-theme-primary));
    transition: background-color 0.28s;
}

.RouterLink {
    color: white;
    text-decoration: none;
}

@media (max-width: 1280px) {
    .RouterLink p {
        display: none;
    }
}
</style>