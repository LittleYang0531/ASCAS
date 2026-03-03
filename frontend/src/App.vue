<script setup lang="ts">
import { useTheme } from 'vuetify';
import Message from './components/Message.vue';
import { errorText, isError, type } from './utils/message';

const theme = useTheme();
</script>

<template>
    <v-app>
        <v-app-bar>
            <v-spacer></v-spacer>
            <v-btn :prependIcon="theme.name.value == 'light' ? 'mdi-weather-night' : 'mdi-white-balance-sunny'"
                text="Toggle Theme" slim @click="theme.toggle()"></v-btn>
        </v-app-bar>

        <v-navigation-drawer>
            <v-list>
                <v-list-item title="Navigation drawer"></v-list-item>
            </v-list>
        </v-navigation-drawer>

        <v-main>
            <router-view v-slot="{ Component }">
                <component :is="Component"></component>
            </router-view>
        </v-main>
    </v-app>
    <div class="nProcessBar">

    </div>
    <Message :isError="isError" :errorText="errorText" :type="type" hasAppBar="true"></Message>
</template>