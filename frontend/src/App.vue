<script setup lang="ts">
import { useTheme } from 'vuetify';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { showMsg } from './utils/message';
import { onMounted, ref, watch, type Ref } from 'vue';
import { newFetch } from './utils/fetch';
import { MessageType } from './models/message';
import NProgress from 'nprogress';
import { API_BASE_URL } from './config';
import type { User } from './models/user';
import { useRoute } from 'vue-router';
import AppBarHint from './components/AppBarHint.vue';
import { type as versionType } from './version'
import { userId, userInfo as globalUserInfo } from './utils/user';
import AIAssistant from './components/AIAssistant.vue';

const theme = useTheme();
const route = useRoute();
const loaded = ref(false);
const userInfo: Ref<User> = ref({});
const showAppBar = ref(versionType == 'dev');
const pageTitle = ref("ASCAS");

async function loading() {
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/check`)).json();

    if (res.code == 401) {
        window.location.href = import.meta.env.BASE_URL + "login?back=" + encodeURIComponent(window.location.pathname);
        return;
    } else if (res.code != 200) {
        showMsg(MessageType.Error, "发生错误，请稍后再试");
        return;
    }

    userId.value = res.item.uid!;
    globalUserInfo.value = res.item;
    userInfo.value = res.item;
    loaded.value = true;
}

loading();

const titles = {
    "Home": "首页",
    "Index": "首页",
    "CropList": "作物表列表",
    "CropDetails": "作物表信息",
    "CropCreate": "新建作物表",
    "TeamList": "团队列表",
    "TeamDetails": "团队信息",
    "TeamCreate": "新建团队",
    "TeamEdit": "编辑团队",
    "MessageList": "消息列表",
    "Invite": "邀请链接",
    "UserDetails": "个人空间",
    "UserEdit": "编辑个人信息",
    "BlogList": "博客列表",
    "BlogCreate": "新建博客",
    "BlogDetails": "博客详情",
    "BlogEdit": "编辑博客",
    "SensorList": "传感器列表",
    "SensorCreate": "新建传感器",
    "SensorEdit": "编辑传感器",
    "SensorDetails": "传感器信息",
};
watch(() => route.name, () => {
	var title = "ASCAS | 田间形状采集辅助系统";
	document.title = route.name ? titles[route.name as keyof typeof titles] + ' - ' + title : title;
    pageTitle.value = route.name ? titles[route.name as keyof typeof titles] + ' | ASCAS' : "ASCAS";
});

onMounted(() => {
    var theme2 = localStorage.getItem("theme");
    if (theme2 == 'light' && theme.name.value != 'light') theme.toggle();
    else if (theme2 == 'dark' && theme.name.value != 'dark') theme.toggle();

    theme.themes.value.light!.colors.primary = "#4CAF50";
    theme.themes.value.dark!.colors.primary = "#4CAF50";
})

const drawer = ref(false);
</script>

<template>
    <transition>
        <v-app v-if="loaded">
            <AppBarHint 
                v-model:showAppBar="showAppBar" 
                :title="pageTitle" @c="drawer = !drawer" 
                :transparent="route.name == 'Home' || route.name == 'Index'"
            ></AppBarHint>
            <NavigationDrawer :user="userInfo" v-model:drawer="drawer"></NavigationDrawer>
            <AIAssistant></AIAssistant>
            <v-main class="Main">
                <router-view v-slot="{ Component }">
                    <component :is="Component"></component>
                </router-view>
            </v-main>
        </v-app>
        <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
            <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
        </div>
    </transition>
    <Message :top="!loaded ? 16 : 80"></Message>
</template>

<style lang="css" scoped>
.v-enter-active, .v-leave-active {
    transition: opacity 0.28s;
}
.v-enter-from, .v-leave-to {
    opacity: 0;
}
.v-enter-to, .v-leave-from {
    opacity: 1;
}

.Main {
    max-width: 960px;
    width: 100%;
    margin: auto;
    transition: none;
}
</style>

<style lang="css">
a {
    color: green;
    text-decoration: none;
}
a:hover {
    text-decoration: underline;
}

.mdi-spin {
  -webkit-animation: mdi-spin 2s infinite linear;
  animation: mdi-spin 2s infinite linear;
}

@-webkit-keyframes mdi-spin {
  0% {
    -webkit-transform: rotate(0deg);
    transform: rotate(0deg);
  }
  100% {
    -webkit-transform: rotate(359deg);
    transform: rotate(359deg);
  }
}

@keyframes mdi-spin {
  0% {
    -webkit-transform: rotate(0deg);
    transform: rotate(0deg);
  }
  100% {
    -webkit-transform: rotate(359deg);
    transform: rotate(359deg);
  }
}

.MyExpansionPanelTitle > .v-expansion-panel-title__overlay {
    opacity: 0!important;
}
.MyExpansionPanelText > .v-expansion-panel-text__wrapper {
    padding: 0;
}

.full-height {
    height: calc(100vh - 64px);
}

.full-width {
    width: 100%;
}
</style>