<script setup lang="ts">
import type { Crop } from '../../../models/crop';
import UserCard from '../../../components/User/Card.vue';
import { mergeUsers } from '../../../models/user';

const item = defineProps<{
    item: Crop
}>();
</script>

<template>
    <v-list style="width: 100%">
        <UserCard v-for="user in mergeUsers(item.item.editors!, item.item.viewers!, [ item.item.owner! ])" :user="user" :active-click="true"></UserCard>
    </v-list>
</template>