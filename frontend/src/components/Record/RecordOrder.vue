<script setup lang="ts">
import type { OrderNode } from '../../models/record';

const props = defineProps<{
    order: OrderNode,
    title: Record<string, string>
}>();
const emits = defineEmits<{
    (e: 'update', order: OrderNode): void,
    (e: 'remove'): void
}>();

function updateColumn(column: string) {
    emits('update', { 
        column: column, 
        isASC: props.order.isASC 
    });
}
function updateASC() {
    emits('update', { 
        column: props.order.column, 
        isASC: !props.order.isASC 
    });
}
</script>

<template>
    <v-list-item
        class="cursor-pointer"
        prepend-icon="$mdiDrag"
    >
        <p class="d-flex align-center ma-0 mr-4 ga-2">
            <v-icon
                :icon="props.order.isASC ? '$mdiSortAlphabeticalAscending' : '$mdiSortAlphabeticalDescending'"
                class="me-2"
                :title="props.order.isASC ? '升序排序' : '降序排序'"
                @click="updateASC()"
            ></v-icon>
            <v-select
                :model-value="props.order.column"
                :items="Object.keys(title).map((key) => ({ title: title[key], value: key }))"
                variant="outlined"
                density="comfortable"
                hide-details
                focused
                @update:model-value="updateColumn"
            ></v-select>
        </p>
        <template v-slot:append>
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