<script setup lang="ts">
const props = defineProps(['isError', 'errorText', 'type', 'hasAppBar']);
</script>

<template>
    <div :style="`opacity: ${isError ? 1 : 0}; top: ${props.hasAppBar ? 64 : 16}px; display: ${isError ? 'block' : 'none'}`" class="message">
        <v-alert :type="type" style="min-width: 300px;">
            <template v-slot:text>
                <div v-html="errorText"></div>
            </template>
        </v-alert>
    </div>
</template>

<style lang="css" scoped>
.message {
    transition: opacity 0.5s;
    z-index: 999;
    right: 16px;
    position: fixed
}
</style>