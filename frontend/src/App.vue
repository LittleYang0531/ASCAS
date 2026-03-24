<script setup lang="ts">
import { useTheme } from 'vuetify';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { showMsg } from './utils/message';
import { onBeforeMount, ref, watch, type Ref } from 'vue';
import { newFetch } from './utils/fetch';
import { MessageType } from './models/message';
import NProgress from 'nprogress';
import { API_BASE_URL } from './config';
import type { User } from './models/user';
import { useRoute } from 'vue-router';
import AppBarHint from './components/AppBarHint.vue';
import { type as versionType } from './version'
import { userId } from './utils/user';

const theme = useTheme();
const route = useRoute();
const loaded = ref(false);
const userInfo: Ref<User> = ref({});
const showAppBar = ref(versionType == 'dev');

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
};
watch(() => route.name, () => {
	var title = "ASCAS | 田间形状采集辅助系统";
	document.title = route.name ? titles[route.name as keyof typeof titles] + ' - ' + title : title;
});

onBeforeMount(() => {
    var theme2 = localStorage.getItem("theme");
    if (theme2 == 'light' && theme.name.value != 'light') theme.toggle();
    else if (theme2 == 'dark' && theme.name.value != 'dark') theme.toggle();

    theme.themes.value.light!.colors.primary = "#4CAF50";
    theme.themes.value.dark!.colors.primary = "#4CAF50";
})
</script>

<template>
    <transition>
        <v-app v-if="loaded">
            <AppBarHint v-model:showAppBar="showAppBar"></AppBarHint>
            <NavigationDrawer :user="userInfo"></NavigationDrawer>
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
    <Message :hasAppBar="showAppBar && loaded"></Message>
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
</style>