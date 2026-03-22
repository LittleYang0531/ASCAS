<script setup lang="ts">
import { ref } from 'vue';
import type { RecordProperty } from '../../../models/crop';
import QRCodeDialog from '../../Dialog/QRCodeDialog.vue';

const label = defineProps<{ props: RecordProperty, label: string, disabled: boolean, cropId?: number }>();
const model = defineModel<string>("model", { required: true });

const QrcodeOpen = ref(false);
function scanQrcode() {
    QrcodeOpen.value = true;
}
</script>

<template>
    <v-text-field
        v-model="model"
        variant="outlined"
        density="comfortable"
        hide-details
        :disabled="true"
    >
        <template v-slot:label>
            <span>{{ label.label }}</span>
            <span v-if="label.props.required" style="color: red">&nbsp;*</span>
        </template>
    </v-text-field>
    <v-btn
        icon="$mdiQrcodeScan"
        color="primary"
        @click="scanQrcode()"
    ></v-btn>
    <QRCodeDialog v-model:open="QrcodeOpen" v-model:model="model"></QRCodeDialog>
</template>