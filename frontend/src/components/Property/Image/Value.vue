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
    <div class="d-flex align-center justify-begin">
        <div 
            v-if="value != ''" 
            @click="(e) => { enableClick && emits('click'); e.stopPropagation(); }" 
            class="d-flex align-center ga-1 cursor-pointer"
        >
            <v-icon icon="$mdiImage" color="primary"></v-icon>
            <span>{{ value }}.jpg</span>
        </div>
    </div>
</template>