<script lang="ts" setup>
import { RecordPropertyType_icons, type RecordProperty } from '../../models/crop';

const props = defineProps<{
    props: RecordProperty,
    hasEdit: boolean,
    hasRemove: boolean,
    hasDetails: boolean
}>();
const emits = defineEmits<{
    (e: 'edit'): void,
    (e: 'remove'): void,
    (e: 'details'): void
}>();
</script>

<template>
    <v-list-item
        :key="props.props.name"
        class="cursor-pointer"
        prepend-icon="$mdiDrag"
    >
        <p class="d-flex align-center ma-0">
            <v-icon :icon="RecordPropertyType_icons[props.props.type as keyof typeof RecordPropertyType_icons]" class="me-2"></v-icon>
            {{ props.props.title }}
            <span v-if="props.props.required" style="color: red">&nbsp;*</span>
            <span v-if="props.props.unit != ''">（{{ props.props.unit }}）</span>
        </p>
        <template v-slot:append>
            <v-btn
                v-if="props.hasEdit"
                color="info"
                icon="$mdiPencil"
                size="small"
                variant="text"
                @click="emits('edit')"
            ></v-btn>
            <v-btn
                v-if="props.hasRemove"
                color="error"
                icon="$mdiTrashCan"
                size="small"
                variant="text"
                @click="emits('remove')"
            ></v-btn>
            <v-btn
                v-if="props.hasDetails"
                color="primary"
                icon="$mdiInformation"
                size="small"
                variant="text"
                @click="emits('details')"
            ></v-btn>
        </template>
    </v-list-item>
</template>