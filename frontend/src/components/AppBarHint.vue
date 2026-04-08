<script setup lang="ts">
import { onMounted } from 'vue';
import { versionString } from '../version';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';
import { useTheme } from 'vuetify';

const showAppBar = defineModel<boolean>("showAppBar");
const props = defineProps<{
    title: string
}>();
const emits = defineEmits<{
    (e: 'c'): void
}>();
function closeAppBar() {
    showAppBar.value = false;
    localStorage.setItem("showAppBar", "false");
}

const theme = useTheme();
function themeToggle() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

onMounted(() => {
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
    <v-app-bar color="warning" height="64" v-if="showAppBar">
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
    <v-app-bar color="primary" height="64" v-else>
        <v-app-bar-nav-icon @click="emits('c')"></v-app-bar-nav-icon>
        <v-app-bar-title>{{ props.title }}</v-app-bar-title>
        <template v-slot:append>
            <v-btn
                :icon="theme.name.value == 'light' ? '$mdiWeatherNight' : '$mdiWhiteBalanceSunny'"
                @click="themeToggle()"
            ></v-btn>
        </template>
    </v-app-bar>
</template>