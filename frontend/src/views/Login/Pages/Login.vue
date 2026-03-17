<script setup lang="ts">
import NProgress from 'nprogress';
import { onBeforeUnmount, ref } from 'vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import { API_BASE_URL } from '../../../config';
import crypto from 'crypto-js';
import { sleep } from '../../../utils/sleep';
import { newFetch } from '../../../utils/fetch';

const fetching = ref(false);
const base_url = import.meta.env.BASE_URL;
const username = ref("");
const password = ref("");
const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();

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
    }, () => { fetching.value = false; })).json();
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

onkeyup = function(e) {
    if (e.key == "Enter") login();
}

onBeforeUnmount(() => {
    onkeyup = null;
})
</script>

<template>
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
        <span>新用户？<a class="cursor-pointer" @click="emits('switchPage', 'register')">点此注册</a></span>
        <span><a class="cursor-pointer" @click="emits('switchPage', 'reset')">忘记密码</a></span>
    </div>
</template>