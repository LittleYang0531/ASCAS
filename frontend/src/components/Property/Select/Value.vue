<script setup lang="ts">
import type { RecordProperty } from '../../../models/crop';

const item = defineProps<{
    cid: number,
    property: RecordProperty,
    value: string | number
    enableClick: boolean
}>();
const emits = defineEmits<{
    (e: 'click'): void
}>();
</script>

<template>
    <div class="d-flex flex-column align-center justify-center">
        <v-chip size="x-small" variant="outlined" color="primary">{{ value }}</v-chip>
    </div>
</template>