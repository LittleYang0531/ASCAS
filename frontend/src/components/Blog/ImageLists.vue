<script setup lang="ts">
import { ref } from 'vue';
import { API_BASE_URL } from '../../config';
import draggable from 'vuedraggable';
import ImageOverlay from '../Overlay/ImageOverlay.vue';

const model = defineModel<string[]>('model', { required: true });
const props = defineProps<{ bid: number, enableDelete?: boolean }>();

const imageModel = ref(false);
const imageIndex = ref(0);
const imageUrl = ref("");
const imageEnableLeft = ref(false);
const imageEnableRight = ref(false);
const imageOpacity = ref(1);
const imageTitle = ref("");
function showImage(index: number, fadeIn = true) {
    if (fadeIn) { 
        imageEnableLeft.value = false;
        imageEnableRight.value = false;
        imageOpacity.value = 0;
        imageIndex.value = index;
        setTimeout(() => {
            imageUrl.value = `${API_BASE_URL}/blogs/${props.bid}/images/${model.value[index]}`;
            imageOpacity.value = 1;
            imageTitle.value = `${model.value[index]}.jpg`;
        }, 280);
        setTimeout(() => {
            imageEnableLeft.value = index > 0;
            imageEnableRight.value = index < model.value.length - 1;
        }, 560);
    } else {
        imageIndex.value = index;
        imageUrl.value = `${API_BASE_URL}/blogs/${props.bid}/images/${model.value[index]}`;
        imageTitle.value = `${model.value[index]}.jpg`;
        imageModel.value = true;
        imageEnableLeft.value = index > 0;
        imageEnableRight.value = index < model.value.length - 1;
    }
}

function remove(index: number) {
    model.value.splice(index, 1);
}
</script>

<template>
    <draggable v-model="model" animation="200" item-key="name" class="d-flex ga-2 flex-wrap justify-start full-width">
        <template v-slot:item="{ element, index }">
            <div>
                <v-img
                    :src="`${API_BASE_URL}/blogs/${props.bid}/images/${element}`"
                    class="cursor-pointer ImageItem flex-grow-0"
                    width="128"
                    aspect-ratio="1"
                    cover
                    @click="showImage(index, false)"
                >
                    <v-btn 
                        v-if="props.enableDelete" 
                        icon="$mdiClose" 
                        color="error" 
                        class="position-absolute"
                        style="top: 4px; right: 4px;"
                        size="small"
                        width="32"
                        height="32"
                        @click="if (props.enableDelete) { remove(index); $event.stopPropagation(); }"
                    ></v-btn>
                </v-img>
            </div>
        </template>
    </draggable>
    <ImageOverlay 
        v-model:model="imageModel" 
        :src="imageUrl" 
        :enableLeft="imageEnableLeft" 
        :enableRight="imageEnableRight"
        :opacity="imageOpacity"
        :title="imageTitle"
        @click:left="showImage(imageIndex - 1)"
        @click:right="showImage(imageIndex + 1)"
    ></ImageOverlay>
</template>

<style lang="css" scoped>
.ImageItem {
    border-radius: 4px;
    border: 1px solid var(--v-theme-surface);
}
</style>