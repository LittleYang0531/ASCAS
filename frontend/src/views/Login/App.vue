<script lang="ts" setup>
import NProgress from 'nprogress';
import { onMounted, onUnmounted, ref } from 'vue';
import { API_BASE_URL, login_background_urls } from '../../config';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import Message from '../../components/Message.vue';
import { sleep } from '../../utils/sleep';
import { useTheme } from 'vuetify';
import { newFetch } from '../../utils/fetch';
import AppLogin from './Pages/Login.vue';
import AppRegister from './Pages/Register.vue';
import AppRegisterSuccess from './Pages/RegisterSuccess.vue';
import AppResetRequestSuccess from './Pages/ResetRequestSuccess.vue';
import AppReset from './Pages/Reset.vue';
import AppResetVerify from './Pages/ResetVerify.vue';
import AppResetSuccess from './Pages/ResetSuccess.vue';
import AppVerify from './Pages/Verify.vue';

const loaded = ref(false);
const theme = useTheme();
const history = window.history;
const verified = ref(false);
const currpage = ref("login");

const titles = {
    "login": "登录",
    "register": "注册",
    "register_success": "注册成功",
    "reset": "重置密码",
    "reset_request_success": "重置邮件已发送",
    "reset_verify": "密码重置",
    "reset_success": "重置成功",
    "verify": "邮箱验证"
};
type Page = keyof typeof titles;

async function loading() {
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/check`)).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "已登录，正在跳转...");
        await sleep(1000);
        var url = "/";
        var params = new URLSearchParams(window.location.search);
        if (params.get("back") != null) url = params.get("back")!;
        window.location.href = url;
        return;
    }

    if (currpage.value == "verify") {
        var res = await (await newFetch(`${API_BASE_URL}/users/verify`, {
            method: "POST",
            body: JSON.stringify({
                code: new URLSearchParams(window.location.search).get("code")
            }),
        })).json();
        if (res.code == 200) verified.value = true;
        else verified.value = false;
    } else if (currpage.value == "reset_verify") {
        var res = await (await newFetch(`${API_BASE_URL}/users/reset`, {
            method: "POST",
            body: JSON.stringify({
                type: "verify",
                code: new URLSearchParams(window.location.search).get("code")
            })
        })).json();
        if (res.code == 200) verified.value = true;
        else verified.value = false;
    }

    loaded.value = true;
}
loading();

function toggleTheme() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

function switchPage(page: string) {
    currpage.value = page as Page;
    history.pushState(null, '', `${import.meta.env.BASE_URL}login?${page}`);
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";
}
window.onpopstate = function() {
    var page = new URLSearchParams(window.location.search).keys().next().value;
    if (page && titles[page as Page]) currpage.value = page;
    else currpage.value = "login";
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";
}

onMounted(() => {
    var theme2 = localStorage.getItem("theme");
    if (theme2 == 'light' && theme.name.value != 'light') theme.toggle();
    else if (theme2 == 'dark' && theme.name.value != 'dark') theme.toggle();

    var page = new URLSearchParams(window.location.search).keys().next().value;
    if (page && titles[page as Page]) currpage.value = page;
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";

    theme.themes.value.light!.colors.primary = "#4CAF50";
    theme.themes.value.dark!.colors.primary = "#4CAF50";
});
onUnmounted(() => {
    NProgress.done();
    window.onpopstate = null;
});
</script>

<template>
    <transition>
        <div v-if="loaded">
            <div class="login-container d-flex align-center justify-center">
                <v-carousel 
                    :show-arrows="false"
                    cycle 
                    height="100vh" 
                    class="login-carousel"
                    crossfade
                    hide-delimiters
                >
                    <v-carousel-item v-for="(url, i) in login_background_urls" :key="i">
                        <v-img :src="url" cover height="100vh"></v-img>
                    </v-carousel-item>
                </v-carousel>
                <div class="login-masking" :style="`background-color: rgba(0, 0, 0, ${theme.name.value == 'light' ? 0.2 : 0.6});`"></div>
                <v-card class="login-form" rounded="xl" elevation="24">
                    <div class="d-flex justify-space-between mb-6">
                        <v-icon
                            icon="$mdiChevronLeft"
                            size="small"
                            @click="history.back()"
                        ></v-icon>
                        <v-icon
                            :icon="theme.name.value == 'light' ? '$mdiWeatherNight' : '$mdiWhiteBalanceSunny'"
                            size="small"
                            @click="toggleTheme()"
                        ></v-icon>
                    </div>
                    <v-carousel v-model="currpage" hide-delimiters :show-arrows="false" height="auto" :touch="false">
                        <v-carousel-item value="login">
                            <AppLogin @switchPage="switchPage"></AppLogin>
                        </v-carousel-item>
                        <v-carousel-item value="register">
                            <AppRegister @switchPage="switchPage"></AppRegister>
                        </v-carousel-item>
                        <v-carousel-item value="register_success">
                            <AppRegisterSuccess></AppRegisterSuccess>
                        </v-carousel-item>
                        <v-carousel-item value="reset">
                            <AppReset @switchPage="switchPage"></AppReset>
                        </v-carousel-item>
                        <v-carousel-item value="reset_request_success">
                            <AppResetRequestSuccess></AppResetRequestSuccess>
                        </v-carousel-item>
                        <v-carousel-item value="reset_verify">
                            <AppResetVerify @switchPage="switchPage" :verified="verified"></AppResetVerify>
                        </v-carousel-item>
                        <v-carousel-item value="reset_success">
                            <AppResetSuccess @switchPage="switchPage"></AppResetSuccess>
                        </v-carousel-item>
                        <v-carousel-item value="verify">
                            <AppVerify @switchPage="switchPage" :verified="verified"></AppVerify>
                        </v-carousel-item>
                    </v-carousel>
                </v-card>
            </div>
        </div>
        <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
            <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
        </div>
    </transition>
    <Message></Message>
</template>

<style lang="css" scoped>
.login-container {
    width: 100%;
    height: 100vh;
}

.login-carousel {
    position: absolute;
}

.login-masking {
    width: 100%;
    height: 100vh;
    position: absolute;
    transition: background-color 0.28s;
}

.login-form {
    width: 448px;
    padding: 20px;
    opacity: 0.9;
}

.login-form:before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    background-color: #fff; /* 伪元素的背景色 */
    opacity: 1; /* 伪元素不透明 */
    z-index: -1; /* 将伪元素置于元素下方 */
}

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
}
.cursor-pointer {
    cursor: pointer;
}
</style>