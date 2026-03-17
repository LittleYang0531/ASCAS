<script setup lang="ts">
import type { Crop } from '../../../models/crop';
import UserCard from '../../../components/User/Card.vue';

const item = defineProps<{
    item: Crop
}>();
</script>

<template>
    <v-list style="width: 100%">
        <UserCard v-for="user in item.item.viewers" :user="user" :active-click="true"></UserCard>
    </v-list>
</template>