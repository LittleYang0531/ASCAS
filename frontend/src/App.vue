<script setup lang="ts">
import { useTheme } from 'vuetify';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { errorText, isError, showMsg, type } from './utils/message';
import { onBeforeMount, ref, type Ref } from 'vue';
import { newFetch } from './utils/fetch';
import { MessageType } from './models/message';
import NProgress from 'nprogress';
import { API_BASE_URL } from './config';
import type { User } from './models/user';

const theme = useTheme();
const loaded = ref(false);
const userInfo: Ref<User> = ref({});

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

    userInfo.value = res.item;
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
        <v-app v-if="loaded">
            <NavigationDrawer :user="userInfo"></NavigationDrawer>
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
    <Message :isError="isError" :errorText="errorText" :type="type"></Message>
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
a {
    color: green;
    text-decoration: none;
}
a:hover {
    text-decoration: underline;
}
</style>