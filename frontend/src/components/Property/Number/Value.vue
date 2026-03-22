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
    <div class="text-truncate d-flex align-center justify-center">
        {{ value }}
    </div>
</template>