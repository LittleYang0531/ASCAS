<script setup lang="ts">
const open = defineModel<boolean>("open", { required: true });
const emits = defineEmits<{
    (e: 'generate'): void,
}>();
</script>

<template>
    <v-dialog v-model="open" max-width="448px">
        <v-card
            prepend-icon="$mdiRefresh"
            title="确定重新生成 Token？"
            class="pt-1"
        >
            <v-list class="pa-5 pt-0">
                <p>一旦重新生成，旧的 Token 将立即失效！请谨慎操作。</p>
                <div class="d-flex align-center justify-end ga-2">
                    <v-btn color="info" @click="open = false">取消</v-btn>
                    <v-btn color="error" @click="open = false; emits('generate')">重新生成</v-btn>
                </div>
            </v-list>
        </v-card>
    </v-dialog>
</template>