<script setup lang="ts">
import type { Crop, RecordProperty } from '../../../models/crop';
import PropertyOverview from '../../../components/Property/Overview.vue';
import PropertyDialog from '../../../components/Dialog/PropertyDialog.vue';
import { ref } from 'vue';
import type { Ref } from 'vue';

const item = defineProps<{
    item: Crop
}>();
const showDialog = ref(false);
const dialogData: Ref<RecordProperty> = ref({});
function showPropertyDetails(prop: RecordProperty) {
    dialogData.value = prop;
    showDialog.value = true;
}
</script>

<template>
    <v-list style="width: 100%">
        <PropertyOverview
            v-for="prop in item.item.properties"
            :hasEdit="false"
            :hasRemove="false"
            :hasDetails="true"
            @details="showPropertyDetails(prop)"
            :props="prop"
            @click="showPropertyDetails(prop)"
        ></PropertyOverview>
    </v-list>
    <PropertyDialog
        title="查看作物属性"
        btnTitle="关闭窗口"
        btnIcon="$mdiClose"
        v-model:open="showDialog" 
        v-model:props="dialogData" 
        @submit="showDialog = false"
        :disabled="true"
        :disableType="true"
    ></PropertyDialog>
</template>