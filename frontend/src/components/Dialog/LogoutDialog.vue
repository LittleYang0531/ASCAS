<script setup lang="ts">
const open = defineModel<boolean>("open", { required: true });
const emits = defineEmits<{
    (e: 'exit'): void,
}>();
</script>

<template>
    <v-dialog v-model="open" max-width="336px">
        <v-card
            prepend-icon="$mdiLogout"
            title="确定退出登录吗"
            class="pt-1"
        >
            <v-list class="pa-5 pt-0">
                <div class="d-flex align-center justify-end ga-2">
                    <v-btn color="error" @click="open = false">取消</v-btn>
                    <v-btn color="primary" @click="open = false; emits('exit')">退出登录</v-btn>
                </div>
            </v-list>
        </v-card>
    </v-dialog>
</template>