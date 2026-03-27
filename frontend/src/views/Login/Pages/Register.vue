<script setup lang="ts">
import NProgress from 'nprogress';
import { onUnmounted, ref } from 'vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import { API_BASE_URL } from '../../../config';
import crypto from 'crypto-js';
import { newFetch } from '../../../utils/fetch';

const fetching = ref(false);
const base_url = import.meta.env.BASE_URL;
const register_username = ref("");
const register_password = ref("");
const register_password_confirm = ref("");
const register_email = ref("");
const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();

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
    }, () => { fetching.value = false; })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "注册成功");
        emits('switchPage', 'register_success');
    } else if (res.code == 409) {
        showMsg(MessageType.Error, "用户名或邮箱已存在");
    } else {
        showMsg(MessageType.Error, "注册失败，请稍后再试");
    }

    fetching.value = false;
}

onkeyup = function(e) {
    if (e.key == "Enter") register();
}

onUnmounted(() => {
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
        <span><a class="cursor-pointer" @click="emits('switchPage', 'login')">返回登录</a></span>
    </div>
</template>