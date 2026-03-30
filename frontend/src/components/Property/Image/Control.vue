<script setup lang="ts">
// (model = '') => (solvingImage) => (uploadingImage) => (model != '')

import { ref } from 'vue';
import CameraOverlay from '../../Overlay/CameraOverlay.vue';
import ImageOverlay from '../../Overlay/ImageOverlay.vue';
import VOutlined from '../../VOutlined.vue';
import { useDisplay } from 'vuetify';
import { showMsg } from '../../../utils/message';
import { MessageType } from '../../../models/message';
import type { RecordProperty } from '../../../models/crop';
import { API_BASE_URL, imageMaxSize } from '../../../config';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });
const isDragging = ref(false);
const imageProperty = ref({
    isCropped: false,
    width: 0,
    height: 0,
    size: 0
});
const solvingImage = ref(false);
const uploadingImage = ref(false);
const uploaded = ref(0);
const uploadTotal = ref(1);
const uploadSpeed = ref(0);
const imagePreview = ref(false);
const cameraPreview = ref(false);
const mobile = useDisplay().mobile;

async function uploadImage(file: string) {
    uploaded.value = 0, uploadTotal.value = 1;
    solvingImage.value = false;
    uploadingImage.value = true;
    if (label.cropId == undefined) {
        showMsg(MessageType.Error, "内部错误：缺少作物表 id");
        uploadingImage.value = false;
        return;
    }
    file = file.substring(file.indexOf(",") + 1);
    return new Promise((resolve, reject) => {
        reject;
        const xhr = new XMLHttpRequest();
        xhr.addEventListener("readystatechange", () => {
            if (xhr.readyState === xhr.DONE) {
                var hash = JSON.parse(xhr.responseText);
                model.value = hash["hash"];
                showMsg(MessageType.Success, "图片上传成功");
                uploadingImage.value = false;
                resolve({});
            }
        });
        var lastTime = new Date().getTime();
        var lastUploaded = 0;
        xhr.upload.addEventListener("progress", (e: any) => {
            var loaded: number = e.loaded;
            var total: number = e.total;
            uploaded.value = loaded;
            uploadTotal.value = total;
            var currTime = new Date().getTime();
            uploadSpeed.value = (loaded - lastUploaded) / (currTime - lastTime) * 1000;
            lastTime = currTime;
            lastUploaded = loaded;
        });
        xhr.open("POST", `${API_BASE_URL}/crops/${label.cropId}/images`);
        xhr.setRequestHeader("Authorization", "SessionToken " + document.cookie.substr(document.cookie.indexOf("session=") + 8));
        xhr.send(file);
    });
}

async function cropImage(file: File, scale: number) {
    return new Promise((resolve, reject) => {
        reject;
        var canvas = document.createElement("canvas");
        var ctx = canvas.getContext("2d")!;
        var img = new Image();
        img.onload = function() {
            canvas.width = Math.ceil(img.width * scale);
            canvas.height = Math.ceil(img.height * scale);
            ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
            canvas.toBlob(function(blob) {
                return resolve({
                    blob: blob,
                    width: canvas.width,
                    height: canvas.height
                });
            }, "image/jpeg", 0.7);
        };
        img.src = URL.createObjectURL(file);
    });
}
async function blobToBase64(blob: Blob) {
    return new Promise<string>((resolve, reject) => {
        var reader = new FileReader();
        reader.onloadend = function() {
            resolve(reader.result as string);
        };
        reader.onerror = function() {
            reject(new Error("读取文件失败"));
        };
        reader.readAsDataURL(blob);
    });
}
async function solveImage(file: File) {
    solvingImage.value = true;
    var data = await cropImage(file, 1) as any;
    if (data.blob.size < imageMaxSize * 1024) {
        var base64 = await blobToBase64(data.blob);
        imageProperty.value = {
            isCropped: false,
            width: data.width,
            height: data.height,
            size: data.blob.size
        };
        await uploadImage(base64);
        return;
    }
    var l = 0; var r = 1;
    while (true) {
        var mid = (l + r) / 2;
        var data = await cropImage(file, mid) as any;
        if (data.blob.size < imageMaxSize * 1024 && data.blob.size > imageMaxSize * 1024 * 0.9) {
            var base64 = await blobToBase64(data.blob);
            imageProperty.value = {
                isCropped: true,
                width: data.width,
                height: data.height,
                size: data.blob.size
            };
            await uploadImage(base64);
            return;
        }
        if (data.blob.size > imageMaxSize * 1024) {
            r = mid;
        } else { 
            l = mid;
        }
    }
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
</script>

<template>
    <VOutlined
        class="cursor-pointer"
        @dragover="dragEnterEvent"
        @dragenter.prevent
        @dragleave="dragLeaveEvent"
        @drop="dropEvent"
        @click="clickEvent"
        hover
        :isHover="isDragging"
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
        <div 
            class="d-flex align-center justify-center ga-1" 
            style="width: 100%; height: 32px;" 
            v-if="model == '' && !solvingImage && !uploadingImage"
        >
            <v-icon icon="$mdiCloudUpload" color="primary"></v-icon>
            <span class="text-medium-emphasis">尚未上传图片。请点击选择，或将图片拖拽到此处</span>
        </div>
        <div 
            class="d-flex align-center justify-center ga-1 text-truncate" 
            style="width: 100%; height: 32px;" 
            v-else-if="model == ''"
        >
            <div class="mdi-spin"><v-icon icon="$mdiLoading" color="primary"></v-icon></div>
            <span class="text-medium-emphasis" v-if="solvingImage">处理图片中......</span>
            <span class="text-medium-emphasis" v-if="uploadingImage">
                上传图片中......
                <span v-if="uploadTotal != 1">
                    （{{ Math.round(uploaded / 10.24) / 100.0 }}kb / {{ Math.round(uploadTotal / 10.24) / 100.0 }}kb，{{ Math.round(uploadSpeed / 10.24) / 100.0 }}kb/s）
                </span>
            </span>
        </div>
        <div class="d-flex align-center justify-space-between" style="width: 100%; height: 32px; padding: 0 6px; gap: 6px;" v-else>
            <div class="d-flex align-center ga-1 text-truncate">
                <v-icon icon="$mdiImage" color="primary"></v-icon>
                <span class="text-medium-emphasis">{{ model }}.jpg</span>
                <span class="text-medium-emphasis" v-if="imageProperty.size">
                    ({{ Math.round(imageProperty.size / 10.24) / 100.0 }}kb{{ imageProperty.isCropped ? `，已裁剪至 ${imageProperty.width}x${imageProperty.height}` : "" }})
                </span>
            </div>
            <v-icon
                icon="$mdiCloseCircle"
                :class="`clearButton ${mobile ? 'mobile' : ''}`"
                @click="clearImage"
            ></v-icon>
        </div>
        <v-progress-linear 
            :model-value="uploaded / uploadTotal * 100" class="position-absolute"
            :height="2"
            style="top: none; bottom: 0;"
            color="primary"
            v-if="uploadingImage"
        ></v-progress-linear>
    </VOutlined>
    <v-btn
        icon="$mdiCamera"
        color="primary"
        :disabled="model != '' || solvingImage || uploadingImage"
        @click="cameraPreview = true"
    ></v-btn>
    <ImageOverlay v-model:model="imagePreview" :src="`${API_BASE_URL}/crops/${label.cropId}/images/${model}`"></ImageOverlay>
    <CameraOverlay v-model:model="cameraPreview" @takephoto="solveImage"></CameraOverlay>
</template>

<style lang="css" scoped>
.clearButton {
    opacity: 0;
    transition: opacity 0.28s;
}
.mobile.clearButton {
    opacity: var(--v-medium-emphasis-opacity);
}
</style>