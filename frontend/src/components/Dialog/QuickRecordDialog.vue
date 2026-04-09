<script setup lang="ts">
import { UserPermission_icons, type Crop } from '../../models/crop';
import { locate } from '../../router';

const open = defineModel<boolean>("open", { required: true });
const props = defineProps<{
    crops: Crop[]
}>();
</script>

<template>
    <v-dialog v-model="open" max-width="336px">
        <v-card
            prepend-icon="$mdiDatabase"
            title="选择作物表"
            class="pt-1"
        >
            <v-list class="pa-5 pt-0">
                <v-list-item v-for="crop in props.crops" @click="locate(`/crops/${crop.cid}?page=add`)">
                    <div class="d-flex align-center ga-2">
                        <v-icon :icon="UserPermission_icons[crop.permission as keyof typeof UserPermission_icons] || '$mdiLock'"></v-icon>
                        <span>{{ crop.title }}</span>
                        <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">id：{{ crop.name }}</v-chip>
                    </div>
                </v-list-item>
            </v-list>
        </v-card>
    </v-dialog>
</template>