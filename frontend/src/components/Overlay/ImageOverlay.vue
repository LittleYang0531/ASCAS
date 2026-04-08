<script lang="ts" setup>
import { onUnmounted, ref } from 'vue';

const model = defineModel<boolean>("model", { required: true });
const src = defineProps<{
    src: string,
    enableLeft?: boolean,
    enableRight?: boolean,
    opacity?: number,
    title?: string
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
const display = ref(0);

const previousScale = ref(1);
const scale = ref(1);

function calc(newScale: number, clientX: number, clientY: number) {
    previousScale.value = scale.value;
    scale.value = newScale;

    if (window.innerHeight >= scale.value * naturalHeight.value) {
        top.value = (window.innerHeight - scale.value * naturalHeight.value) / 2;
    } else {
        var imageY = clientY - top.value;
        var newImageY = imageY / previousScale.value * scale.value;
        top.value = clientY - newImageY;
        top.value = Math.min(top.value, 0);
        top.value = Math.max(top.value, window.innerHeight - scale.value * naturalHeight.value);
    }

    if (window.innerWidth >= scale.value * naturalWidth.value) {
        left.value = (window.innerWidth - scale.value * naturalWidth.value) / 2;
    } else {
        var imageX = clientX - left.value;
        var newImageX = imageX / previousScale.value * scale.value;
        left.value = clientX - newImageX;
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
    var standardScale = Math.min(window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    newScale = Math.max(newScale, standardScale);
    newScale = Math.min(newScale, 8);
    calc(newScale, e.clientX, e.clientY);
    wheelInterval = setInterval(() => {
        imgclass.value = "image";
    }, 280);
}

function onmousedown(e: MouseEvent) {
    e.stopPropagation();
    if (e.button != 0) return;
    var canSwipe = window.innerWidth > scale.value * naturalWidth.value;
    if (!canSwipe) document.body.style.cursor = "grab";
    forbidWheel = true;
    var realDeltaX = 0;
    var realDeltaY = 0;
    var lastX = e.clientX; 
    var lastY = e.clientY;
    window.onmousemove = (e: MouseEvent) => {
        var currX = e.clientX;
        var currY = e.clientY;
        var deltaX = currX - lastX;
        var deltaY = currY - lastY;
        realDeltaX += deltaX;
        realDeltaY += deltaY;
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
        if (canSwipe && Math.abs(realDeltaX) > Math.abs(realDeltaY) && Math.abs(realDeltaX) > 40) {
            if (realDeltaX > 0) {
                src.enableLeft && emits('click:left');
            } else if (realDeltaX < -0) {
                src.enableRight && emits('click:right');
            }
        }
    }
}

function ontouchstart(e: TouchEvent) {
    e.stopPropagation();
    var realDeltaX = 0;
    var realDeltaY = 0;
    var lastX = e.touches[0]!.clientX; 
    var lastY = e.touches[0]!.clientY;
    window.ontouchmove = (e: TouchEvent) => {
        var currX = e.touches[0]!.clientX;
        var currY = e.touches[0]!.clientY;
        var deltaX = currX - lastX;
        var deltaY = currY - lastY;
        realDeltaX += deltaX;
        realDeltaY += deltaY;
        lastX = currX, lastY = currY;
    }
    window.ontouchend = (e: TouchEvent) => {
        e;
        window.ontouchmove = () => {};
        window.ontouchend = () => {};
        if (realDeltaX > 0) {
            src.enableLeft && emits('click:left');
        } else if (realDeltaX < -0) {
            src.enableRight && emits('click:right');
        }
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
    var lastStandardScale = Math.min(lastWindowWidth / naturalWidth.value, lastWindowHeight / naturalHeight.value);
    var standardScale = Math.min(window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    if (scale.value <= lastStandardScale) scale.value = standardScale;
    scale.value = Math.max(scale.value, standardScale);
    scale.value = Math.min(scale.value, 8);
    if (window.innerHeight < scale.value * naturalHeight.value) {
        top.value = Math.min(top.value, 0);
        top.value = Math.max(top.value, window.innerHeight - scale.value * naturalHeight.value);
    } else {
        top.value = (window.innerHeight - scale.value * naturalHeight.value) / 2;
    }
    if (window.innerWidth < scale.value * naturalWidth.value) {
        left.value = Math.min(left.value, 0);
        left.value = Math.max(left.value, window.innerWidth - scale.value * naturalWidth.value);
    } else {
        left.value = (window.innerWidth - scale.value * naturalWidth.value) / 2;
    }
    lastWindowWidth = window.innerWidth;
    lastWindowHeight = window.innerHeight;
}

function onload() {
    display.value = 1;
    var image: HTMLImageElement = img.value.image;
    naturalWidth.value = image.naturalWidth;
    naturalHeight.value = image.naturalHeight;
    lastWindowWidth = window.innerWidth;
    lastWindowHeight = window.innerHeight;
    var newScale = Math.min(window.innerWidth / naturalWidth.value, window.innerHeight / naturalHeight.value);
    newScale = Math.min(newScale, 8);
    calc(newScale, 0, 0);
    window.onwheel = onwheel;
    window.onresize = onresize;
}

function close(e: Event) {
    e.stopPropagation();
    model.value = false;
}

onUnmounted(() => {
    window.onwheel = () => {};
    window.onresize = () => {};
});
</script>

<template>
    <v-overlay v-model="model" @mousedown="onclick(close)" width="100%" height="100vh">
        <v-img 
            :class="imgclass"
            :style="`left: ${left}px; top: ${top}px; opacity: ${(src.opacity ?? 1) * display};`"
            ref="img"
            :src="src.src"
            :width="scale * naturalWidth"
            :height="scale * naturalHeight"
            @loadstart="display = 0"
            @load="onload"
            @mousedown="onmousedown"
            @touchstart="ontouchstart"
            :draggable="false"
        ></v-img>
        <v-snackbar
            :model-value="true"
            :timeout="-1"
            :text="src.title"
            contained
            @click="$event.stopPropagation()"
            @mousedown="$event.stopPropagation()"
            class="ImageTitle"
            v-if="src.title"
        ></v-snackbar>
        <v-btn
            icon="$mdiClose"
            class="ButtonBase CloseButton ButtonHover"
            @mousedown="$event.stopPropagation(); onclick(close)"
        ></v-btn>
        <v-btn
            icon="$mdiArrowLeftBold"
            :class="`ButtonBase LeftButton ${src.enableLeft == undefined || !src.enableLeft ? 'DisabledButton' : 'ButtonHover'}`"
            @mousedown="$event.stopPropagation(); onclick(() => src.enableLeft && emits('click:left'))"
        ></v-btn>
        <v-btn
            icon="$mdiArrowRightBold"
            :class="`ButtonBase RightButton ${src.enableRight == undefined || !src.enableRight ? 'DisabledButton' : 'ButtonHover'}`"
            @mousedown="$event.stopPropagation(); onclick(() => src.enableRight && emits('click:right'))"
        ></v-btn>
    </v-overlay>
</template>

<style lang="css" scoped>
.image {
    position: absolute;
    max-width: none;
    max-height: none;
    transition: opacity 0.28s;
}

.imageTransition {
    transition: width 0.28s linear, height 0.28s linear, left 0.28s linear, top 0.28s linear, opacity 0.28s;
}

.ImageTitle {
    opacity: 0.7;
    transition: opacity 0.28s;
}
.ImageTitle:hover {
    opacity: 1;
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