<script setup lang="ts">
import { useTheme } from 'vuetify';
import type { User } from '../models/user';
import { API_BASE_URL } from '../config';
import { locate } from '../router';
import { useRoute } from 'vue-router';

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
    console.log(theme.name.value);
    localStorage.setItem("theme", theme.name.value);
}
</script>

<template>
    <v-navigation-drawer
        expand-on-hover
        permanent
        rail
    >
        <v-list>
            <v-list-item
                :prepend-avatar="`${API_BASE_URL}/users/avatar/${props.user.uid}`"
                :subtitle="props.user.email"
                :title="props.user.name"
                @click="locate('/users/' + props.user.uid)"
            ></v-list-item>
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
                title="服务器设置"
                :active="active('/settings')"
                @click="locate('/settings')"
            ></v-list-item>
            <v-list-item
                prepend-icon="$mdiLogout"
                title="退出登录"
                @click=""
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