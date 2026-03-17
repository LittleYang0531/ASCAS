<script setup lang="ts">
import NProgress from 'nprogress';
import { onBeforeUnmount, ref } from 'vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import { newFetch } from '../../../utils/fetch';
import { API_BASE_URL } from '../../../config';
import crypto from 'crypto-js';

const fetching = ref(false);
const base_url = import.meta.env.BASE_URL;
const verified = defineProps<{
    verified: boolean
}>();
const reset_password = ref("");
const reset_password_confirm = ref("");
const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();

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
    }, () => { fetching.value = false; })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "密码重置成功");
        emits('switchPage', 'reset_success');
    } else if (res.code == 400) {
        showMsg(MessageType.Error, "重置链接无效或已过期");
    } else {
        showMsg(MessageType.Error, "重置失败，请稍后再试");
    }

    fetching.value = false;
}

onkeyup = function(e) {
    if (e.key == "Enter") {
        if (verified.verified) resetPassword();
    }
}

onBeforeUnmount(() => {
    onkeyup = null;
})
</script>

<template>
    <div v-if="verified.verified == false">
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
</template>