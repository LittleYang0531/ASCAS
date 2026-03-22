<script setup lang="ts">
import { onBeforeMount, ref, watchEffect, type Ref } from 'vue';
import type { Crop, RecordProperty } from '../../../models/crop';
import type { OrderNode, WhereNode } from '../../../models/record';
import { API_BASE_URL } from '../../../config';
import { newFetch } from '../../../utils/fetch';
import RecordValue from '../../../components/Property/Value.vue';
import ImageOverlay from '../../../components/Overlay/ImageOverlay.vue';
import draggable from 'vuedraggable';
import RecordOrder from '../../../components/Record/RecordOrder.vue';
import RecordWhere from '../../../components/Record/RecordWhere.vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import RecordEditDialog from '../../../components/Dialog/RecordEditDialog.vue';
import { sleep } from '../../../utils/sleep';
import RecordExportDialog from '../../../components/Dialog/RecordExportDialog.vue';
import { userId } from '../../../utils/user';

const crop = defineProps<{
    crop: Crop
}>();
const itemLength = ref(0);
const data: Ref<Array<Record<string, string>>> = ref([]);
const itemsPerPage = ref(10);
const loading = ref(false);
const headers: Ref<Array<Object>>  = ref([]);

// 数据筛选器模块
const filterModel: Ref<number | undefined> = ref(undefined);
const columnTitle: Ref<Record<string, string>> = ref({});
const columnType: Ref<Record<string, string>> = ref({});
const whereNodes: Ref<WhereNode> = ref({ isLeaf: false, isAnd: true, params: [] });
const whereString = ref("");
const whereCount = ref(0);
const orderNodes: Ref<Array<OrderNode> > = ref([{ column: "id", isASC: true }]);
const orderString = ref("");
const orderCount = ref(0);
function resetWhere() {
    whereNodes.value = { isLeaf: false, isAnd: true, params: [] };
}
function addWhereNode() {
    (whereNodes.value as any).params!.push({ 
        isLeaf: false, 
        isAnd: true,
        params: []
    });
}
function addWhereLeaf() {
    (whereNodes.value as any).params!.push({ 
        isLeaf: true, 
        column: "id", 
        op: "SQLOperator::EQUAL", 
        value: "0" 
    });
}
function updateOrder(index: number, order: OrderNode) {
    orderNodes.value[index] = order;
}
function removeOrder(index: number) {
    orderNodes.value.splice(index, 1);
    if (orderNodes.value.length == 0) orderNodes.value.push({ column: "id", isASC: true });
}
function resetOrder() {
    orderNodes.value = [{
        column: "id",
        isASC: true
    }];
}
function addOrder() {
    orderNodes.value.push({
        column: "id",
        isASC: true
    });
}

const opTable = {
    "SQLOperator::EQUAL": "=",
    "SQLOperator::NOTEQUAL": "!=",
    "SQLOperator::GREATER": ">",
    "SQLOperator::GREATER_OR_EQUAL": ">=",
    "SQLOperator::SMALLER": "<",
    "SQLOperator::SMALLER_OR_EQUAL": "<=",
    "SQLOperator::LIKE": "LIKE",
    "SQLOperator::NOTLIKE": "NOT LIKE",
    "SQLOperator::REGEXP": "REGEXP",
    "SQLOperator::NOTREGEXP": "NOT REGEXP",
};
function quote_encode(source: string) {
    if (source == null) return "";
    return source.replace(/\\/g, "\\\\").replace(/\"/g, "\\\"").replace(/\n/g, "\\n");
}
function whereToString(where: WhereNode) {
    if (where.isLeaf) {
        var column = columnTitle.value[where.column!];
        var op = opTable[where.op as keyof typeof opTable];
        var value = columnType.value[where.column!] == 'RecordPropertyType::NUMBER' ? where.value : `"${ quote_encode(where.value!) }"`;
        return column + ' ' + op + ' ' + value;
    } else {
        var strings: Array<string> = [];
        for (var i = 0; i < where.params!.length; i++) strings.push(whereToString(where.params![i]!));
        return "(" + (where.params!.length ? strings.join(where.isAnd ? " AND " : " OR ") : "TRUE") + ")";
    }
}
function getWhereCount(where: WhereNode): number {
    if (where.isLeaf) return 1;
    var cnt = 0;
    for (var i = 0; i < where.params!.length; i++) cnt += getWhereCount(where.params![i]!);
    return cnt;
}
watchEffect(() => {
    whereString.value = "WHERE " + whereToString(whereNodes.value);
    whereCount.value = getWhereCount(whereNodes.value);

    var strings: Array<string> = [];
    for (var i = 0; i < orderNodes.value.length; i++) {
        var column = columnTitle.value[orderNodes.value[i]?.column!];
        strings.push(column + ' ' + (orderNodes.value[i]?.isASC ? 'ASC' : 'DESC'));
    }
    orderString.value = "ORDER BY " + strings.join(", "); 
    orderCount.value = orderNodes.value.length;
});

// ImageOverlay 模块
const imageUrls: Ref<Array<string>> = ref([]);
const imageModel = ref(false);
const imageIndex = ref(0);
const imageUrl = ref("");
const imageEnableLeft = ref(false);
const imageEnableRight = ref(false);
const imageColumns: Ref<Record<number, number> > = ref({});
const imageOpacity = ref(1);
const imageTitles: Ref<Array<string> > = ref([]);
const imageTitle = ref("");
function findNextImage(index: number) {
    for (var i = index + 1; i < imageUrls.value.length; i++) {
        if (!imageUrls.value[i]?.endsWith("/images/")) return i;
    }
    return -1;
}
function findPreviousImage(index: number) {
    for (var i = index - 1; i >= 0; i--) {
        if (!imageUrls.value[i]?.endsWith("/images/")) return i;
    }
    return -1;
}
function showImage(index: number, fadeIn = true) {
    if (fadeIn) { 
        imageEnableLeft.value = false;
        imageEnableRight.value = false;
        imageOpacity.value = 0;
        imageIndex.value = index;
        setTimeout(() => {
            imageUrl.value = imageUrls.value[index]!;
            imageOpacity.value = 1;
            imageTitle.value = imageTitles.value[index]!;
        }, 280);
        setTimeout(() => {
            imageEnableLeft.value = findPreviousImage(index) != -1;
            imageEnableRight.value = findNextImage(index) != -1;
        }, 560);
    } else {
        imageIndex.value = index;
        imageUrl.value = imageUrls.value[index]!;
        imageTitle.value = imageTitles.value[index]!;
        imageModel.value = true;
        imageEnableLeft.value = findPreviousImage(index) != -1;
        imageEnableRight.value = findNextImage(index) != -1;
    }
}

// TableCell 模块
const editDialog = ref(false);
const editId = ref(0);
const editValues: Ref<Record<string, string>> = ref({});
function enableClick(prop: RecordProperty) {
    return prop.type == "RecordPropertyType::IMAGE";
}
function onclick(prop: RecordProperty, row: number, column: number) {
    if (prop.type == "RecordPropertyType::IMAGE") {
        var index = row * (imageUrls.value.length / data.value.length) + imageColumns.value[column]!;
        showImage(index, false);
    }
}
function edit(index: number) {
    var item = data.value[index]!;
    if (crop.crop.permission != 'UserPermission::OWNER' && Number(item.uid) != userId.value) return;
    for (var i = 0; i < crop.crop.properties?.length!; i++) {
        var prop = crop.crop.properties![i]!;
        editValues.value[prop.name!] = item[prop.name!]!.toString();
    }
    editId.value = Number(item.id!);
    editDialog.value = true;
}
async function submitEdit() {
    for (var i = 0; i < crop.crop.properties!.length; i++) {
        var prop = crop.crop.properties![i]!;
        if (prop.required) {
            if (prop.type == "RecordPropertyType::MULTI") {
                if (JSON.parse(editValues.value[prop.name!]!).length == 0) {
                    showMsg(MessageType.Error, `${prop.title}不能为空`);
                    return;
                }
            } else {
                if (editValues.value[prop.name!] == "") {
                    showMsg(MessageType.Error, `${prop.title}不能为空`);
                    return;
                }
            }
        }
    }
    await (await newFetch(`${API_BASE_URL}/crops/${crop.crop.cid}/records/${editId.value}/edit`, {
        method: "POST",
        body: JSON.stringify(editValues.value)
    })).json();
    showMsg(MessageType.Success, "记录修改成功");
    editDialog.value = false;
    await sleep(1000);
    await load();
}
async function removeRecord() {
    if (!confirm('确定要删除吗？')) return;
    await (await newFetch(`${API_BASE_URL}/crops/${crop.crop.cid}/records/${editId.value}/remove`, {
        method: 'POST'
    })).json();
    showMsg(MessageType.Success, '记录删除成功');
    editDialog.value = false;
    await sleep(1000);
    await load();
}

// 数据加载模块
async function load() {
    loading.value = true;
    var res = await(await newFetch(`${API_BASE_URL}/crops/${crop.crop.cid}/records/list`, {
        method: "POST",
        body: JSON.stringify({
            where: whereNodes.value,
            order: orderNodes.value,
            limit: itemsPerPage.value,
            offset: 0
        })
    })).json();

    itemLength.value = res.count;
    data.value = res.items;
    imageUrls.value = [];
    for (var i = 0; i < res.items.length; i++) {
        var item = res.items[i]!;
        var cnt = 0;
        for (var j = 0; j < crop.crop.properties?.length!; j++) {
            var prop = crop.crop.properties![j]!;
            if (prop.type == "RecordPropertyType::IMAGE") {
                imageUrls.value.push(`${API_BASE_URL}/crops/${crop.crop.cid}/images/${item[prop.name!]!}`);
                imageTitles.value.push(`#${item.id} - ${crop.crop.properties![j]?.title} | ${item[prop.name!]!}.jpg`);
                imageColumns.value[j] = cnt++;
            }
        }
    }

    showMsg(MessageType.Success, `查询完成，找到 ${res.count} 条记录`);
    var query = JSON.parse(localStorage.getItem("queryCache") ?? "{}");
    query[crop.crop.cid!] = {
        where: whereNodes.value,
        order: orderNodes.value
    };
    localStorage.setItem("queryCache", JSON.stringify(query));
    loading.value = false;
}
onBeforeMount(async () => {
    var query = JSON.parse(localStorage.getItem("queryCache") ?? "{}");
    whereNodes.value = query[crop.crop.cid!]?.where ?? { isLeaf: false, isAnd: true, params: [] };
    orderNodes.value = query[crop.crop.cid!]?.order ?? [{ column: "id", isASC: true }];

    columnTitle.value["id"] = "编号";
    columnType.value["id"] = "RecordPropertyType::NUMBER";
    for (var i = 0; i < crop.crop.properties?.length!; i++) {
        var prop = crop.crop.properties![i]!;
        headers.value.push({
            title: prop.title + (prop.unit ? "（" + prop.unit + "）" : ""),
            value: prop.name
        });
        columnTitle.value[prop.name!] = prop.title + (prop.unit ? "（" + prop.unit + "）" : "");
        columnType.value[prop.name!] = prop.type!;
    }
    await load();
});

// 数据导出模块
const exportDialog = ref(false);
function exportResults() {
    exportDialog.value = true;
}
async function getFullData(callback = (_: Array<Record<string, string>>) => {}) {
    var res = await (await newFetch(`${API_BASE_URL}/crops/${crop.crop.cid}/records/list`, {
        method: "POST",
        body: JSON.stringify({
            where: whereNodes.value,
            order: [{ column: "id", isASC: true }],
            limit: itemLength.value,
            offset: 0
        })
    })).json();
    callback(res.items);
}
</script>

<template>
    <v-expansion-panels static bg-color="transparent" v-model="filterModel">
        <v-expansion-panel value="0">
            <v-expansion-panel-title class="pa-4 MyExpansionPanelTitle">
                <div class="d-flex align-center ga-2">
                    <h2 class="ma-0">
                        筛选器
                        <span v-if="filterModel == undefined">& 排序方式</span>
                    </h2>
                    <v-badge location="top right" color="primary" :content="whereCount" v-if="whereCount > 0">
                        <v-icon icon="$mdiFilterVariant"></v-icon>
                    </v-badge>
                    <v-badge location="top right" color="primary" :content="orderCount" v-if="orderCount > 0 && filterModel == undefined">
                        <v-icon icon="$mdiOrderAlphabeticalAscending"></v-icon>
                    </v-badge>
                </div>
            </v-expansion-panel-title>
            <v-expansion-panel-text class="MyExpansionPanelText pa-4 pt-0">
                <v-list class="mb-4">
                    <RecordWhere
                        :where="whereNodes"
                        :title="columnTitle"
                        :type="columnType"
                        root
                    ></RecordWhere>
                </v-list>
                <v-textarea
                    v-model="whereString"
                    label="WHERE 子句"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    class="mb-4 Textarea"
                    auto-grow
                    disabled
                ></v-textarea>
                <div class="d-flex align-center justify-space-between mb-4">
                    <v-btn prepend-icon="$mdiRefresh" color="error" @click="resetWhere()">重置筛选</v-btn>
                    <div class="d-flex align-center ga-2">
                        <v-btn prepend-icon="$mdiPlus" color="primary" @click="addWhereNode()">逻辑运算</v-btn>
                        <v-btn prepend-icon="$mdiPlus" color="primary" @click="addWhereLeaf()">比较运算</v-btn>
                    </div>
                </div>
                <div class="d-flex align-center ga-2 mb-4">
                    <h2 class="ma-0">排序方式</h2>
                    <v-badge location="top right" color="primary" :content="orderCount" v-if="orderCount > 0">
                        <v-icon icon="$mdiOrderAlphabeticalAscending"></v-icon>
                    </v-badge>
                </div>
                <v-list class="mb-4" v-if="orderNodes.length">
                    <draggable v-model="orderNodes" animation="200" item-key="name">
                        <template v-slot:item="{ element, index }">
                            <RecordOrder 
                                :order="element" 
                                :title="columnTitle"
                                :type="columnType"
                                @update="(order) => updateOrder(index, order)"
                                @remove="removeOrder(index)"
                            ></RecordOrder>
                        </template>
                    </draggable>
                </v-list>
                <v-textarea
                    v-model="orderString"
                    label="ORDER 子句"
                    variant="outlined"
                    density="comfortable"
                    hide-details
                    class="mb-4 Textarea"
                    auto-grow
                    disabled
                ></v-textarea>
                <div class="d-flex align-center justify-space-between">
                    <v-btn prepend-icon="$mdiRefresh" color="error" @click="resetOrder()">重置排序</v-btn>
                    <v-btn prepend-icon="$mdiPlus" color="primary" @click="addOrder()">添加排序</v-btn>
                </div>
            </v-expansion-panel-text>
        </v-expansion-panel>
    </v-expansion-panels>
    <v-divider></v-divider>
    <div class="d-flex align-center justify-space-between pa-4">
        <h2 class="ma-0">查询结果</h2>
        <div class="d-flex align-center ga-2">
            <v-btn prepend-icon="$mdiExport" color="primary" @click="exportResults()">导出结果</v-btn>
            <v-btn prepend-icon="$mdiMagnify" color="primary" @click="load()">点击查询</v-btn>
        </div>
    </div>
    <v-data-table-server
        v-model:items-per-page="itemsPerPage"
        :headers="headers"
        :items="data"
        :items-length="itemLength"
        :loading="loading"
        :items-per-page-options="[
            { value: 10, title: '10' },
            { value: 20, title: '20' },
            { value: 30, title: '30' },
            { value: 50, title: '50' },
            { value: 100, title: '100' }
        ]"
        items-per-page-text="每页记录数："
        @update:options="load()"
        class="mb-4"
        hover
    >
        <template v-slot:headers="{ columns }">
            <tr>
                <th nowrap>编号</th>
                <template v-for="column in columns">
                    <th nowrap>
                        <div class="d-flex align-center">
                            <span>{{ column.title }}</span>
                        </div>
                    </th>
                </template>
            </tr>
        </template>
        <template v-slot:item="{ index, item }">
            <tr class="text-no-wrap" @click="edit(index)">
                <td align="center">#{{ item.id }}</td>
                <template v-for="(prop, j) in crop.crop.properties">
                    <RecordValue 
                        :cid="crop.crop.cid!"
                        :property="prop"
                        :value="item[prop.name!]!"
                        :enableClick="enableClick(prop)"
                        @click="onclick(prop, index, j)"
                    ></RecordValue>
                </template>
            </tr>
        </template>
    </v-data-table-server>
    <ImageOverlay 
        v-model:model="imageModel" 
        :src="imageUrl" 
        :enableLeft="imageEnableLeft" 
        :enableRight="imageEnableRight"
        :opacity="imageOpacity"
        :title="imageTitle"
        @click:left="showImage(findPreviousImage(imageIndex))"
        @click:right="showImage(findNextImage(imageIndex))"
    ></ImageOverlay>
    <RecordEditDialog
        v-model:open="editDialog"
        v-model:values="editValues"
        :properties="crop.crop.properties!"
        :cid="crop.crop.cid!"
        @submit="submitEdit()"
        @remove="removeRecord()"
    ></RecordEditDialog>
    <RecordExportDialog
        v-model:open="exportDialog"
        :properties="crop.crop.properties!"
        :cid="crop.crop.cid!"
        @getData="getFullData"
    ></RecordExportDialog>
</template>

<style lang="css" scoped>
.MyExpansionPanelTitle h2 {
    font-size: 24px;
    height: 36px;
    line-height: 36px;
}

.Textarea {
    font-family: 'Cascadia Mono', 'Consolas'
}
</style>

<style lang="css">
.MyExpansionPanelTitle > .v-expansion-panel-title__overlay {
    opacity: 0!important;
}
.MyExpansionPanelText > .v-expansion-panel-text__wrapper {
    padding: 0;
}
</style>