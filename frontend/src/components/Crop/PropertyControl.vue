<script lang="ts" setup>
import { onBeforeMount, ref } from 'vue';
import type { RecordProperty } from '../../models/crop';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import VOutlined from '../VOutlined.vue';
import { newFetch } from '../../utils/fetch';
import { API_BASE_URL } from '../../config';
import ImageOverlay from '../ImageOverlay.vue';
import { isJSON } from '../../utils/json';

const label = defineProps<{ props: RecordProperty, label: string, class: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
const geometry = ref("");
const geometryLoaded = ref(false);
const isDragging = ref(false);
const imageProperty = ref({
    isCropped: false,
    width: 0,
    height: 0,
    size: 0
});
const imagePreview = ref(false);

async function uploadImage(file: string) {
    if (label.cropId == undefined) {
        showMsg(MessageType.Error, "内部错误：缺少作物表 id");
        return;
    }
    file = file.substring(file.indexOf(",") + 1);
    var hash = await (await newFetch(`${API_BASE_URL}/crops/${label.cropId}/images`, {
        method: "POST",
        body: file
    })).json();
    model.value = hash["hash"];
    showMsg(MessageType.Success, "图片上传成功");
}
function solveImage(file: File) {
    var canvas = document.createElement("canvas");
    var ctx = canvas.getContext("2d")!;
    var img = new Image();
    img.onload = function() {
        var scale = Math.sqrt(Math.min(1, 2048 * 1024 / 2 / img.width / img.height));

        canvas.width = Math.ceil(img.width * scale);
        canvas.height = Math.ceil(img.height * scale);
        ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
        canvas.toBlob(function(blob) {
            if (blob) {
                var pngBase64 = canvas.toDataURL("image/png");
                imageProperty.value = {
                    isCropped: scale < 1,
                    width: canvas.width,
                    height: canvas.height,
                    size: blob.size
                };
                uploadImage(pngBase64);
            } else {
                showMsg(MessageType.Error, "图片转换失败");
            }
        });
    };
    img.src = URL.createObjectURL(file);
}
function clickEvent() {
    if (model.value != "") {
        imagePreview.value = true;
    } else {
        const input = document.createElement("input");
        input.type = "file";
        input.accept = "image/*";
        input.onchange = function() {
            if (input.files && input.files.length > 0) {
                const file = input.files[0]!;
                solveImage(file);
            }
        };
        input.click();
    }
}
function dropEvent(e: DragEvent) {
    if (model.value != "") return;
    e.preventDefault();
    e.stopPropagation();
    isDragging.value = false;

    if (e.dataTransfer?.files.length != 1) {
        showMsg(MessageType.Error, "只能上传一个文件");
        return;
    }

    const file = e.dataTransfer.files[0]!;
    if (!file.type.startsWith("image/")) {
        showMsg(MessageType.Error, "只能上传图片文件");
        return;
    }

    solveImage(file);
}
function dragEnterEvent(e: DragEvent) {
    if (model.value != "") return;
    e.preventDefault();
    isDragging.value = true;
}
function dragLeaveEvent(e: DragEvent) {
    if (model.value != "") return;
    e.preventDefault();
    isDragging.value = false;
}
function clearImage(e: Event) {
    e.stopPropagation();
    model.value = "";
    imageProperty.value = {
        isCropped: false,
        width: 0,
        height: 0,
        size: 0
    };
}

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
            enableHighAccuracy: true, // 提高精度
            maximumAge: 0 // 不使用缓存位置
        });
    } else {
        showMsg(MessageType.Error, "浏览器不支持获取地理位置，请使用支持的浏览器！");
        geometryLoaded.value = true;
    }
}

onBeforeMount(() => {
    if (label.props.type == "RecordPropertyType::NUMBER") {
        if (model.value == "") model.value = "0";
    }
    if (label.props.type == "RecordPropertyType::GEOMETRY") loadGeolocation();
});
</script>

<template>
    <div 
        :class="`d-flex align-center justify-center ga-2 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::NUMBER'"
    >
        <v-number-input
            :model-value="Number(model)"
            variant="outlined"
            density="comfortable"
            hide-details
            @update:model-value="(val) => model = val.toString()"
            :precision="null"
            :disabled="label.disabled"
        >
            <template v-slot:label>
                <span>{{ label.label }}</span>
                <span v-if="label.props.required" style="color: red">&nbsp;*</span>
            </template>
        </v-number-input>
        <p
            v-if="label.props.unit != ''"
            class="ma-0"
        >（{{ label.props.unit }}）</p>
    </div>

    <div 
        :class="`d-flex align-center justify-center ga-2 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::STRING'"
    >
        <v-text-field
            v-model="model"
            variant="outlined"
            density="comfortable"
            hide-details
            :disabled="label.disabled"
            clearable
        >
            <template v-slot:label>
                <span>{{ label.label }}</span>
                <span v-if="label.props.required" style="color: red">&nbsp;*</span>
            </template>
        </v-text-field>
        <p
            v-if="label.props.unit != ''"
            class="ma-0"
        >（{{ label.props.unit }}）</p>
    </div>

    <div 
        :class="`d-flex align-center justify-center ga-2 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::SELECT'"
    >
        <v-select
            v-model="model"
            variant="outlined"
            density="comfortable"
            hide-details
            :items="label.props.options"
            @update:model-value="(val) => model = val.toString()"
            :disabled="label.disabled"
        >
            <template v-slot:label>
                <span>{{ label.label }}</span>
                <span v-if="label.props.required" style="color: red">&nbsp;*</span>
            </template>
        </v-select>
        <p
            v-if="label.props.unit != ''"
            class="ma-0"
        >（{{ label.props.unit }}）</p>
    </div>
    
    <div 
        :class="`d-flex align-center justify-center ga-2 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::MULTI'"
    >
        <v-select
            :model-value="!isJSON(model!) ? [] : JSON.parse(model!)"
            variant="outlined"
            density="comfortable"
            hide-details
            multiple
            :items="label.props.options"
            @update:model-value="(val) => model = JSON.stringify(val.sort())"
            :disabled="label.disabled"
        >
            <template v-slot:label>
                <span>{{ label.label }}</span>
                <span v-if="label.props.required" style="color: red">&nbsp;*</span>
            </template>
        </v-select>
        <p
            v-if="label.props.unit != ''"
            class="ma-0"
        >（{{ label.props.unit }}）</p>
    </div>

    <div 
        :class="`d-flex align-center justify-center ga-4 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::GEOMETRY'"
    >
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
    </div>

    <div 
        :class="`d-flex align-center justify-center ga-4 ${label.class}`"
        v-if="label.props.type == 'RecordPropertyType::IMAGE'"
    >
        <VOutlined 
            :class="`myHoverOutlined cursor-pointer ${isDragging ? 'hovering' : ''}`"
            @dragover="dragEnterEvent"
            @dragenter.prevent
            @dragleave="dragLeaveEvent"
            @drop="dropEvent"
            @click="clickEvent"
        >
            <template v-slot:label>
                <span>{{ label.label }}</span>
                <span v-if="label.props.required" style="color: red">&nbsp;*</span>
            </template>
            <div 
                class="d-flex align-center justify-center ga-1" 
                style="width: 100%; height: 32px;" 
                v-if="model == ''"
            >
                <v-icon icon="$mdiCloudUpload" color="primary"></v-icon>
                <span class="text-medium-emphasis">尚未上传图片。请点击选择，或将图片拖拽到此处</span>
            </div>
            <div class="d-flex align-center justify-space-between" style="width: 100%; height: 32px; padding: 0 6px" v-else>
                <div class="d-flex align-center ga-1">
                    <v-icon icon="$mdiImage" color="primary"></v-icon>
                    <span class="text-medium-emphasis">{{ model }}.png</span>
                    <span class="text-medium-emphasis">
                        ({{ Math.round(imageProperty.size / 10.24) / 100.0 }}kb{{ imageProperty.isCropped ? `，已裁剪至 ${imageProperty.width}x${imageProperty.height}` : "" }})
                    </span>
                </div>
                <v-icon
                    icon="$mdiCloseCircle"
                    class="text-medium-emphasis clearButton"
                    @click="clearImage"
                ></v-icon>
            </div>
        </VOutlined>
        <ImageOverlay v-model:model="imagePreview" :src="`${API_BASE_URL}/crops/${label.cropId}/images/${model}`"></ImageOverlay>
    </div>
</template>

<style lang="css" scoped>
.clearButton {
    display: none;
}
</style>

<style lang="css">
@media (hover: hover) {
    .myHoverOutlined.v-field:hover .v-field__outline {
        --v-field-border-opacity: var(--v-high-emphasis-opacity);
    }
    .myHoverOutlined.v-field:hover .clearButton {
        display: block;
    }
}
.hovering .v-field__outline {
    --v-field-border-opacity: var(--v-high-emphasis-opacity)!important;
}
</style>