<script setup lang="ts">
const open = defineModel<boolean>("open", { required: true });
const props = defineProps<{
    title: string,
    sensorId: string
}>();
const emits = defineEmits<{
    (e: 'submit'): void,
}>();
</script>

<template>
    <v-dialog v-model="open" max-width="448px" persistent>
        <v-card
            prepend-icon="$mdiCheckCircleOutline"
            title="创建成功"
            class="pt-1"
        >
            <v-list class="pa-5 pt-0">
                <p>传感器名称：{{ title }}</p>
                <p>传感器 Token: </p>
                <p>{{ sensorId }}</p>
                <p>请务必保存好您的传感器 Token，以便上传传感器数据。后续您将无法在平台上查看到它。</p>
                <div class="d-flex align-center justify-end ga-2">
                    <v-btn color="primary" @click="open = false; emits('submit')">完成</v-btn>
                </div>
            </v-list>
        </v-card>
    </v-dialog>
</template>