<script lang="ts" setup>
const props = defineProps<{
    label?: string
    class?: string
    hover?: boolean
}>();
const slots = defineSlots<{
    default: () => any,
    label: () => any
}>();
</script>

<template>
    <v-field 
        :class="`pa-2 outlined ${props.class || ''} ${props.hover ? 'hoverOutlined' : ''}`" 
        :label="props.label" 
        variant="outlined" 
        active 
        :focused="false"
    >
        <template v-if="$slots.label" v-slot:label>
            <span class="d-flex align-center">
                <slot name="label"></slot>
            </span>
        </template>
        <slot></slot>
    </v-field>
</template>

<style lang="css">
@media (hover: hover) {
    .outlined.v-field:hover .v-field__outline {
        --v-field-border-opacity: 0.38;
    }
    .outlined.v-field:hover > .v-field__outline {
        --v-field-border-opacity: 0.38;
    }
    .outlined.v-field .v-field:not(.outlined):hover .v-field__outline {
        --v-field-border-opacity: var(--v-high-emphasis-opacity);
    }

    .hoverOutlined.v-field:hover .v-field__outline {
        --v-field-border-opacity: var(--v-high-emphasis-opacity);
    }
    .hoverOutlined.v-field:hover .clearButton {
        opacity: var(--v-medium-emphasis-opacity);
    }
}
</style>