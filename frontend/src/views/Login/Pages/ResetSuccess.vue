<script setup lang="ts">
const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();
</script>

<template>
    <div>
        <div class="d-flex justify-center mb-4">
            <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
        </div>
        <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">密码重置成功</h2></div>
        <div class="d-flex justify-center mb-4">
            <v-btn
                width="100%"
                color="primary"
                @click="emits('switchPage', 'login')"
            >前往登录</v-btn>
        </div>
    </div>
</template>