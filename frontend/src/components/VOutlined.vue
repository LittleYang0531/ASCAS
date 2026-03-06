<script lang="ts" setup>
const props = defineProps<{
    label: string
}>();
</script>

<template>
    <v-field class="mt-4 pa-2 outlined" :label="props.label" variant="outlined" active :focused="false">
        <slot></slot>
    </v-field>
</template>