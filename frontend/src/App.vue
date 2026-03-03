<script setup lang="ts">
import AppBar from './components/AppBar.vue';
import Message from './components/Message.vue';
import NavigationDrawer from './components/NavigationDrawer.vue';
import { errorText, isError, type } from './utils/message';
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