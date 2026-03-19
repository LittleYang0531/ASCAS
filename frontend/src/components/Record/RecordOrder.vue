<script setup lang="ts">
import { RecordPropertyType_icons } from '../../models/crop';
import type { OrderNode } from '../../models/record';

const props = defineProps<{
    order: OrderNode,
    title: Record<string, string>,
    type: Record<string, string>
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
        <div class="d-flex align-center ma-0 mr-4 ga-2">
            <v-icon
                :icon="props.order.isASC ? '$mdiSortAlphabeticalAscending' : '$mdiSortAlphabeticalDescending'"
                class="me-2"
                :title="props.order.isASC ? '升序排序' : '降序排序'"
                @click="updateASC()"
            ></v-icon>
            <v-select
                :label="props.order.isASC ? '升序排序（ASC）' : '降序排序（DESC）'"
                :model-value="props.order.column"
                :items="Object.keys(props.title).map((key) => ({ title: props.title[key], value: key }))"
                variant="outlined"
                density="comfortable"
                hide-details
                focused
                class="mt-2"
                @update:model-value="updateColumn"
            >
                <template v-slot:selection="{ item }">
                    <div class="d-flex align-center ga-2">
                        <v-icon
                            :icon="RecordPropertyType_icons[type[item.value!]! as keyof typeof RecordPropertyType_icons]"
                        ></v-icon>
                        <span>{{ item.title }}</span>
                    </div>
                </template>
                <template v-slot:item="{ props, item }">
                    <v-list-item v-bind="props" :title="undefined">
                        <div class="d-flex align-center ga-2">
                            <v-icon
                                :icon="RecordPropertyType_icons[type[item.value!]! as keyof typeof RecordPropertyType_icons]"
                            ></v-icon>
                            <span>{{ item.title }}</span>
                        </div>
                    </v-list-item>
                </template>
            </v-select>
        </div>
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