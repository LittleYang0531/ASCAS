<script setup lang="ts">
import { useTheme } from 'vuetify';
import AppBar from './components/AppBar.vue';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { errorText, isError, type } from './utils/message';
import { onBeforeMount } from 'vue';

const theme = useTheme();

onBeforeMount(() => {
    theme.themes.value.light!.colors.primary = "#4CAF50";
    theme.themes.value.dark!.colors.primary = "#4CAF50";
})
</script>

<template>
    <v-app>
        <AppBar></AppBar>
        <NavigationDrawer></NavigationDrawer>
        <v-main>
            <router-view v-slot="{ Component }">
                <component :is="Component"></component>
            </router-view>
        </v-main>
    </v-app>
    <div class="nProcessBar">

    </div>
    <Message :isError="isError" :errorText="errorText" :type="type" :hasAppBar="true"></Message>
</template>