<script setup lang="ts">
import type { RecordProperty } from '../../models/crop';
import NumberValue from './Number/Value.vue';
import StringValue from './String/Value.vue';
import SelectValue from './Select/Value.vue';
import MultiValue from './Multi/Value.vue';
import GeometryValue from './Geometry/Value.vue';
import QrcodeValue from './Qrcode/Value.vue';
import DateValue from './Date/Value.vue';
import SensorValue from './Sensor/Value.vue';
import ImageValue from './Image/Value.vue';

const item = defineProps<{
    cid: number,
    property: RecordProperty,
    value: string | number
    enableClick: boolean
}>();
const emits = defineEmits<{
    (e: 'click'): void
}>();

const values = {
    "RecordPropertyType::NUMBER": NumberValue,
    "RecordPropertyType::STRING": StringValue,
    "RecordPropertyType::SELECT": SelectValue,
    "RecordPropertyType::MULTI": MultiValue,
    "RecordPropertyType::GEOMETRY": GeometryValue,
    "RecordPropertyType::QRCODE": QrcodeValue,
    "RecordPropertyType::DATE": DateValue,
    "RecordPropertyType::SENSOR": SensorValue,
    "RecordPropertyType::IMAGE": ImageValue
}
</script>

<template>
    <td>
        <component
            :is="values[item.property.type as keyof typeof values]"
            :cid="item.cid"
            :property="item.property"
            :value="item.value"
            :enableClick="item.enableClick"
            @click="() => { item.enableClick && emits('click'); }"
        ></component>
    </td>
</template>