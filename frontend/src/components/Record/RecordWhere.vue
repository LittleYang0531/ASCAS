<script setup lang="ts">
import type { WhereNode } from '../../models/record';
import VOutlined from '../VOutlined.vue';

const props = defineProps<{
    where: WhereNode
}>();
const emits = defineEmits<{
    (e: 'update', where: WhereNode): void,
    (e: 'remove'): void
}>();
</script>

<template>
    <v-list-item
        class="cursor-pointer"
        prepend-icon="$mdiDrag"
        style="width: 100%"
    >
        <div class="d-flex align-center justify-center me-2">
            <template v-if="props.where.isLeaf">

            </template>
            <template v-else>
                <v-icon 
                    :icon="props.where.isAnd ? '$mdiVectorArrangeAbove' : '$mdiVectorArrangeBelow'"
                    :title="props.where.isAnd ? '与' : '或'"
                    class="me-4"
                ></v-icon>
                <div style="width: 100%">
                    <VOutlined
                        :label="props.where.isAnd ? '与条件（AND）' : '或条件（OR）'"
                        class="mt-2"
                    >
                        <div v-if="props.where.params!.length">
                            <RecordWhere
                                v-for="param in props.where.params"
                                :where="param"
                            ></RecordWhere>
                        </div>
                        <div v-else class="d-flex align-center justify-center" style="width: 100%; height: 32px;">
                            <v-icon icon="$mdiProgressAlert" color="error" class="me-2"></v-icon>
                            <span>空条件，默认为真</span>
                        </div>
                    </VOutlined>
                </div>
            </template>
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