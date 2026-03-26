<script setup lang="ts">
const model = defineModel<number | undefined>('open', { required: true });
const slots = defineSlots<{
    default: () => any,
    title: () => any
}>();
</script>

<template>
    <v-expansion-panels static bg-color="transparent" v-model="model" elevation="0">
        <v-expansion-panel value="1">
            <v-expansion-panel-title class="pa-4 MyExpansionPanelTitle">
                <slot name="title"></slot>
            </v-expansion-panel-title>
            <v-expansion-panel-text class="MyExpansionPanelText pa-4 pt-0">
                <slot></slot>
            </v-expansion-panel-text>
        </v-expansion-panel>
    </v-expansion-panels>
</template>