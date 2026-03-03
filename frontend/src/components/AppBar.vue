<script lang="ts" setup>
import { onMounted } from 'vue';
import { useTheme } from 'vuetify';

const theme = useTheme();

function themeToggle() {
    theme.toggle();
    localStorage.setItem("theme", theme.name.value);
}

onMounted(() => {
    var theme2 = localStorage.getItem("theme");
    if (theme2 == 'light' && theme.name.value != 'light') theme.toggle();
    else if (theme2 == 'dark' && theme.name.value != 'dark') theme.toggle();
})
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