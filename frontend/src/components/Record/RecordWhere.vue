<script setup lang="ts">
import type { WhereNode } from '../../models/record';
import { isNumeric } from '../../utils/property';
import VOutlined from '../VOutlined.vue';
import draggable from 'vuedraggable';

const where = defineModel<WhereNode>('where', { required: true });
const props = defineProps<{
    title: Record<string, string>,
    type: Record<string, string>,
    root?: boolean
}>();
const emits = defineEmits<{
    (e: 'remove'): void
}>();

const numberOp = [
    { title: "=", value: "SQLOperator::EQUAL" },
    { title: "!=", value: "SQLOperator::NOTEQUAL" },
    { title: ">", value: "SQLOperator::GREATER" },
    { title: ">=", value: "SQLOperator::GREATER_OR_EQUAL" },
    { title: "<", value: "SQLOperator::SMALLER" },
    { title: "<=", value: "SQLOperator::SMALLER_OR_EQUAL" },
];
const stringOp = [
    { title: "=", value: "SQLOperator::EQUAL" },
    { title: "!=", value: "SQLOperator::NOTEQUAL" },
    { title: "LIKE", value: "SQLOperator::LIKE" },
    { title: "NOT LIKE", value: "SQLOperator::NOTLIKE" },
    { title: "REGEXP", value: "SQLOperator::REGEXP" },
    { title: "NOT REGEXP", value: "SQLOperator::NOTREGEXP" },
]

function updateColumn() {
    if (isNumeric(props.type[(where.value as any).column!]!)) {
        (where.value as any).value = "0";
    } else {
        (where.value as any).value = "";
    }

    var op = isNumeric(props.type[(where.value as any).column!]!) ? numberOp : stringOp;
    var exists = false;
    for (var i = 0; i < op.length; i++) exists = exists || (where.value as any).op == op[i]?.value;
    if (!exists) (where.value as any).op = op[0]?.value;
}
</script>

<template>
    <v-list-item
        class="cursor-pointer"
        :prepend-icon="root ? '' : '$mdiDrag'"
        prepend-gap="16"
    >
        <div class="d-flex align-center justify-center ml-2 me-2">
            <template v-if="where.isLeaf">
                <div class="d-flex align-center ga-2" style="width: 100%">
                    <v-select
                        v-model="where.column"
                        :items="Object.keys(title).map((key) => ({ title: title[key], value: key }))"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        focused
                        @update:model-value="updateColumn()"
                    ></v-select>
                    <v-select
                        v-model="where.op"
                        :items="isNumeric(props.type[where.column!]!) ? numberOp : stringOp"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        focused
                    ></v-select>
                    <v-number-input
                        :model-value="Number(where.value)"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        @update:model-value="(val) => (where as any).value = val.toString()"
                        :precision="null"
                        class="flex-grow-1"
                        v-if="isNumeric(props.type[where.column!]!)"
                    ></v-number-input>
                    <v-text-field
                        v-model="where.value"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        clearable
                        class="flex-grow-1"
                        v-else
                    ></v-text-field>
                </div>
            </template>
            <template v-else>
                <v-icon 
                    :icon="where.isAnd ? '$mdiVectorArrangeAbove' : '$mdiVectorArrangeBelow'"
                    :title="where.isAnd ? '与' : '或'"
                    class="me-4"
                    @click="where.isAnd = !where.isAnd"
                ></v-icon>
                <div style="width: 100%">
                    <VOutlined
                        :label="where.isAnd ? '与条件（AND）' : '或条件（OR）'"
                        class="mt-2"
                    >
                        <div style="width: 100%" v-if="where.params!.length">
                            <draggable v-model="where.params" group="where" animation="200" item-key="name">
                                <template v-slot:item="{ index }">
                                    <RecordWhere
                                        v-model:where="where.params![index]!"
                                        :title="props.title"
                                        :type="props.type"
                                        @remove="where.params?.splice(index, 1)"
                                    ></RecordWhere>
                                </template>
                            </draggable>
                        </div>
                        <div style="width: 100%; height: 64px;" v-else>
                            <draggable v-model="where.params" group="where" animation="200" item-key="name">
                                <template v-slot:item="{}"></template>
                                <template v-slot:footer v-if="!where.params?.length">
                                    <div class="d-flex align-center justify-center" style="width: 100%; height: 64px;">
                                        <v-icon icon="$mdiProgressAlert" color="error" class="me-2"></v-icon>
                                        <span>空条件，默认为真</span>
                                    </div>
                                </template>
                            </draggable>
                        </div>
                    </VOutlined>
                </div>
            </template>
        </div>
        <template v-slot:append v-if="!root">
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