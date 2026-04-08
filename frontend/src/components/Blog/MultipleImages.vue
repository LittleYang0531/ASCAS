<script setup lang="ts">
// (model = '') => (solvingImage) => (uploadingImage) => (model != '')

import { ref } from 'vue';
import VOutlined from '../VOutlined.vue';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { API_BASE_URL, imageMaxSize } from '../../config';
import ImageLists from './ImageLists.vue';

const model = defineModel<string[]>('model', { required: true });
const props = defineProps<{ bid: number }>();
const isDragging = ref(false);
const solvingImage = ref(false);
const uploadingImage = ref(false);
const uploaded = ref(0);
const uploadTotal = ref(1);
const uploadSpeed = ref(0);

async function uploadImage(file: string) {
    uploaded.value = 0, uploadTotal.value = 1;
    solvingImage.value = false;
    uploadingImage.value = true;
    file = file.substring(file.indexOf(",") + 1);
    return new Promise((resolve, reject) => {
        reject;
        const xhr = new XMLHttpRequest();
        xhr.addEventListener("readystatechange", () => {
            if (xhr.readyState === xhr.DONE) {
                var hash = JSON.parse(xhr.responseText);
                model.value.push(hash["hash"]);
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
        xhr.open("POST", `${API_BASE_URL}/blogs/${props.bid}/images`);
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
        await uploadImage(base64);
        return;
    }
    var l = 0; var r = 1;
    while (true) {
        var mid = (l + r) / 2;
        var data = await cropImage(file, mid) as any;
        if (data.blob.size < imageMaxSize * 1024 && data.blob.size > imageMaxSize * 1024 * 0.9) {
            var base64 = await blobToBase64(data.blob);
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
    if (solvingImage.value || uploadingImage.value) return;
    const input = document.createElement("input");
    input.type = "file";
    input.accept = "image/*";
    input.multiple = true;
    input.onchange = async function() {
        if (input.files) {
            for (var i = 0; i < input.files.length; i++) {
                const file = input.files[i]!;
                await solveImage(file);
            }
        }
    };
    input.click();
}
async function dropEvent(e: DragEvent) {
    if (solvingImage.value || uploadingImage.value) return;
    e.preventDefault();
    e.stopPropagation();
    isDragging.value = false;

    if (e.dataTransfer?.files.length == 0) return;
    var files = e.dataTransfer?.files!;
    for (var i = 0; i < files.length; i++) {
        const file = files[i]!;
        if (!file.type.startsWith("image/")) {
            showMsg(MessageType.Error, "只能上传图片文件");
            continue;
        }
        await solveImage(file);
    }
}
function dragEnterEvent(e: DragEvent) {
    if (solvingImage.value || uploadingImage.value) return;
    e.preventDefault();
    isDragging.value = true;
}
function dragLeaveEvent(e: DragEvent) {
    if (solvingImage.value || uploadingImage.value) return;
    e.preventDefault();
    isDragging.value = false;
}
</script>

<template>
    <VOutlined 
        class="full-width"
        label="博客图片"
        v-if="model.length"
    >
        <div class="d-flex align-center full-width justify-center flex-column">
            <VOutlined
                @dragover="dragEnterEvent"
                @dragenter.prevent
                @dragleave="dragLeaveEvent"
                @drop="dropEvent"
                @click="clickEvent"
                hover
                :isHover="isDragging"
                class="full-width mt-1 cursor-pointer"
            >
                <div class="d-flex align-center justify-center ga-1 full-width" style="height: 64px">
                    <v-icon icon="$mdiCloudUpload" color="primary"></v-icon>
                    <span class="text-medium-emphasis">请点击选择，或将图片拖拽到此处</span>
                </div>
            </VOutlined>
            <div v-if="model.length" class="mt-2 full-width">
                <ImageLists v-model:model="model" :bid="props.bid" enableDelete></ImageLists>
            </div>
        </div>
    </VOutlined>
    <VOutlined
        @dragover="dragEnterEvent"
        @dragenter.prevent
        @dragleave="dragLeaveEvent"
        @drop="dropEvent"
        @click="clickEvent"
        hover
        :isHover="isDragging"
        class="full-width cursor-pointer mt-1"
        label="博客图片"
        v-else 
    >
        <div class="d-flex align-center justify-center ga-1 full-width" style="height: 64px">
            <v-icon icon="$mdiCloudUpload" color="primary"></v-icon>
            <span class="text-medium-emphasis">请点击选择，或将图片拖拽到此处</span>
        </div>
    </VOutlined>
</template>