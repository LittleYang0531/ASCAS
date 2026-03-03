<script setup lang="ts">
import { useTheme } from 'vuetify';
import AppBar from './components/AppBar.vue';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { errorText, isError, showMsg, type } from './utils/message';
import { onBeforeMount, ref } from 'vue';
import { myFetch } from './utils/fetch';
import { MessageType } from './models/message';
import NProgress from 'nprogress';
import { API_BASE_URL } from './config';

async function newFetch(url: string, options?: RequestInit): Promise<Response> {
    try {
        const response = await myFetch(url, options);
        return response;
    }
    catch (error) {
        NProgress.done();
        showMsg(MessageType.Error, "网络错误，请稍后再试");
        throw error;
    }
}

const theme = useTheme();
const loaded = ref(false);

async function loading() {
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/check`)).json();
    NProgress.done();

    if (res.code == 401) {
        window.location.href = "/login";
        return;
    } else if (res.code != 200) {
        showMsg(MessageType.Error, "发生错误，请稍后再试");
        return;
    }

    loaded.value = true;
}

loading();

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
        <v-app :theme="theme.name.value" v-if="loaded">
            <AppBar></AppBar>
            <NavigationDrawer></NavigationDrawer>
            <v-main>
                <router-view v-slot="{ Component }">
                    <component :is="Component"></component>
                </router-view>
            </v-main>
        </v-app>
        <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
            <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
        </div>
    </transition>
    <Message :isError="isError" :errorText="errorText" :type="type" :hasAppBar="loaded"></Message>
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
</style>

<style lang="css">
* {
    /* transition: background-color 5s, color 5s; */
}
</style>