<script setup lang="ts">
import NProgress from 'nprogress';
import { onUnmounted, ref } from 'vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import { API_BASE_URL } from '../../../config';
import { newFetch } from '../../../utils/fetch';

const fetching = ref(false);
const base_url = import.meta.env.BASE_URL;
const reset_email = ref("");
const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();

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
    }, () => { fetching.value = false; })).json();
    NProgress.done();
    if (res.code == 200) {
        showMsg(MessageType.Success, "重置邮件已发送");
        emits('switchPage', "reset_request_success");
    } else if (res.code == 404) {
        showMsg(MessageType.Error, "该邮箱未注册");
    } else {
        showMsg(MessageType.Error, "重置失败，请稍后再试");
    }

    fetching.value = false;
}

onkeyup = function(e) {
    if (e.key == "Enter") reset();
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
        <span><a class="cursor-pointer" @click="emits('switchPage', 'login')">返回登录</a></span>
    </div>
</template>