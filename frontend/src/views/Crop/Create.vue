<script lang="ts" setup>
import { ref, type Ref } from 'vue';
import type { RecordProperty } from '../../models/crop';
import PropertyOverview from '../../components/Property/Overview.vue';
import PropertyDialog from '../../components/Dialog/PropertyDialog.vue';
import draggable from 'vuedraggable';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { sleep } from '../../utils/sleep';
import { mergeUsers, type User } from '../../models/user';
import UserMultipleSelect from '../../components/User/MultipleSelect.vue';
import { propertyTypeMap } from '../../utils/property';

const loaded = ref(true);
const fetching = ref(false);
const title = ref("");
const description = ref("");
const property: Ref<RecordProperty> = ref({
    name: (new Date().getTime()).toString(),
    title: "",
    unit: "",
    required: false,
    def: "",
    options: [],
    type: "RecordPropertyType::NUMBER"
});
const properties: Ref<Array<RecordProperty> > = ref([]);
const propertiesDialog = ref(false);
const editingIndex = ref(-1);
const editors: Ref<Array<User> > = ref([]);
const viewers: Ref<Array<User> > = ref([]);

function create() {
    property.value = {
        name: (new Date().getTime()).toString(),
        title: "",
        unit: "",
        required: false,
        def: "",
        options: [],
        type: "RecordPropertyType::NUMBER"
    };
    editingIndex.value = -1;
    propertiesDialog.value = true;
}
function add() {
    if (property.value.title == "") {
        showMsg(MessageType.Error, "属性名不能为空");
        return;
    }
    if (propertyTypeMap[property.value.type!]?.options) {
        if (property.value.options?.length == 0) {
            showMsg(MessageType.Error, "至少需要添加一个选项");
            return;
        }
    } else {
        property.value.options = [];
    }

    if (!propertyTypeMap[property.value.type!]?.allowDef) {
        property.value.required = true;
        property.value.def = "";
    }

    if (editingIndex.value != -1) {
        properties.value[editingIndex.value] = property.value;
        editingIndex.value = -1;
    } else {
        properties.value.push(property.value);
    }

    propertiesDialog.value = false;
}
function edit(index: number) {
    property.value = properties.value[index]!;
    editingIndex.value = index;
    propertiesDialog.value = true;
}
function delete2(index: number) {
    properties.value.splice(index, 1);
}

async function submit() {
    if (title.value == "") {
        showMsg(MessageType.Error, "作物名称不能为空");
        return;
    }
    if (properties.value.length == 0) {
        showMsg(MessageType.Error, "请至少添加一个属性");
        return;
    }
    fetching.value = true;
    var res = await (await newFetch(`${API_BASE_URL}/crops/create`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            description: description.value,
            properties: properties.value,
            editors: editors.value.map((e) => e.uid!),
            viewers: mergeUsers(editors.value, viewers.value).map((e) => e.uid!)
        })
    }, () => { fetching.value = false; })).json();
    var id = res["id"];
    showMsg(MessageType.Success, "创建成功，正在跳转...");
    await sleep(1000);
    window.location.href = `/crops/${id}`;
}
</script>

<template>
    <div v-if="loaded">
        <h1>新建作物表</h1>
        <v-timeline align="start" side="end" class="justify-start Timeline">
            <v-timeline-item icon="$mdiCog" dot-color="red-lighten-1">
                <div>
                    <h2 class="ma-0 font-weight-light mb-4">基本信息</h2>
                    <v-text-field
                        v-model="title"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                    >
                        <template v-slot:label>
                            <span>作物名称</span>  
                            <span style="color: red">&nbsp;*</span>
                        </template>
                    </v-text-field>
                    <v-textarea
                        v-model="description"
                        label="作物描述"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        class="mt-4"
                        auto-grow
                    ></v-textarea>
                </div>
            </v-timeline-item>
            <v-timeline-item icon="$mdiDatabase" dot-color="purple-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">作物属性</h2>
                <v-list class="mb-4" v-if="properties.length">
                    <draggable v-model="properties" animation="200" item-key="name">
                        <template v-slot:item="{ element, index }">
                            <PropertyOverview 
                                :props="element"
                                :hasEdit="true"
                                :hasRemove="true"
                                :hasDetails="false"
                                @edit="edit(index)" 
                                @remove="delete2(index)"
                            ></PropertyOverview>
                        </template>
                    </draggable>
                </v-list>
                <div class="d-flex align-center justify-end">
                    <v-btn prepend-icon="$mdiPlus" color="primary" @click="create()">添加属性</v-btn>
                </div>
                <PropertyDialog
                    :title="editingIndex != -1 ? '编辑作物属性' : '添加作物属性'"
                    :btnTitle="editingIndex != -1 ? '保存修改' : '添加属性'"
                    :btnIcon="editingIndex != -1 ? '$mdiCheck' : '$mdiPlus'"
                    v-model:open="propertiesDialog" 
                    v-model:props="property" 
                    @submit="add"
                    :disabled="false"
                    :disableType="false"
                ></PropertyDialog>
            </v-timeline-item>
            <v-timeline-item icon="$mdiAccount" dot-color="green-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">成员权限</h2>
                <UserMultipleSelect
                    v-model:users="editors"
                    label="EDITORS（协作编辑）"
                    class="mb-6"
                    enableSelect
                ></UserMultipleSelect>
                <UserMultipleSelect
                    v-model:users="viewers"
                    label="VIEWERS（协作查看）"
                    enableSelect
                ></UserMultipleSelect>
            </v-timeline-item>
        </v-timeline>
        <div class="mt-4 d-flex align-center justify-end mb-4">
            <v-btn
                prepend-icon="$mdiCheck"
                color="primary"
                @click="submit()"
                :disabled="fetching"
            >新建</v-btn>
        </div>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>

<style lang="css" scoped>
.Timeline {
    grid-template-columns: auto min-content 1fr;
}
</style>

<style lang="css">
.v-timeline-item__body {
    width: 100%;
}
</style>