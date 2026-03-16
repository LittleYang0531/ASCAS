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
const cameraClass = ref("ButtonBase CameraButton ButtonHover CameraButton-right");

function onresize() {
    scale.value = Math.min(window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    top.value = (window.innerHeight - scale.value * naturalHeight.value) / 2;
    left.value = (window.innerWidth - scale.value * naturalWidth.value) / 2;
    if (window.innerWidth > window.innerHeight) { // 横屏
        cameraClass.value = "ButtonBase CameraButton ButtonHover CameraButton-right"
    } else { // 竖屏
        cameraClass.value = "ButtonBase CameraButton ButtonHover CameraButton-bottom";
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

watch(model, (newVal) => {
    if (newVal) {
        navigator.mediaDevices.getUserMedia({ video: true, audio: false })
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
    } else {
        const stream = video.value?.srcObject as MediaStream | undefined;
        if (stream) {
            const tracks = stream.getTracks();
            tracks.forEach((track) => track.stop());
            video.value!.srcObject = null;
        }
    }
});

onBeforeMount(() => {
    window.onresize = onresize;
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
            :style="{ top: `${top}px`, left: `${left}px` }"
            class="position-absolute"
        ></video>
        <v-btn
            icon="$mdiClose"
            class="ButtonBase CloseButton ButtonHover"
            @click="model = false"
        ></v-btn>
        <v-btn
            ref="camera"
            icon="$mdiCamera"
            :class="cameraClass"
            @click="takephoto"
        ></v-btn>
    </v-overlay>
</template>

<style lang="css" scoped>
.ButtonBase {
    position: absolute;
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