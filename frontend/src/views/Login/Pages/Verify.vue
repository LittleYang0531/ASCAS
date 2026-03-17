<script setup lang="ts">
const verified = defineProps<{
    verified: boolean
}>();

const emits = defineEmits<{
    (e: 'switchPage', page: string): void
}>();
</script>

<template>
    <div v-if="verified.verified">
        <div class="d-flex justify-center mb-4">
            <v-icon icon="$mdiCheckCircle" size="64" color="green"></v-icon>
        </div>
        <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">邮箱验证成功</h2></div>
        <div class="d-flex justify-center mb-4">
            <v-btn
                width="100%"
                color="primary"
                @click="emits('switchPage', 'login')"
            >前往登录</v-btn>
        </div>
    </div>
    <div v-else>
        <div class="d-flex justify-center mb-4">
            <v-icon icon="$mdiCloseCircle" size="64" color="red"></v-icon>
        </div>
        <div class="d-flex justify-center mb-10"><h2 class="mt-0 mb-0">邮箱验证失败</h2></div>
        <div class="d-flex flex-column justify-center align-center mb-4">
            <span>验证链接无效或已过期</span>
            <span>请仔细检查您的邮箱，并使用正确的链接进行邮箱验证！</span>
        </div>
    </div>
</template>