<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import type { RecordProperty } from '../../models/crop';
import CropPropertyOverview from '../../components/Crop/PropertyOverview.vue';
import CropPropertyDialog from '../../components/Crop/PropertyDialog.vue';
import draggable from 'vuedraggable';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';

async function load(to: any, from: any, next: any) {
    NProgress.start();
    NProgress.inc();

    next((e: any) => e.loading({

    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
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

    property.value = {
        name: (new Date().getTime()).toString(),
        title: "",
        unit: "",
        required: false,
        def: "",
        options: [],
        type: "RecordPropertyType::NUMBER"
    };
    propertiesDialog.value = false;
}
function edit(props: RecordProperty) {
    property.value = JSON.parse(JSON.stringify(props));
    editingIndex.value = properties.value.indexOf(props);
    propertiesDialog.value = true;
}
function delete2(props: RecordProperty) {
    var index = properties.value.indexOf(props);
    console.log(index);
    if (index > -1) {
        properties.value.splice(index, 1);
    }
}

function loading(data: any) {

    loaded.value = true;
}

defineExpose({ loading });
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
                        label="作物名称"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                    ></v-text-field>
                    <v-text-field
                        v-model="description"
                        label="作物描述"
                        variant="outlined"
                        density="comfortable"
                        hide-details
                        class="mt-4"
                    ></v-text-field>
                </div>
            </v-timeline-item>
            <v-timeline-item icon="$mdiDatabase" dot-color="purple-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">作物属性</h2>
                <v-list class="mb-4" v-if="properties.length">
                    <draggable v-model="properties" item-key="name">
                        <template #item="{ element }">
                            <CropPropertyOverview 
                                :props="element" 
                                @edit="edit(element)" 
                                @remove="delete2(element)"
                            ></CropPropertyOverview>
                        </template>
                    </draggable>
                </v-list>
                <v-btn prepend-icon="$mdiPlus" color="primary" @click="propertiesDialog = true">添加属性</v-btn>
                <CropPropertyDialog 
                    :title="editingIndex != -1 ? '编辑作物属性' : '添加作物属性'"
                    :btnTitle="editingIndex != -1 ? '保存修改' : '添加属性'"
                    :btnIcon="editingIndex != -1 ? '$mdiCheck' : '$mdiPlus'"
                    v-model:open="propertiesDialog" 
                    v-model:props="property" 
                    @submit="add"
                ></CropPropertyDialog>
            </v-timeline-item>
            <v-timeline-item icon="$mdiAccount" dot-color="green-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">成员权限</h2>
            </v-timeline-item>
        </v-timeline>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh;">
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