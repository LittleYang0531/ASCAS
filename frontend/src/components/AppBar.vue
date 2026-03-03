<script lang="ts" setup>
import { useTheme } from 'vuetify';

const theme = useTheme();

function themeToggle() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}
</script>

<template>
    <v-app-bar class="AppBar">
        <v-spacer></v-spacer>
        <v-icon 
            :icon="theme.name.value == 'light' ? 'mdi-weather-night' : 'mdi-white-balance-sunny'"
            size="small"
            @click="themeToggle()"
        ></v-icon>
    </v-app-bar>
</template>

<style lang="css" scoped>
.AppBar {
    padding: 0 16px;
}
</style>