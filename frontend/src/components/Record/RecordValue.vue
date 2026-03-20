<script setup lang="ts">
import type { RecordProperty } from '../../models/crop';

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
    <td v-if="item.property.type == 'RecordPropertyType::NUMBER'" align="center">
        {{ value }}
    </td>
    <td v-if="item.property.type == 'RecordPropertyType::STRING'" class="text-truncate" style="max-width: 200px;" :title="(value as string)">
        {{ value }}
    </td>
    <td v-if="item.property.type == 'RecordPropertyType::SELECT'" align="center">
        <v-chip size="x-small" variant="outlined" color="primary">{{ value }}</v-chip>
    </td>
    <td v-if="item.property.type == 'RecordPropertyType::MULTI'" align="center">
        <div class="d-flex flex-column align-center justify-center">
            <div v-for="val in JSON.parse(value as string)">
                <v-chip size="x-small" variant="outlined" color="primary">{{ val }}</v-chip>
            </div>
        </div>
    </td>
    <td v-if="item.property.type == 'RecordPropertyType::GEOMETRY'" class="text-truncate" style="max-width: 200px;" :title="(value as string)">
        {{ value }}
    </td>
    <td v-if="item.property.type == 'RecordPropertyType::IMAGE'">
        <div 
            v-if="value != ''" 
            @click="(e) => { enableClick && emits('click'); e.stopPropagation(); }" 
            class="d-flex align-center ga-1 cursor-pointer"
        >
            <v-icon icon="$mdiImage" color="primary"></v-icon>
            <span>{{ value }}.jpg</span>
        </div>
    </td>
</template>