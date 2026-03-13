<script lang="ts" setup>
import { UserPermission_icons, type Crop } from '../../models/crop';
import { locate } from '../../router';

const props = defineProps<{
    crop: Crop
}>();

function formatDate(date: number) {
    var now = new Date();
    var diff = now.getTime() / 1000.0 - date;
    if (diff < 60) {
        return '刚刚';
    } else if (diff < 60 * 60) {
        return Math.floor(diff / 60) + '分钟前';
    } else if (diff < 60 * 60 * 24) {
        return Math.floor(diff / 60 / 60) + '小时前';
    } else if (diff < 60 * 60 * 24 * 30) {
        return Math.floor(diff / 60 / 60 / 24) + '天前';
    } else {
        var dateObj = new Date(date * 1000);
        return dateObj.getFullYear().toString().padStart(4, '0') + '-' + 
               (dateObj.getMonth() + 1).toString().padStart(2, '0') + '-' + 
               dateObj.getDate().toString().padStart(2, '0') + ' ' + 
               dateObj.getHours().toString().padStart(2, '0') + ':' + 
               dateObj.getMinutes().toString().padStart(2, '0') + ':' + 
               dateObj.getSeconds().toString().padStart(2, '0');
    }
}
</script>

<template>
    <div class="pa-4 d-flex">
        <div class="flex-grow-1">
            <div class="d-flex align-center ga-2">
                <router-link :to="`/crops/${props.crop.cid}`" class="text-title-large">{{ props.crop.title }}</router-link>
                <v-icon :icon="UserPermission_icons[props.crop.permission as keyof typeof UserPermission_icons] || '$mdiLock'"></v-icon>
                <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">id：{{ props.crop.name }}</v-chip>
                <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">
                    {{ props.crop.permission?.substring("UserPermission::".length) }}
                </v-chip>
            </div>
            <p class="text-body-small text-medium-emphasis">{{ props.crop.description }}</p>
            <div class="d-flex align-center ga-1">
                <v-icon icon="$mdiCrown" size="x-small"></v-icon>
                <router-link 
                    :to="`/users/${props.crop.owner?.uid}`" 
                    class="text-body-small text-medium-emphasis"
                >{{ props.crop.owner?.name }}</router-link>
                <v-icon icon="$mdiClock" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ formatDate(props.crop.createdAt!) }}</span>
                <v-icon icon="$mdiPencil" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ formatDate(props.crop.updatedAt!) }}</span>
            </div>
        </div>
        <div>
            <v-btn
                color="primary"
                prepend-icon="$mdiPencil"
                @click="locate(`/crops/${props.crop.cid}?page=edit`)"
                v-if="props.crop.permission == 'UserPermission::OWNER'"
            >编辑</v-btn>
        </div>
    </div>
    <v-divider></v-divider>
</template>