<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, watch, type Ref } from 'vue';
import type { Crop, RecordProperty } from '../../models/crop';
import CropPropertyOverview from '../../components/Crop/PropertyOverview.vue';
import CropPropertyDialog from '../../components/Crop/PropertyDialog.vue';
import draggable from 'vuedraggable';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { sleep } from '../../utils/sleep';

async function load(to: any, from: any, next: any) {
    to; from;

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
const editors: Ref<Array<number> > = ref([]);
const editorSearch = ref("");
const editorItems: Ref<Array<any> > = ref([]);
const editorLoading = ref(false);
const viewers: Ref<Array<number> > = ref([]);
const viewerSearch = ref("");
const viewerItems: Ref<Array<any> > = ref([]);
const viewerLoading = ref(false);

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

async function submit() {
    if (properties.value.length == 0) {
        showMsg(MessageType.Error, "请至少添加一个属性");
        return;
    }
    var res = await (await newFetch(`${API_BASE_URL}/crops/create`, {
        method: "POST",
        body: JSON.stringify({
            title: title.value,
            description: description.value,
            properties: properties.value,
            editors: editors.value,
            viewers: viewers.value
        })
    })).json();
    var id = res["id"];
    showMsg(MessageType.Success, "创建成功，正在跳转...");
    await sleep(1000);
    window.location.href = `/crop/${id}`;
}

function loading(data: any) {
    data;

    loaded.value = true;
}

defineExpose({ loading });

watch(editorSearch, async (val) => {
    editorItems.value = editorItems.value.filter(item => editors.value.includes(item.value));
    if (!val) {}
    else {
        editorLoading.value = true;
        var users = await (await newFetch(`${API_BASE_URL}/users/search?keyword=${val}`)).json();
        for (var i = 0; i < users.items.length; i++)
            editorItems.value.push({
                title: users.items[i].name + "（" + users.items[i].email + "）",
                value: users.items[i].uid
            });
        editorItems.value = editorItems.value.filter((item, index, self) => self.findIndex(i => i.value == item.value) == index);
        editorItems.value = editorItems.value.sort((a, b) => a.value - b.value);
        editorLoading.value = false;
    }
})
watch(viewerSearch, async (val) => {
    viewerItems.value = viewerItems.value.filter(item => viewers.value.includes(item.value));
    if (!val) {} 
    else {
        viewerLoading.value = true;
        var users = await (await newFetch(`${API_BASE_URL}/users/search?keyword=${val}`)).json();
        for (var i = 0; i < users.items.length; i++)
            viewerItems.value.push({
                title: users.items[i].name + "（" + users.items[i].email + "）",
                value: users.items[i].uid
            });
        viewerItems.value = viewerItems.value.filter((item, index, self) => self.findIndex(i => i.value == item.value) == index);
        viewerItems.value = viewerItems.value.sort((a, b) => a.value - b.value);
        viewerLoading.value = false;
    }
})
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
                ></CropPropertyDialog>
            </v-timeline-item>
            <v-timeline-item icon="$mdiAccount" dot-color="green-lighten-1">
                <h2 class="ma-0 font-weight-light mb-4">成员权限</h2>
                <v-autocomplete
                    v-model="editors"
                    v-model:search="editorSearch"
                    :items="editorItems"
                    :loading="editorLoading"
                    label="EDITORS（协作编辑）"
                    autocomplete="off"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    hide-no-data
                    class="mb-4"
                    multiple
                ></v-autocomplete>
                <v-autocomplete
                    v-model="viewers"
                    v-model:search="viewerSearch"
                    :items="viewerItems"
                    :loading="viewerLoading"
                    label="VIEWERS（协作查看）"
                    autocomplete="off"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    hide-no-data
                    multiple
                ></v-autocomplete>
            </v-timeline-item>
        </v-timeline>
        <div class="mt-4 d-flex align-center justify-end">
            <v-btn
                prepend-icon="$mdiCheck"
                color="primary"
                @click="submit()"
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