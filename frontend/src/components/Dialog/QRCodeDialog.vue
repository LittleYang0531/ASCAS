<script setup lang="ts">
import { useDisplay } from 'vuetify';
import VOutlined from '../VOutlined.vue';
import { Html5Qrcode } from 'html5-qrcode';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';
import { ref } from 'vue';
import QrScanOverlay from '../Overlay/QrScanOverlay.vue';

const open = defineModel<boolean>("open", { required: true });
const model = defineModel<string>("model", { required: true });
const mobile = useDisplay().mobile;
const isDragging = ref(false);
const cameraOpen = ref(false);

function cameraScan(result: string) {
    model.value = result;
    open.value = false;
}

async function screenScan() {
    var stream = await navigator.mediaDevices.getDisplayMedia({ video: true });
    var video = document.createElement("video");
    video.srcObject = stream;
    video.onloadedmetadata = () => {
        const canvas = document.createElement('canvas');
        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;
        const context = canvas.getContext('2d') as CanvasRenderingContext2D;
        context.drawImage(video, 0, 0, canvas.width, canvas.height);
        canvas.toBlob((blob) => {
            if (blob) {
                const file = new File([blob], `photo.jpg`, { type: 'image/jpeg' });
                imageScan(file);
            }
        }, 'image/jpeg', 0.7);
    };
    video.play();
}

function imageScan(file: File) {
    const html5QrCode = new Html5Qrcode("qrcode-reader");
    html5QrCode.scanFile(file, true).then((result) => {
        console.log(result);
        model.value = result;
        open.value = false;
    }).catch((err) => {
        showMsg(MessageType.Error, `二维码识别失败：${err}`);
    });
}

function clickEvent() {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = "image/*";
    input.onchange = () => {
        if (input.files?.length) imageScan(input.files[0]!);
    };
    input.click();
}
function dropEvent(e: DragEvent) {
    e.preventDefault();
    const file = e.dataTransfer?.files[0];
    if (file == undefined) return;
    if (file.type.indexOf('image') == -1) {
        showMsg(MessageType.Error, "只能拖拽图片文件");
        return;
    }
    imageScan(file);
};
function dragEnterEvent(e: DragEvent) {
    e.preventDefault();
    isDragging.value = true;
}
function dragLeaveEvent(e: DragEvent) {
    e.preventDefault();
    isDragging.value = false;
}

function pasteEvent(e: ClipboardEvent) {
    e.preventDefault();
    if (!e.clipboardData?.files.length) return;
    const item = e.clipboardData.files[0]!;
    if (item.type.indexOf('image') == -1) {
        showMsg(MessageType.Error, "只能粘贴图片文件");
        return;
    }
    imageScan(item);
}
</script>

<template>
    <v-dialog v-model="open" max-width="448px" @paste="pasteEvent">
        <v-card
            prepend-icon="$mdiQrcode"
            title="扫描二维码"
            class="pt-1"
        >
            <div class="pa-5 pt-0">
                <VOutlined 
                    :class="`cursor-pointer ${isDragging ? 'hovering' : ''}`"
                    @dragover="dragEnterEvent"
                    @dragenter.prevent
                    @dragleave="dragLeaveEvent"
                    @drop="dropEvent"
                    @click="clickEvent"
                    hover
                >
                    <div class="d-flex flex-column align-center justify-center ga-2" style="width: 100%; height: 96px;">
                        <div class="d-flex align-center justify-center ga-2">
                            <v-icon icon="$mdiQrcodeScan" color="primary"></v-icon>
                            <span>请点击选择，或将图片拖拽到此处</span>
                        </div>
                        <div class="d-flex align-center justify-center ga-2">
                            <v-icon icon="$mdiContentPaste" color="primary"></v-icon>
                            <span>或将图片粘贴至此处</span>
                        </div>
                    </div>
                </VOutlined>
                <div class="d-flex align-center justify-center mt-2">
                    <span>或者，扫描已有二维码：</span>
                    <v-btn color="primary" @click="cameraOpen = true">调用相机</v-btn>
                    <v-btn color="primary ml-2" v-if="!mobile" @click="screenScan()">屏幕截图</v-btn>
                </div>
            </div>
        </v-card>
    </v-dialog>
    <QrScanOverlay v-model:model="cameraOpen" @result="cameraScan"></QrScanOverlay>
    <div id="qrcode-reader" class="d-none"></div>
</template>