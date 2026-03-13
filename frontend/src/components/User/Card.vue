<script lang="ts" setup>
import { API_BASE_URL } from '../../config';
import type { User } from '../../models/user';
import { locate } from '../../router';

const props = defineProps<{
    user: User
    activeClick: boolean
}>();
</script>

<template>
    <v-list-item
        :prepend-avatar="`${API_BASE_URL}/users/${props.user.uid}/avatar`"
        :subtitle="props.user.email"
        :title="props.user.name"
        @click="props.activeClick && locate('/users/' + props.user.uid)"
        class="pt-2 pb-2"
    >
        <template v-slot:append>
            <slot></slot>
        </template>
    </v-list-item>
</template>