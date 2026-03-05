<script lang="ts" setup>
import NProgress from 'nprogress';
import { onBeforeMount, onBeforeUnmount, ref } from 'vue';
import { API_BASE_URL, login_background_urls } from '../../config';
import { errorText, isError, showMsg, type } from '../../utils/message';
import { MessageType } from '../../models/message';
import Message from '../../components/Message.vue';
import { sleep } from '../../utils/sleep';
import { useTheme } from 'vuetify';
import { myFetch } from '../../utils/fetch';
import crypto from 'crypto-js';

const loaded = ref(false);
const theme = useTheme();
const history = window.history;
const username = ref("");
const password = ref("");
const register_username = ref("");
const register_password = ref("");
const register_password_confirm = ref("");
const register_email = ref("");
const verify_success = ref(false);
const reset_email = ref("");
const reset_verified = ref(false);
const reset_password = ref("");
const reset_password_confirm = ref("");
const currpage = ref("login");

const fetching = ref(false);
const base_url = import.meta.env.BASE_URL

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

async function newFetch(url: string, options?: RequestInit): Promise<Response> {
    try {
        const response = await myFetch(url, options);
        return response;
    }
    catch (error) {
        NProgress.done();
        showMsg(MessageType.Error, "网络错误，请稍后再试");
        fetching.value = false;
        throw error;
    }
}

async function loading() {
    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/check`)).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Error, "已登录，正在跳转...");
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
        if (res.code == 200) verify_success.value = true;
        else verify_success.value = false;
    } else if (currpage.value == "reset_verify") {
        var res = await (await newFetch(`${API_BASE_URL}/users/reset`, {
            method: "POST",
            body: JSON.stringify({
                type: "verify",
                code: new URLSearchParams(window.location.search).get("code")
            })
        })).json();
        if (res.code == 200) reset_verified.value = true;
        else reset_verified.value = false;
    }

    loaded.value = true;
}

function toggleTheme() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

loading();

function setCookie(name: string, value: string, days: number) {
    var expires = "";
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        expires = "; expires=" + date.toUTCString();
    }
    document.cookie = name + "=" + value + expires + "; path=/";
}

async function login() {
    if (fetching.value) return;
    fetching.value = true;

    if (username.value == "" || password.value == "") {
        showMsg(MessageType.Error, "用户名和密码不能为空");
        fetching.value = false;
        return;
    }

    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/login`, {
        method: "POST",
        body: JSON.stringify({
            name: username.value,
            passwd: crypto.MD5(password.value).toString()
        }),
    })).json();
    NProgress.done();
    if (res.code == 200) {
        setCookie("session", res.session, res.expires);
        showMsg(MessageType.Success, "登录成功");
        await sleep(1000);
        var url = "/";
        var params = new URLSearchParams(window.location.search);
        if (params.get("back") != null) url = params.get("back")!;
        window.location.href = url;
    } else if (res.code == 401) {
        showMsg(MessageType.Error, "用户名或密码错误");
    } else if (res.code == 403) {
        showMsg(MessageType.Error, "账户未完成验证");
    } else {
        showMsg(MessageType.Error, "登录失败，请稍后再试");
    }

    fetching.value = false;
}

async function register() {
    if (fetching.value) return;
    fetching.value = true;

    if (register_username.value == "" || register_password.value == "" || register_email.value == "") {
        showMsg(MessageType.Error, "用户名、密码和邮箱不能为空");
        fetching.value = false;
        return;
    }
    if (register_password.value != register_password_confirm.value) {
        showMsg(MessageType.Error, "两次输入的密码不一致");
        fetching.value = false;
        return;
    }

    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/create`, {
        method: "POST",
        body: JSON.stringify({
            name: register_username.value,
            passwd: crypto.MD5(register_password.value).toString(),
            email: register_email.value
        }),
    })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "注册成功");
        switchPage("register_success");
    } else if (res.code == 409) {
        showMsg(MessageType.Error, "用户名或邮箱已存在");
    } else {
        showMsg(MessageType.Error, "注册失败，请稍后再试");
    }

    fetching.value = false;
}

async function reset() {
    if (fetching.value) return;
    fetching.value = true;

    if (reset_email.value == "") {
        showMsg(MessageType.Error, "邮箱不能为空");
        fetching.value = false;
        return;
    }

    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/reset`, {
        method: "POST",
        body: JSON.stringify({
            type: "request",
            email: reset_email.value
        }),
    })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "重置邮件已发送");
        switchPage("reset_request_success");
    } else if (res.code == 404) {
        showMsg(MessageType.Error, "该邮箱未注册");
    } else {
        showMsg(MessageType.Error, "重置失败，请稍后再试");
    }

    fetching.value = false;
}

async function resetPassword() {
    if (fetching.value) return;
    fetching.value = true;

    if (reset_password.value == "") {
        showMsg(MessageType.Error, "密码不能为空");
        fetching.value = false;
        return;
    }

    if (reset_password.value != reset_password_confirm.value) {
        showMsg(MessageType.Error, "两次输入的密码不一致");
        fetching.value = false;
        return;
    }

    NProgress.start();
    NProgress.inc();

    var res = await (await newFetch(`${API_BASE_URL}/users/reset`, {
        method: "POST",
        body: JSON.stringify({
            type: "reset",
            code: new URLSearchParams(window.location.search).get("code"),
            passwd: crypto.MD5(reset_password.value).toString()
        }),
    })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "密码重置成功");
        switchPage("reset_success");
    } else if (res.code == 400) {
        showMsg(MessageType.Error, "重置链接无效或已过期");
    } else {
        showMsg(MessageType.Error, "重置失败，请稍后再试");
    }

    fetching.value = false;
}

function switchPage(page: Page) {
    currpage.value = page;
    history.pushState(null, '', `${import.meta.env.BASE_URL}login?${page}`);
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";
}
window.onpopstate = function() {
    var page = new URLSearchParams(window.location.search).keys().next().value;
    if (page && titles[page as Page]) currpage.value = page;
    else currpage.value = "login";
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";
}
onkeyup = function(e) {
    if (e.key == "Enter") {
        if (currpage.value == "login") login();
        else if (currpage.value == "register") register();
        else if (currpage.value == "reset") reset();
        else if (currpage.value == "reset_verify" && reset_verified.value) resetPassword();
    }
}

onBeforeMount(() => {
    var theme2 = localStorage.getItem("theme");
    if (theme2 == 'light' && theme.name.value != 'light') theme.toggle();
    else if (theme2 == 'dark' && theme.name.value != 'dark') theme.toggle();

    var page = new URLSearchParams(window.location.search).keys().next().value;
    if (page && titles[page as Page]) currpage.value = page;
    document.title = titles[currpage.value as Page] + " - ASCAS | 田间形状采集辅助系统";

    theme.themes.value.light!.colors.primary = "#4CAF50";
    theme.themes.value.dark!.colors.primary = "#4CAF50";
});
onBeforeUnmount(() => {
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
                    <v-carousel v-model="currpage" hide-delimiters :show-arrows="false" height="auto">
                        <v-carousel-item value="login">
                            <div class="d-flex justify-center mb-4">
                                <v-img
                                    :src="`${base_url}favicon.ico`"
                                    width="64"
                                    height="64"
                                ></v-img>
                            </div>
                            <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">登录至 ASCAS</h2></div>
                            <div class="d-flex justify-center mb-4">
                                <v-text-field
                                    v-model="username"
                                    label="用户名/邮箱"
                                    variant="outlined"
                                    density="comfortable"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-10">
                                <v-text-field
                                    v-model="password"
                                    label="用户密码"
                                    variant="outlined"
                                    density="comfortable"
                                    type="password"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-4">
                                <v-btn
                                    width="100%"
                                    color="primary"
                                    @click="login()"
                                    :disabled="fetching"
                                >登录</v-btn>
                            </div>
                            <div class="d-flex justify-space-between">
                                <span>新用户？<a class="cursor-pointer" @click="switchPage('register')">点此注册</a></span>
                                <span><a class="cursor-pointer" @click="switchPage('reset')">忘记密码</a></span>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="register">
                            <div class="d-flex justify-center mb-4">
                                <v-img
                                    :src="`${base_url}favicon.ico`"
                                    width="64"
                                    height="64"
                                ></v-img>
                            </div>
                            <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">注册 ASCAS 账户</h2></div>
                            <div class="d-flex justify-center mb-4">
                                <v-text-field
                                    v-model="register_username"
                                    label="用户名"
                                    variant="outlined"
                                    density="comfortable"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-4">
                                <v-text-field
                                    v-model="register_email"
                                    label="电子邮箱"
                                    variant="outlined"
                                    density="comfortable"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-4">
                                <v-text-field
                                    v-model="register_password"
                                    label="用户密码"
                                    variant="outlined"
                                    density="comfortable"
                                    type="password"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-10">
                                <v-text-field
                                    v-model="register_password_confirm"
                                    label="确认密码"
                                    variant="outlined"
                                    density="comfortable"
                                    type="password"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-4">
                                <v-btn
                                    width="100%"
                                    color="primary"
                                    @click="register()"
                                    :disabled="fetching"
                                >注册</v-btn>
                            </div>
                            <div class="d-flex justify-end">
                                <span><a class="cursor-pointer" @click="switchPage('login')">返回登录</a></span>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="register_success">
                            <div class="d-flex justify-center mb-4">
                                <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
                            </div>
                            <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">注册成功</h2></div>
                            <div class="d-flex flex-column justify-center align-center mb-4">
                                <span>请检查您的邮箱以完成验证</span>
                                <span>验证链接有效期为 1 小时，请尽快完成邮箱验证！</span>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="reset">
                            <div class="d-flex justify-center mb-4">
                                <v-img
                                    :src="`${base_url}favicon.ico`"
                                    width="64"
                                    height="64"
                                ></v-img>
                            </div>
                            <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">重置密码</h2></div>
                            <div class="d-flex justify-center mb-4">
                                <v-text-field
                                    v-model="reset_email"
                                    label="电子邮箱"
                                    variant="outlined"
                                    density="comfortable"
                                    hide-details
                                ></v-text-field>
                            </div>
                            <div class="d-flex justify-center mb-4">
                                <v-btn
                                    width="100%"
                                    color="primary"
                                    @click="reset()"
                                    :disabled="fetching"
                                >发送重置邮件</v-btn>
                            </div>
                            <div class="d-flex justify-end">
                                <span><a class="cursor-pointer" @click="switchPage('login')">返回登录</a></span>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="reset_request_success">
                            <div class="d-flex justify-center mb-4">
                                <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
                            </div>
                            <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">重置邮件已发送</h2></div>
                            <div class="d-flex flex-column justify-center align-center mb-4">
                                <span>请检查您的邮箱以完成密码重置</span>
                                <span>重置链接有效期为 1 小时，请尽快完成密码重置！</span>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="reset_verify">
                            <div v-if="reset_verified == false">
                                <div class="d-flex justify-center mb-4">
                                    <v-icon icon="$mdiCloseCircle" size="64" color="red"></v-icon>
                                </div>
                                <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">重置链接无效</h2></div>
                                <div class="d-flex flex-column justify-center align-center mb-4">
                                    <span>重置链接无效或已过期</span>
                                    <span>请仔细检查您的邮箱，并使用正确的链接进行密码重置！</span>
                                </div>
                            </div>
                            <div v-else>
                                <div class="d-flex justify-center mb-4">
                                    <v-img
                                        :src="`${base_url}favicon.ico`"
                                        width="64"
                                        height="64"
                                    ></v-img>
                                </div>
                                <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">密码重置</h2></div>
                                <div class="d-flex justify-center mb-4">
                                    <v-text-field
                                        v-model="reset_password"
                                        label="新密码"
                                        variant="outlined"
                                        density="comfortable"
                                        type="password"
                                        hide-details
                                    ></v-text-field>
                                </div>
                                <div class="d-flex justify-center mb-10">
                                    <v-text-field
                                        v-model="reset_password_confirm"
                                        label="确认密码"
                                        variant="outlined"
                                        density="comfortable"
                                        type="password"
                                        hide-details
                                    ></v-text-field>
                                </div>
                                <div class="d-flex justify-center mb-4">
                                    <v-btn
                                        width="100%"
                                        color="primary"
                                        @click="resetPassword()"
                                        :disabled="fetching"
                                    >重置</v-btn>
                                </div>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="reset_success">
                            <div>
                                <div class="d-flex justify-center mb-4">
                                    <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
                                </div>
                                <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">密码重置成功</h2></div>
                                <div class="d-flex justify-center mb-4">
                                    <v-btn
                                        width="100%"
                                        color="primary"
                                        @click="switchPage('login')"
                                    >前往登录</v-btn>
                                </div>
                            </div>
                        </v-carousel-item>
                        <v-carousel-item value="verify">
                            <div v-if="verify_success">
                                <div class="d-flex justify-center mb-4">
                                    <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
                                </div>
                                <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">邮箱验证成功</h2></div>
                                <div class="d-flex justify-center mb-4">
                                    <v-btn
                                        width="100%"
                                        color="primary"
                                        @click="switchPage('login')"
                                    >前往登录</v-btn>
                                </div>
                            </div>
                            <div v-else>
                                <div class="d-flex justify-center mb-4">
                                    <v-icon icon="$mdiCloseCircle" size="64" color="red"></v-icon>
                                </div>
                                <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">邮箱验证失败</h2></div>
                                <div class="d-flex flex-column justify-center align-center mb-4">
                                    <span>验证链接无效或已过期</span>
                                    <span>请仔细检查您的邮箱，并使用正确的链接进行邮箱验证！</span>
                                </div>
                            </div>
                        </v-carousel-item>
                    </v-carousel>
                </v-card>
            </div>
        </div>
        <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
            <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
        </div>
    </transition>
    <Message :isError="isError" :errorText="errorText" :type="type"></Message>
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

.login-form-title {
    font-weight: 800;
    font-size: 24px;
}

a {
    color: green;
}

.cursor-pointer {
    cursor: pointer;
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