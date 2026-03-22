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
        <div v-for="val in JSON.parse(value as string)">
            <v-chip size="x-small" variant="outlined" color="primary">{{ val }}</v-chip>
        </div>
    </div>
</template>