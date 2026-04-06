<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent } from 'vue';
import { newFetch } from '../utils/fetch';
import { API_BASE_URL } from '../config';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';
import { sleep } from '../utils/sleep';
import { locate } from '../router';

async function load(to: any, from: any, next: any) {
    to; from; next;
    NProgress.start();
    NProgress.inc();

    var get = to.query;
    var url = "";
    if (get.type == "team") url = `${API_BASE_URL}/teams/${get.tid}/invite/accept?code=${get.code}`;
    else if (get.type == "crop") url = `${API_BASE_URL}/crops/${get.cid}/invite/accept?code=${get.code}`;
    else {
        showMsg(MessageType.Error, '邀请链接无效');
        NProgress.done();
        return;
    }

    var res = await newFetch(url, { method: 'POST' });
    if (res.status == 404) {
        showMsg(MessageType.Error, '邀请链接无效');
        NProgress.done();
        return;
    }
    showMsg(MessageType.Success, '接受邀请成功');
    await sleep(1000);
    if (get.type == "team") locate('/teams/' + get.tid);
    else if (get.type == "crop") locate('/crops/' + get.cid);
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<template>
</template>