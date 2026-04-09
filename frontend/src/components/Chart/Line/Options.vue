<script setup lang="ts">
import { onMounted } from 'vue';
import type { RecordProperty } from '../../../models/crop';

const model = defineModel<Record<string, unknown> >('model', { required: true });
const props = defineProps<{
    properties: Array<RecordProperty>
}>();

function propFilterX(p: RecordProperty) {
    return p.type == 'RecordPropertyType::DATE';
}

function propFilterY(p: RecordProperty) {
    return p.type == 'RecordPropertyType::NUMBER';
}

function propFilterType(p: RecordProperty) {
    return p.type == 'RecordPropertyType::SELECT' || p.type == 'RecordPropertyType::MULTI';
}

onMounted(() => {
    if (!model.value['line.columnX']) {
        var prop = props.properties.find(propFilterX);
        if (prop) model.value['line.columnX'] = prop.name;
    }
    if (!model.value['line.columnY']) {
        var prop = props.properties.find(propFilterY);
        if (prop) model.value['line.columnY'] = prop.name;
    }
    if (!model.value['line.unit']) {
        model.value['line.unit'] = '%Y-%m-%d';
    }
    if (!model.value['line.aggregation']) {
        model.value['line.aggregation'] = 'avg';
    }
});
</script>

<template>
    <v-select
        class="mt-4"
        v-model="model['line.columnX']"
        :items="properties.filter(propFilterX).map((p) => ({ title: p.title, value: p.name }))"
        label="选择X轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.columnY']"
        :items="properties.filter(propFilterY).map((p) => ({ title: p.title, value: p.name }))"
        label="选择Y轴"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.columnType']"
        :items="properties.filter(propFilterType).map((p) => ({ title: p.title, value: p.name }))"
        label="选择分组（可选）"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
        clearable
    ></v-select>
    <v-select
        class="mt-4"
        v-model="model['line.aggregation']"
        :items="[
            { title: '平均值', value: 'avg' },
            { title: '总和', value: 'sum' },
            { title: '最大值', value: 'max' },
            { title: '最小值', value: 'min' },
            { title: '计数', value: 'count' }
        ]"
        label="选择聚合方式"
        variant="outlined"
        density="comfortable"
        hide-details
        focused
    ></v-select>
    <v-tooltip
        location="bottom"
        origin="auto"
        no-click-animation
    >
        <template v-slot:activator="{ props }">
            <v-text-field
                class="mt-4"
                v-model="model['line.unit']"
                label="X轴格式*"
                variant="outlined"
                density="comfortable"
                v-bind="props"
                hide-details
            ></v-text-field>
        </template>

        <div class="d-flex flex-column full-width" style="font-family: 'Cascadia Mono', monospace;">
            <span>使用时间格式字符串，参见 <a href="https://cppreference.cn/w/cpp/chrono/c/strftime" target="_blank">https://cppreference.cn/w/cpp/chrono/c/strftime</a></span>
            <span>支持以下占位符：</span>
            <span>%a, %A: 写入星期几名称，例如周二</span>
            <span>%b, %B: 写入月份名称，例如一月</span>
            <span>%c: 写入标准日期和时间字符串，例：2023-01-01 周二 12:00:00</span>
            <span>%d: 将月份中的天数写入为十进制数字（范围 [01,31]）</span>
            <span>%H: 将小时写入为十进制数字，24 小时制（范围 [00-23]）</span>
            <span>%I: 将小时写入为十进制数字，12 小时制（范围 [01,12]）</span>
            <span>%m: 将月份写入为十进制数字（范围 [01,12]）</span>
            <span>%M: 将分钟写入为十进制数字（范围 [00,59]）</span>
            <span>%p: 写入本地化 AM 或 PM</span>
            <span>%S: 将秒写入为十进制数字（范围 [00,60]）</span>
            <span>%Y: 将年份写入为十进制数字，例如 2017</span>
            <span style="color: red">暂不支持转换说明符 %% </span>
        </div>
    </v-tooltip>
</template>