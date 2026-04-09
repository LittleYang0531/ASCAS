<script setup lang="ts">
import { onMounted, ref, watch } from 'vue';
import type { RecordProperty } from '../../../models/crop';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });

const geometry = ref("");
const geometryLoaded = ref(true);

function loadGeolocation() {
    geometryLoaded.value = false;
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition((pos: GeolocationPosition) => {
            var latitude = pos.coords.latitude;
            var longitude = pos.coords.longitude;
            var accuracy = pos.coords.accuracy;
            geometry.value = `( lat=${latitude}°, lon=${longitude}°, acc=${accuracy}m )`;
            model.value = geometry.value;
            geometryLoaded.value = true;
        }, (err: GeolocationPositionError) => {
            showMsg(MessageType.Error, `获取地理位置失败（${err.code}）：${err.message}`);
            geometryLoaded.value = true;
        }, {
            enableHighAccuracy: true,
            maximumAge: 0
        });
    } else {
        showMsg(MessageType.Error, "浏览器不支持获取地理位置，请使用支持的浏览器！");
        geometryLoaded.value = true;
    }
}

watch(model, (val) => {
    if (val == "") loadGeolocation();
});

onMounted(() => {
    if (model.value == "") loadGeolocation();
});
</script>

<template>
    <v-text-field
        v-model="model"
        variant="outlined"
        density="comfortable"
        hide-details
        :disabled="true"
        :loading="!geometryLoaded"
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
    </v-text-field>
    <v-btn
        icon="$mdiRefresh"
        color="primary"
        :disabled="!geometryLoaded"
        @click="loadGeolocation()"
    ></v-btn>
</template>