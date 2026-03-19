<script lang="ts" setup>
import { ref } from 'vue';
import type { RecordProperty } from '../../models/crop';
import PropertyControl from './PropertyControl.vue';
import draggable from 'vuedraggable';
import VOutlined from '../VOutlined.vue';

const props = defineModel<RecordProperty>("props", { required: true });
const open = defineModel<boolean>("open", { required: true });
const title = defineProps<{
    title: string,
    btnTitle: string,
    btnIcon: string,
    disabled: boolean,
    disableType: boolean
}>();
const emits = defineEmits<{
    (e: 'submit'): void,
}>();
const addOption = ref("");
const editingIndex = ref(-1);
const editingValue = ref("");

function edit(index: number) {
    editingValue.value = props.value.options![index]!;
    editingIndex.value = index;
}
function submitEdit(index: number) {
    if (index > -1) props.value.options![index] = editingValue.value;
    editingIndex.value = -1;
}
function remove(index: number) {
    props.value.options!.splice(index, 1);
}
</script>

<template>
    <v-dialog v-model="open" max-width="448px">
        <v-card
            prepend-icon="$mdiListBox"
            :title="title.title"
            class="pt-1"
        >
            <div class="pa-5">
                <v-select
                    v-model="props.type"
                    label="属性类型"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    :items='[
                        { title: "数值型", value: "RecordPropertyType::NUMBER" },
                        { title: "字符串型", value: "RecordPropertyType::STRING" },
                        { title: "单项选择", value: "RecordPropertyType::SELECT" },
                        { title: "多项选择", value: "RecordPropertyType::MULTI" },
                        { title: "定位坐标", value: "RecordPropertyType::GEOMETRY" },
                        { title: "上传图片", value: "RecordPropertyType::IMAGE" }
                    ]'
                    :disabled="title.disabled || title.disableType"
                    @click="props.def = ''"
                ></v-select>
                <v-text-field
                    v-model="props.title"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    class="mt-4"
                    :disabled="title.disabled"
                >
                    <template v-slot:label>
                        <span>属性名称</span>  
                        <span style="color: red">&nbsp;*</span>
                    </template>
                </v-text-field>
                <div v-if="props.type != 'RecordPropertyType::GEOMETRY' && props.type != 'RecordPropertyType::IMAGE'">
                    <v-text-field
                        v-model="props.unit"
                        label="属性单位"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        class="mt-4"
                        :disabled="title.disabled"
                    ></v-text-field>
                    <VOutlined label="可选选项" v-if="props.type == 'RecordPropertyType::SELECT' || props.type == 'RecordPropertyType::MULTI'" class="mt-4">
                        <template v-slot:label>
                            <span>可选选项</span>  
                            <span style="color: red">&nbsp;*</span>
                        </template>
                        <div class="d-flex flex-column" style="width: 100%">
                            <v-list class="pa-0" v-if="props.options?.length" :disabled="title.disabled">
                                <draggable v-model="props.options" animation="200" item-key="name">
                                    <template v-slot:item="{ element, index }">
                                        <v-list-item prepend-icon="$mdiDrag">
                                            <span v-if="editingIndex != index">{{ element }}</span>
                                            <v-text-field
                                                v-else
                                                v-model="editingValue"
                                                bg-color="transparent"
                                                class="mr-3"
                                                density="compact"
                                                variant="plain"
                                                autofocus
                                                hide-details
                                                @click.stop
                                                @keydown.stop
                                                @keyup.enter="submitEdit(index)"
                                                @mousedown.stop
                                            ></v-text-field>
                                            <template v-slot:append>
                                                <v-btn
                                                    :icon="editingIndex != index ? `$mdiPencil` : `$mdiCheck`"
                                                    :color="editingIndex != index ? 'info' : 'success'"
                                                    size="small"
                                                    variant="text"
                                                    @click="editingIndex != index ? edit(index) : submitEdit(index)"
                                                    :disabled="title.disabled"
                                                ></v-btn>
                                                <v-btn
                                                    icon="$mdiTrashCan"
                                                    color="error"
                                                    size="small"
                                                    variant="text"
                                                    @click="remove(index)"
                                                    :disabled="title.disabled"
                                                ></v-btn>
                                            </template>
                                        </v-list-item>
                                    </template>
                                </draggable>
                            </v-list>
                            <v-list v-else>
                                <v-list-item>
                                    <div class="d-flex align-center justify-center ga-1">
                                        <v-icon icon="$mdiProgressAlert" color="error"></v-icon>
                                        <span>暂无选项</span>
                                    </div>
                                </v-list-item>
                            </v-list>
                            <div class="d-flex align-center mt-2 ga-2">
                                <v-text-field
                                    v-model="addOption"
                                    label="添加选项"
                                    variant="outlined"
                                    density="comfortable"
                                    hide-details
                                    active
                                    :disabled="title.disabled"
                                >
                                    <template v-slot:label>
                                        <span>添加选项</span>  
                                        <span style="color: red">&nbsp;*</span>
                                    </template>
                                </v-text-field>
                                <v-btn
                                    icon="$mdiPlus"
                                    @click="() => { addOption && props.options?.push(addOption); addOption = ''; }"
                                    color="primary"
                                    :disabled="title.disabled"
                                ></v-btn>
                            </div>
                        </div>
                    </VOutlined>
                    <div class="d-flex justify-space-between mt-2">
                        <p>是否为必填项</p>
                        <v-switch v-model="props.required" hide-details color="primary" :disabled="title.disabled"></v-switch>
                    </div>
                    <PropertyControl v-model:model="props.def!" :props="props" label="默认值" class="mt-2" :disabled="title.disabled"></PropertyControl>
                </div>
                <div class="mt-3 d-flex justify-center">
                    <v-btn
                        :prepend-icon="title.btnIcon"
                        color="primary"
                        @click="emits('submit')"
                    >{{ title.btnTitle }}</v-btn>
                </div>
            </div>
        </v-card>
    </v-dialog>
</template>