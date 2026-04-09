<script lang="ts" setup>
import type { RecordProperty } from '../../models/crop';
import NumberControl from './Number/Control.vue';
import StringControl from './String/Control.vue';
import SelectControl from './Select/Control.vue';
import MultiControl from './Multi/Control.vue';
import GeometryControl from './Geometry/Control.vue';
import QrcodeControl from './Qrcode/Control.vue';
import DateControl from './Date/Control.vue';
import ImageControl from './Image/Control.vue';

const label = defineProps<{ props: RecordProperty, label: string, class: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
const controls = {
    "RecordPropertyType::NUMBER": NumberControl,
    "RecordPropertyType::STRING": StringControl,
    "RecordPropertyType::SELECT": SelectControl,
    "RecordPropertyType::MULTI": MultiControl,
    "RecordPropertyType::GEOMETRY": GeometryControl,
    "RecordPropertyType::QRCODE": QrcodeControl,
    "RecordPropertyType::DATE": DateControl,
    "RecordPropertyType::IMAGE": ImageControl
}
</script>

<template>
    <div :class="`d-flex align-center justify-center ga-2 ${label.class}`">
        <component
            :is="controls[label.props.type as keyof typeof controls]"
            v-model:model="model"
            :label="label.label"
            :props="label.props"
            :disabled="label.disabled"
            :cropId="label.cropId"
        ></component>
    </div>
</template>