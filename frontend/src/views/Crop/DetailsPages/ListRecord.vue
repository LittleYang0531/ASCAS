<script setup lang="ts">
import { onBeforeMount, ref, type Ref } from 'vue';
import type { Crop, RecordProperty } from '../../../models/crop';
import type { OrderNode, WhereNode } from '../../../models/record';
import { API_BASE_URL } from '../../../config';
import { newFetch } from '../../../utils/fetch';
import RecordValue from '../../../components/Record/RecordValue.vue';
import ImageOverlay from '../../../components/ImageOverlay.vue';
import draggable from 'vuedraggable';
import RecordOrder from '../../../components/Record/RecordOrder.vue';
import RecordWhere from '../../../components/Record/RecordWhere.vue';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';

const crop = defineProps<{
    crop: Crop
}>();
const itemLength = ref(0);
const data: Ref<Array<Record<string, string>>> = ref([]);
const itemsPerPage = ref(10);
const loading = ref(false);
const headers: Ref<Array<Object>>  = ref([]);

// 数据筛选器模块
const columnTitle: Ref<Record<string, string>> = ref({});
const whereNodes: Ref<WhereNode> = ref({ isLeaf: false, isAnd: true, params: [] });
const orderNodes: Ref<Array<OrderNode> > = ref([{
    column: "id",
    isASC: true
}]);
function resetWhere() {
    whereNodes.value = { isLeaf: false, isAnd: true, params: [] };
}
function addWhere() {
    showMsg(MessageType.Info, "功能开发中...");
    // (whereNodes.value as any).params!.push({ 
    //     isLeaf: true, 
    //     column: "id", 
    //     operator: "SQLOperator::EQUAL", 
    //     value: "" 
    // });
}
function updateOrder(oldOrder: OrderNode, order: OrderNode) {
    const index = orderNodes.value.findIndex((o) => o.column == oldOrder.column);
    if (index != -1) orderNodes.value[index] = order;
}
function removeOrder(order: OrderNode) {
    const index = orderNodes.value.findIndex((o) => o.column == order.column);
    if (index != -1) orderNodes.value.splice(index, 1);
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
function enableClick(prop: RecordProperty) {
    return prop.type == "RecordPropertyType::IMAGE";
}
function onclick(prop: RecordProperty, row: number, column: number) {
    if (prop.type == "RecordPropertyType::IMAGE") {
        var index = row * (imageUrls.value.length / data.value.length) + imageColumns.value[column]!;
        showImage(index, false);
    }
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
    loading.value = false;
}
onBeforeMount(async () => {
    columnTitle.value["id"] = "编号";
    for (var i = 0; i < crop.crop.properties?.length!; i++) {
        var prop = crop.crop.properties![i]!;
        headers.value.push({
            title: prop.title + (prop.unit ? "（" + prop.unit + "）" : ""),
            value: prop.name
        });
        columnTitle.value[prop.name!] = prop.title!;
    }
    await load();
});

// 数据导出模块
function exportResults() {
    showMsg(MessageType.Info, "功能开发中...");
}
</script>

<template>
    <div class="pa-4">
        <h2 class="ma-0 mb-4">筛选器</h2>
        <v-list class="mb-4">
            <RecordWhere
                :where="whereNodes"
            ></RecordWhere>
        </v-list>
        <div class="d-flex align-center justify-space-between mb-4">
            <v-btn prepend-icon="$mdiRefresh" color="error" @click="resetWhere()">重置筛选</v-btn>
            <v-btn prepend-icon="$mdiPlus" color="primary" @click="addWhere()">添加筛选</v-btn>
        </div>
        <h2 class="ma-0 mb-4">排序方式</h2>
        <v-list class="mb-4" v-if="orderNodes.length">
            <draggable v-model="orderNodes" item-key="name">
                <template #item="{ element }">
                    <RecordOrder 
                        :order="element" 
                        :title="columnTitle"
                        @update="(order) => updateOrder(element, order)"
                        @remove="removeOrder(element)"
                    ></RecordOrder>
                </template>
            </draggable>
        </v-list>
        <div class="d-flex align-center justify-space-between">
            <v-btn prepend-icon="$mdiRefresh" color="error" @click="resetOrder()">重置排序</v-btn>
            <v-btn prepend-icon="$mdiPlus" color="primary" @click="addOrder()">添加排序</v-btn>
        </div>
    </div>
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
            <tr class="text-no-wrap">
                <td align="center">#{{ index + 1 }}</td>
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
</template>