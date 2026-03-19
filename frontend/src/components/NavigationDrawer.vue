<script setup lang="ts">
import { useTheme } from 'vuetify';
import type { User } from '../models/user';
import { locate } from '../router';
import { useRoute } from 'vue-router';
import UserCard from './User/Card.vue';
import { showMsg } from '../utils/message';
import { MessageType } from '../models/message';
import { sleep } from '../utils/sleep';

const theme = useTheme();
const route = useRoute();
const props = defineProps<{
    user: User
}>();

function active(prefix: string) {
    return route.path.startsWith(prefix);
}

function themeToggle() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

function setCookie(name: string, value: string, days: number) {
    var expires = "";
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        expires = "; expires=" + date.toUTCString();
    }
    document.cookie = name + "=" + value + expires + "; path=/";
}

async function logout() {
    setCookie("session", "", -1);
    showMsg(MessageType.Success, "退出登录成功");
    await sleep(1000);
    location.href = location.href;
}
</script>

<template>
    <v-navigation-drawer
        expand-on-hover
        permanent
        rail
    >
        <v-list>
            <UserCard :user="props.user" :active-click="true"></UserCard>
        </v-list>

        <v-divider></v-divider>

        <v-list dense nav density="compact">
            <v-list-item
                prepend-icon="$mdiHome"
                title="主页"
                :active="route.path == '/' || route.path == '/index'"
                @click="locate('/')"
            ></v-list-item>
            <v-list-item
                prepend-icon="$mdiDatabase"
                title="我的数据"
                :active="active('/crops')"
                @click="locate('/crops/list')"
            ></v-list-item>
        </v-list>

        <v-divider></v-divider>
        
        <v-list dense nav density="compact">
            <v-list-item
                prepend-icon="$mdiMessage"
                title="我的消息"
                :active="active('/messages')"
                @click="locate('/messages/list')"
            ></v-list-item>
            <v-list-item
                prepend-icon="$mdiAccountGroup"
                title="我的团队"
                :active="active('/teams')"
                @click="locate('/teams/list')"
            ></v-list-item>
            <v-list-item
                prepend-icon="$mdiAccountCog"
                title="个人设置"
                :active="active('/users/' + props.user.uid)"
                @click="locate('/users/' + props.user.uid)"
            ></v-list-item>
        </v-list>

        <v-divider></v-divider>
        <v-list dense nav density="compact">
            <v-list-item
                prepend-icon="$mdiCog"
                title="应用设置"
                :active="active('/settings')"
                @click="locate('/settings')"
            ></v-list-item>
            <v-list-item
                prepend-icon="$mdiLogout"
                title="退出登录"
                @click="logout()"
            ></v-list-item>
        </v-list>

        <template v-slot:append>
            <v-list dense nav density="compact">
                <v-list-item
                    :prepend-icon="theme.name.value == 'light' ? '$mdiWeatherNight' : '$mdiWhiteBalanceSunny'"
                    :title="`切换为${theme.name.value == 'light' ? '暗' : '亮'}色主题`"
                    @click="themeToggle()"
                ></v-list-item>
            </v-list>
        </template>
    </v-navigation-drawer>
</template>