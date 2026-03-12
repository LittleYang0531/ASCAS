<script lang="ts">
import NProgress from 'nprogress';
import { defineComponent, ref, type Ref } from 'vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import { UserPermission_icons, type Crop, type RecordProperty } from '../../models/crop';
import CropPropertyOverview from '../../components/Crop/PropertyOverview.vue';
import CropPropertyDialog from '../../components/Crop/PropertyDialog.vue';
import UserMultipleSelect from '../../components/User/MultipleSelect.vue';
import draggable from 'vuedraggable';
import UserCard from '../../components/User/Card.vue';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { sleep } from '../../utils/sleep';
import type { User } from '../../models/user';

async function load(to: any, from: any, next: any) {
    to; from;

    NProgress.start();
    NProgress.inc();

    var crop = await (await newFetch(`${API_BASE_URL}/crops/${to.params.id}`)).json();

    next((e: any) => e.loading({
        crop: crop.item,
        param: to.query
    }));
}
export default defineComponent({
    async beforeRouteEnter(to, from, next) { await load(to, from, next); },
    async beforeRouteUpdate(to, from) { await load(to, from, (func: any) => func(this)); },
});
</script>

<script lang="ts" setup>
const loaded = ref(false);
const item: Ref<Crop> = ref({});
const tab = ref("properties");

const propertiesDialog = ref(false);
const editingIndex = ref(-1);
const editors: Ref<Array<User> > = ref([]);
const viewers: Ref<Array<User> > = ref([]);

function loading(data: any) {
    item.value = data.crop;
    editors.value = item.value.editors!;
    viewers.value = item.value.viewers!;
    if (data.param.page != undefined) tab.value = data.param.page;
    
    loaded.value = true;
}

const showDialog = ref(false);
const dialogData: Ref<RecordProperty> = ref({});
function showPropertyDetails(prop: RecordProperty) {
    dialogData.value = prop;
    showDialog.value = true;
}

defineExpose({ loading });

const property: Ref<RecordProperty> = ref({
    name: (new Date().getTime()).toString(),
    title: "",
    unit: "",
    required: false,
    def: "",
    options: [],
    type: "RecordPropertyType::NUMBER"
});
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
        item.value.properties![editingIndex.value] = property.value;
        editingIndex.value = -1;
    } else {
        item.value.properties?.push(property.value);
    }

    propertiesDialog.value = false;
}
function edit(props: RecordProperty) {
    property.value = JSON.parse(JSON.stringify(props));
    editingIndex.value = item.value.properties?.indexOf(props)!;
    propertiesDialog.value = true;
}
function delete2(props: RecordProperty) {
    var index = item.value.properties?.indexOf(props)!;
    if (index > -1) {
        item.value.properties?.splice(index, 1);
    }
}
async function submit() {
    if (item.value.properties?.length == 0) {
        showMsg(MessageType.Error, "请至少添加一个属性");
        return;
    }
    await (await newFetch(`${API_BASE_URL}/crops/${item.value.cid}/edit`, {
        method: "POST",
        body: JSON.stringify({
            cid: item.value.cid,
            title: item.value.title,
            description: item.value.description,
            properties: item.value.properties,
            editors: editors.value.map((e) => e.uid!),
            viewers: viewers.value.map((e) => e.uid!)
        })
    })).json();
    showMsg(MessageType.Success, "修改成功");
    await sleep(1000);
    window.location.href = `/crops/${item.value.cid}`;
}
</script>

<template>
    <div v-if="loaded">
        <div class="d-flex align-center ga-2">
            <h1>{{ item.title }}</h1>
            <v-icon :icon="UserPermission_icons[item.permission as keyof typeof UserPermission_icons] || '$mdiLock'"></v-icon>
            <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">id：{{ item.name }}</v-chip>
            <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">
                {{ item.permission?.substring("UserPermission::".length) }}
            </v-chip>
        </div>
        <p class="mt-0 text-medium-emphasis">{{ item.description }}</p>
        <v-divider></v-divider>

        <v-tabs v-model="tab" color="primary">
            <v-tab value="properties">属性列表</v-tab>
            <v-tab value="simple">样例数据</v-tab>
            <v-tab value="editors">协作编辑</v-tab>
            <v-tab value="viewers">协作查看</v-tab>
            <v-tab value="edit" v-if="item.permission == 'UserPermission::OWNER'">作物表编辑</v-tab>
        </v-tabs>
        <v-divider></v-divider>

        <v-tabs-window v-model="tab">
            <!-- 属性列表 -->
            <v-tabs-window-item value="properties">
                <v-list style="width: 100%">
                    <CropPropertyOverview
                        v-for="prop in item.properties"
                        :hasEdit="false"
                        :hasRemove="false"
                        :hasDetails="true"
                        @details="showPropertyDetails(prop)"
                        :props="prop"
                    ></CropPropertyOverview>
                </v-list>
            </v-tabs-window-item>
            <!-- 简要数据 -->
            <v-tabs-window-item value="simple">

            </v-tabs-window-item>
            <!-- 协作编辑 -->
            <v-tabs-window-item value="editors">
                <v-list style="width: 100%">
                    <UserCard v-for="user in item.editors" :user="user" :active-click="true"></UserCard>
                </v-list>
            </v-tabs-window-item>
            <!-- 协作查看 -->
            <v-tabs-window-item value="viewers">
                <v-list style="width: 100%">
                    <UserCard v-for="user in item.viewers" :user="user" :active-click="true"></UserCard>
                </v-list>
            </v-tabs-window-item>
            <!-- 作物表编辑 -->
            <v-tabs-window-item value="edit" class="pa-4" v-if="item.permission == 'UserPermission::OWNER'">
                <h2 class="ma-0 mb-4">基本信息</h2>
                <v-text-field
                    v-model="item.title"
                    label="作物名称"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                ></v-text-field>
                <v-text-field
                    v-model="item.description"
                    label="作物描述"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    class="mt-4"
                ></v-text-field>
                <h2 class="ma-0 mt-4 mb-4">作物属性</h2>
                <v-list class="mb-4" v-if="item.properties?.length">
                    <draggable v-model="item.properties" item-key="name">
                        <template #item="{ element }">
                            <CropPropertyOverview 
                                :props="element"
                                :hasEdit="true"
                                :hasRemove="true"
                                :hasDetails="false"
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
                    :disabled="false"
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
                    >修改</v-btn>
                </div>
            </v-tabs-window-item>
        </v-tabs-window>
        
        <CropPropertyDialog
            title="查看作物属性"
            btnTitle="关闭窗口"
            btnIcon="$mdiClose"
            v-model:open="showDialog" 
            v-model:props="dialogData" 
            @submit="showDialog = false"
            :disabled="true"
        ></CropPropertyDialog>
    </div>
    <div v-else class="d-flex justify-center align-center position-absolute" style="width: 100%; height: 100vh; max-width: 960px;">
        <v-progress-circular indeterminate size="64" color="primary"></v-progress-circular>
    </div>
</template>