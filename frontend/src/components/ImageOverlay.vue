<script lang="ts" setup>
const model = defineModel<boolean>("model", { required: true });
const src = defineProps<{
    src: string
}>();
</script>

<template>
    <v-overlay v-model="model">
        <v-img :src="src.src" absolute></v-img>
    </v-overlay>
</template>

<style lang="css" scoped>
</style>