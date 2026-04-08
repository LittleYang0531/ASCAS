<script setup lang="ts">
import { ref } from 'vue';
import type { RecordProperty } from '../../models/crop';
import PropertyControl from '../Property/Control.vue';
import DeleteDialog from './DeleteDialog.vue';

const open = defineModel<boolean>("open", { required: true });
const values = defineModel<Record<string, string> > ("values", { required: true });
const props = defineProps<{
    properties: Array<RecordProperty>,
    cid: number
}>();
const emits = defineEmits<{
    (e: 'submit'): void,
    (e: 'remove'): void
}>();
const deleteDialogOpen = ref(false);
</script>

<template>
    <v-dialog v-model="open" max-width="672px">
        <v-card
            prepend-icon="$mdiPencil"
            title="修改记录"
            class="pt-1"
        >
            <div class="pa-5 pt-0">
                <PropertyControl
                    v-for="prop in props.properties"
                    v-model:model="values[prop.name!]!"
                    :props="prop"
                    :label="prop.title!"
                    :disabled="false"
                    :cropId="props.cid!"
                    class="mt-4"
                ></PropertyControl>
                <div class="mt-4 d-flex align-center justify-space-between ga-4">
                    <v-btn
                        prepend-icon="$mdiTrashCan"
                        color="error"
                        @click="deleteDialogOpen = true"
                    >删除记录</v-btn>
                    <v-btn
                        prepend-icon="$mdiPencil"
                        color="primary"
                        @click="emits('submit')"
                    >修改记录</v-btn>
                    <DeleteDialog v-model:open="deleteDialogOpen" @delete="emits('remove')"></DeleteDialog>
                </div>
            </div>
        </v-card>
    </v-dialog>
</template>