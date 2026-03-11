<script lang="ts" setup>
import type { RecordProperty } from '../../models/crop';

const icons = {
    "RecordPropertyType::NUMBER": "$mdiNumeric",
    "RecordPropertyType::STRING": "$mdiAlphabetical",
    "RecordPropertyType::SELECT": "$mdiFormatListBulleted",
    "RecordPropertyType::MULTI": "$mdiFormatListChecks",
    "RecordPropertyType::GEOMETRY": "$mdiMapMarker",
    "RecordPropertyType::IMAGE": "$mdiImage"
};

const props = defineProps<{
    props: RecordProperty
}>();
const emits = defineEmits<{
    (e: 'edit'): void,
    (e: 'remove'): void,
}>();
</script>

<template>
    <v-list-item
        :key="props.props.name"
        class="cursor-pointer"
        prepend-icon="$mdiDrag"
    >
        <p class="d-flex align-center ma-0">
            <v-icon :icon="icons[props.props.type as keyof typeof icons]" class="me-2"></v-icon>
            {{ props.props.title }}
            <span v-if="props.props.required" style="color: red">&nbsp;*</span>
            <span v-if="props.props.unit != ''">（{{ props.props.unit }}）</span>
        </p>
        <template v-slot:append>
            <v-btn
                color="info"
                icon="$mdiPencil"
                size="small"
                variant="text"
                @click="emits('edit')"
            ></v-btn>
            <v-btn
                color="error"
                icon="$mdiTrashCan"
                size="small"
                variant="text"
                @click="emits('remove')"
            ></v-btn>
        </template>
    </v-list-item>
</template>