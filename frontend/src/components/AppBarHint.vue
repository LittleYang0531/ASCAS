<script setup lang="ts">
import { onBeforeMount } from 'vue';
import { versionString } from '../version';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';

const showAppBar = defineModel<boolean>("showAppBar");
function closeAppBar() {
    showAppBar.value = false;
    localStorage.setItem("showAppBar", "false");
}

onBeforeMount(() => {
    var version = localStorage.getItem("appVersion");
    if (version == null) version = "";
    if (version != versionString) {
        showMsg(MessageType.Success, "已更新至版本 " + versionString);
        localStorage.removeItem("showAppBar");
        localStorage.setItem("appVersion", versionString);
    }

    if (!showAppBar.value) return;

    var show = localStorage.getItem("showAppBar");
    if (show == null) showAppBar.value = true;
    else showAppBar.value = false;
});
</script>

<template>
    <v-app-bar color="warning" height="48" v-if="showAppBar">
        <div class="d-flex align-center justify-center ga-1" style="width: 100%;">
            <v-icon icon="$mdiProgressAlert" color="error"></v-icon>
            <span>该平台目前为预览版本，所产生的数据随时可能会被清除，仅供测试使用</span>
        </div>
        <template v-slot:append>
            <v-icon 
                icon="$mdiCloseCircle" 
                class="mr-2"
                @click="closeAppBar"
            ></v-icon>
        </template>
    </v-app-bar>
</template>