<script setup lang="ts">
import { onBeforeMount, onBeforeUnmount, ref, watch } from 'vue';

const model = defineModel<boolean>("model", { required: true });
const emits = defineEmits<{
    (e: 'takephoto', data: File): void,
}>();
const video = ref<HTMLVideoElement>();
const naturalWidth = ref(0);
const naturalHeight = ref(0);
const scale = ref(1);
const top = ref(0);
const left = ref(0);
const streaming = ref(false);
const cameraClass = ref("CameraButton-right");

function onresize() {
    scale.value = Math.min(window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    top.value = (window.innerHeight - scale.value * naturalHeight.value) / 2;
    left.value = (window.innerWidth - scale.value * naturalWidth.value) / 2;
    if (window.innerWidth > window.innerHeight) { // 横屏
        cameraClass.value = "CameraButton-right flex-column";
    } else { // 竖屏
        cameraClass.value = "CameraButton-bottom";
    }
};

function takephoto() {
    const canvas = document.createElement('canvas');
    const context = canvas.getContext('2d');
    canvas.width = naturalWidth.value;
    canvas.height = naturalHeight.value;
    context!.drawImage(video.value!, 0, 0, naturalWidth.value, naturalHeight.value);
    canvas.toBlob((blob) => {
        if (blob) {
            const file = new File([blob], `photo.jpg`, { type: 'image/jpeg' });
            emits('takephoto', file);
        }
    }, 'image/jpeg', 0.7);
}

const facingMode = ref<VideoFacingModeEnum>('environment');
function register() {
    navigator.mediaDevices.getUserMedia({ video: { facingMode: facingMode.value }, audio: false })
        .then((stream) => {
            video.value!.addEventListener('canplay', () => {
                if (!streaming.value) {
                    naturalWidth.value = video.value!.videoWidth;
                    naturalHeight.value = video.value!.videoHeight;
                    onresize();
                    streaming.value = true;
                }
            }, false);

            video.value!.srcObject = stream;
            video.value!.play();
        })
        .catch((err) => {
            console.error(`An error occurred: ${err}`);
        });
}

function release() {
    const stream = video.value?.srcObject as MediaStream | undefined;
    if (stream) {
        const tracks = stream.getTracks();
        tracks.forEach((track) => track.stop());
        video.value!.srcObject = null;
    }
}

function reverse(e: Event) {
    e.stopPropagation();
    facingMode.value = facingMode.value == 'environment' ? 'user' : 'environment';
    release();
    register();
}

watch(model, (newVal) => {
    if (newVal) register();
    else release();
});

onBeforeMount(() => {
    window.onresize = onresize;
    if (window.innerWidth > window.innerHeight) { // 横屏
        cameraClass.value = "CameraButton-right flex-column";
    } else { // 竖屏
        cameraClass.value = "CameraButton-bottom";
    }
})
onBeforeUnmount(() => {
    window.onresize = () => {};
})
</script>

<template>
    <v-overlay v-model="model" @click="model = false" width="100%" height="100vh">
        <video 
            ref="video" 
            :width="naturalWidth * scale" 
            :height="naturalHeight * scale" 
            :style="`top: ${top}px; left: ${left}px;`"
            class="position-absolute"
        ></video>
        <v-btn
            icon="$mdiClose"
            class="position-absolute ButtonBase CloseButton ButtonHover"
            @click="model = false"
        ></v-btn>
        <div :class="`position-absolute ${cameraClass} d-flex justify-center align-center ga-2`" @click.prevent>
            <v-btn
                icon="$mdiCamera"
                class="ButtonBase CameraButton ButtonHover"
                @click="takephoto"
            ></v-btn>
            <v-btn
                icon="$mdiCameraFlip"
                class="ButtonBase CameraButton ButtonHover"
                @click="reverse"
            ></v-btn>
        </div>
    </v-overlay>
</template>

<style lang="css" scoped>
.ButtonBase {
    opacity: var(--v-medium-emphasis-opacity);
    transition: opacity 0.28s;
}

.CloseButton {
    top: 16px;
    right: 16px;
    width: 48px;
    height: 48px;
}

.CameraButton {
    width: 48px;
    height: 48px;
}

.CameraButton-right {
    top: calc( 50vh - 24px );
    right: 16px;
}

.CameraButton-bottom {
    bottom: 16px;
    left: calc( 50% - 24px );
}

.DisabledButton {
    opacity: 0.38;
}

.ButtonHover:hover {
    opacity: var(--v-high-emphasis-opacity);
    cursor: pointer;
}
</style>