<script lang="ts" setup>
import { onBeforeUnmount, ref } from 'vue';

const model = defineModel<boolean>("model", { required: true });
const src = defineProps<{
    src: string,
    enableLeft?: boolean
    enableRight?: boolean
}>();
const emits = defineEmits<{
    (e: 'click:left'): void,
    (e: 'click:right'): void,
}>();
const img = ref();
const imgclass = ref("image");
const naturalWidth = ref(0);
const naturalHeight = ref(0);
const left = ref(0);
const top = ref(0);

const previousScale = ref(1);
const scale = ref(1);

function calc(newScale: number, screenX: number, screenY: number) {
    previousScale.value = scale.value;
    scale.value = newScale;

    if (window.innerHeight >= scale.value * naturalHeight.value) {
        top.value = (window.innerHeight - scale.value * naturalHeight.value) / 2;
    } else {
        var imageY = screenY - top.value;
        var newImageY = imageY / previousScale.value * scale.value;
        top.value = screenY - newImageY;
        top.value = Math.min(top.value, 0);
        top.value = Math.max(top.value, window.innerHeight - scale.value * naturalHeight.value);
    }

    if (window.innerWidth >= scale.value * naturalWidth.value) {
        left.value = (window.innerWidth - scale.value * naturalWidth.value) / 2;
    } else {
        var imageX = screenX - left.value;
        var newImageX = imageX / previousScale.value * scale.value;
        left.value = screenX - newImageX;
        left.value = Math.min(left.value, 0);
        left.value = Math.max(left.value, window.innerWidth - scale.value * naturalWidth.value);
    }
}

var lastWheelTime = 0;
var forbidWheel = false;
var wheelInterval: number | undefined = undefined;
function onwheel(e: WheelEvent) {
    e.stopPropagation();
    if (forbidWheel) return;
    var currTime = new Date().getTime();
    if (currTime - lastWheelTime < 32) return;
    clearInterval(wheelInterval);
    imgclass.value = "image imageTransition";
    lastWheelTime = currTime;
    var delta = -e.deltaY / 1000.0;
    var newScale = scale.value + scale.value * delta;
    newScale = Math.max(newScale, 0.01);
    newScale = Math.min(newScale, 8);
    calc(newScale, e.screenX, e.screenY);
    wheelInterval = setInterval(() => {
        imgclass.value = "image";
    }, 280);
}

function onmousedown(e: MouseEvent) {
    e.stopPropagation();
    if (e.button != 0) return;
    document.body.style.cursor = "grab";
    forbidWheel = true;
    var lastX = e.screenX; 
    var lastY = e.screenY;
    window.onmousemove = (e: MouseEvent) => {
        var currX = e.screenX;
        var currY = e.screenY;
        var deltaX = currX - lastX;
        var deltaY = currY - lastY;
        if (window.innerHeight < scale.value * naturalHeight.value) {
            top.value += deltaY;
            top.value = Math.min(top.value, 0);
            top.value = Math.max(top.value, window.innerHeight - scale.value * naturalHeight.value);
        }
        if (window.innerWidth < scale.value * naturalWidth.value) {
            left.value += deltaX;
            left.value = Math.min(left.value, 0);
            left.value = Math.max(left.value, window.innerWidth - scale.value * naturalWidth.value);
        }
        lastX = currX, lastY = currY;
    }
    window.onmouseup = (e: MouseEvent) => {
        if (e.button != 0) return;
        window.onmousemove!(e);
        forbidWheel = false;
        document.body.style.cursor = "";
        window.onmousemove = () => {};
        window.onmouseup = () => {};
    }
}

function onclick(func: any) {
    window.onmouseup = (e: MouseEvent) => {
        func(e);
        window.onmouseup = () => {};
    }
}

var lastWindowWidth = 0;
var lastWindowHeight = 0;
function onresize(e: Event) {
    e;
    var deltaWidth = window.innerWidth - lastWindowWidth;
    var deltaHeight = window.innerHeight - lastWindowHeight;
    top.value += deltaHeight / 2;
    left.value += deltaWidth / 2;
    if (window.innerHeight < scale.value * naturalHeight.value) {
        top.value = Math.min(top.value, 0);
        top.value = Math.max(top.value, window.innerHeight - scale.value * naturalHeight.value);
    }
    if (window.innerWidth < scale.value * naturalWidth.value) {
        left.value = Math.min(left.value, 0);
        left.value = Math.max(left.value, window.innerWidth - scale.value * naturalWidth.value);
    }
    lastWindowWidth = window.innerWidth;
    lastWindowHeight = window.innerHeight;
}

function onload() {
    var image: HTMLImageElement = img.value.image;
    naturalWidth.value = image.naturalWidth;
    naturalHeight.value = image.naturalHeight;
    lastWindowWidth = window.innerWidth;
    lastWindowHeight = window.innerHeight;
    var newScale = Math.min(1, window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    newScale = Math.max(newScale, 0.01);
    newScale = Math.min(newScale, 8);
    calc(newScale, 0, 0);
    window.onwheel = onwheel;
    window.onresize = onresize;
}

function close(e: Event) {
    e.stopPropagation();
    model.value = false;
}

onBeforeUnmount(() => {
    window.onwheel = () => {};
    window.onresize = () => {};
});
</script>

<template>
    <v-overlay v-model="model" @mousedown="onclick(close)" width="100%" height="100vh">
        <v-img 
            :class="imgclass"
            :style="`left: ${left}px; top: ${top}px;`"
            ref="img"
            :src="src.src"
            :width="scale * naturalWidth"
            :height="scale * naturalHeight"
            @load="onload"
            @mousedown="onmousedown"
            :draggable="false"
        ></v-img>
        <v-btn
            icon="$mdiClose"
            class="ButtonBase CloseButton ButtonHover"
            @mousedown="onclick(close)"
        ></v-btn>
        <v-btn
            icon="$mdiArrowLeftBold"
            :class="`ButtonBase LeftButton ${src.enableLeft == undefined || !src.enableLeft ? 'DisabledButton' : 'ButtonHover'}`"
            @mousedown="onclick(emits('click:left'))"
        ></v-btn>
        <v-btn
            icon="$mdiArrowRightBold"
            :class="`ButtonBase RightButton ${src.enableLeft == undefined || !src.enableLeft ? 'DisabledButton' : 'ButtonHover'}`"
            @mousedown="onclick(emits('click:right'))"
        ></v-btn>
    </v-overlay>
</template>

<style lang="css" scoped>
.image {
    position: absolute;
    max-width: none;
    max-height: none;
}

.imageTransition {
    transition: width 0.28s linear, height 0.28s linear, left 0.28s linear, top 0.28s linear;
}

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

.LeftButton {
    top: calc( 50vh - 24px );
    left: 16px;
    width: 48px;
    height: 48px;
}

.RightButton {
    top: calc( 50vh - 24px );
    right: 16px;
    width: 48px;
    height: 48px;
}

.DisabledButton {
    opacity: 0.38;
}

.ButtonHover:hover {
    opacity: var(--v-high-emphasis-opacity);
    cursor: pointer;
}
</style>