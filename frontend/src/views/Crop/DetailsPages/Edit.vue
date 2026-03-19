<script setup lang="ts">
import { onBeforeMount, ref, type Ref } from 'vue';
import type { Crop, RecordProperty } from '../../../models/crop';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import type { User } from '../../../models/user';
import { sleep } from '../../../utils/sleep';
import { newFetch } from '../../../utils/fetch';
import { API_BASE_URL } from '../../../config';
import CropPropertyOverview from '../../../components/Crop/PropertyOverview.vue';
import CropPropertyDialog from '../../../components/Crop/PropertyDialog.vue';
import UserMultipleSelect from '../../../components/User/MultipleSelect.vue';
import draggable from 'vuedraggable';

const fetching = ref(false);
const item = defineProps<{
    item: Crop
}>();
const property: Ref<RecordProperty> = ref({
    name: (new Date().getTime()).toString(),
    title: "",
    unit: "",
    required: false,
    def: "",
    options: [],
    type: "RecordPropertyType::NUMBER"
});
const title = ref("");
const description = ref("");
const editors: Ref<Array<User> > = ref([]);
const viewers: Ref<Array<User> > = ref([]);
const properties = ref<Array<RecordProperty>>([]);
const propertiesDialog = ref(false);
const editingIndex = ref(-1);
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
    if (property.value.type == "RecordPropertyType::SELECT" || property.value.type == "RecordPropertyType::MULTI") {
        if (property.value.options?.length == 0) {
            showMsg(MessageType.Error, "至少需要添加一个选项");
            return;
        }
    } else {
        property.value.options = [];
    }

    if (property.value.type == "RecordPropertyType::GEOMETRY" || property.value.type == "RecordPropertyType::IMAGE") {
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
    property.value = properties.value[index]!
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
    await (await newFetch(`${API_BASE_URL}/crops/${item.item.cid}/edit`, {
        method: "POST",
        body: JSON.stringify({
            cid: item.item.cid,
            title: title.value,
            description: description.value,
            properties: properties.value,
            editors: editors.value.map((e) => e.uid!),
            viewers: viewers.value.map((e) => e.uid!)
        })
    }, () => { fetching.value = false; })).json();
    showMsg(MessageType.Success, "修改成功");
    await sleep(1000);
    window.location.href = `/crops/${item.item.cid}`;
}

onBeforeMount(() => {
    title.value = item.item.title!;
    description.value = item.item.description!;
    editors.value = item.item.editors!;
    viewers.value = item.item.viewers!;
    properties.value = item.item.properties!;
})
</script>

<template>
    <h2 class="ma-0 mb-4">基本信息</h2>
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
    <h2 class="ma-0 mt-4 mb-4">作物属性</h2>
    <v-list class="mb-4" v-if="properties.length">
        <draggable v-model="properties" item-key="name">
            <template #item="{ element, index }">
                <CropPropertyOverview 
                    :props="element"
                    :hasEdit="true"
                    :hasRemove="true"
                    :hasDetails="false"
                    @edit="edit(index)"
                    @remove="delete2(index)"
                ></CropPropertyOverview>
            </template>
        </draggable>
    </v-list>
    <div class="d-flex align-center justify-end">
        <v-btn prepend-icon="$mdiPlus" color="primary" @click="create()">添加属性</v-btn>
    </div>
    <CropPropertyDialog
        :title="editingIndex != -1 ? '编辑作物属性' : '添加作物属性'"
        :btnTitle="editingIndex != -1 ? '保存修改' : '添加属性'"
        :btnIcon="editingIndex != -1 ? '$mdiCheck' : '$mdiPlus'"
        v-model:open="propertiesDialog" 
        v-model:props="property" 
        @submit="add"
        :disabled="false"
        :disableType="editingIndex != -1"
    ></CropPropertyDialog>
    <h2 class="ma-0 mt-4 mb-4">成员权限</h2>
    <UserMultipleSelect
        v-model:users="editors"
        label="EDITORS（协作编辑）"
        class="mb-6"
    ></UserMultipleSelect>
    <UserMultipleSelect
        v-model:users="viewers"
        label="VIEWERS（协作查看）"
    ></UserMultipleSelect>
    <div class="mt-4 d-flex align-center justify-end">
        <v-btn
            prepend-icon="$mdiCheck"
            color="primary"
            @click="submit()"
            :disabled="fetching"
        >修改</v-btn>
    </div>
</template>