<script setup lang="ts">
import { ref } from 'vue';
import { API_BASE_URL } from '../../config';
import { MessageType } from '../../models/message';
import type { Team } from '../../models/team';
import { locate } from '../../router';
import { newFetch } from '../../utils/fetch';
import { showMsg } from '../../utils/message';
import { sleep } from '../../utils/sleep';
import { userId } from '../../utils/user';
import DeleteDialog from '../Dialog/DeleteDialog.vue';

const icons = [ "$mdiAccount", "$mdiCrown" ];
const deleteDialogOpen = ref(false);

const team = defineProps<{
    team: Team
}>();

function formatDate(date: number) {
    var now = new Date();
    var diff = now.getTime() / 1000.0 - date;
    if (diff < 60) {
        return '刚刚';
    } else if (diff < 60 * 60) {
        return Math.floor(diff / 60) + '分钟前';
    } else if (diff < 60 * 60 * 24) {
        return Math.floor(diff / 60 / 60) + '小时前';
    } else if (diff < 60 * 60 * 24 * 30) {
        return Math.floor(diff / 60 / 60 / 24) + '天前';
    } else {
        var dateObj = new Date(date * 1000);
        return dateObj.getFullYear().toString().padStart(4, '0') + '-' + 
               (dateObj.getMonth() + 1).toString().padStart(2, '0') + '-' + 
               dateObj.getDate().toString().padStart(2, '0') + ' ' + 
               dateObj.getHours().toString().padStart(2, '0') + ':' + 
               dateObj.getMinutes().toString().padStart(2, '0') + ':' + 
               dateObj.getSeconds().toString().padStart(2, '0');
    }
}

async function remove() {
    await (await newFetch(`${API_BASE_URL}/teams/${team.team.tid}/remove`, {
        method: 'POST'
    })).json();
    showMsg(MessageType.Success, '删除成功');
    await sleep(1000);
    location.href = location.href;
}
</script>

<template>
    <div class="pa-4 d-flex">
        <div class="flex-grow-1">
            <div class="d-flex align-center ga-2">
                <router-link :to="`/teams/${team.team.tid}`" class="text-title-large">{{ team.team.title }}</router-link>
                <v-icon :icon="icons[Number(userId == team.team.owner!.uid)]"></v-icon>
                <v-chip size="x-small" variant="outlined" class="text-medium-emphasis">
                    {{ userId == team.team.owner!.uid ? "OWNER" : "MEMBER" }}
                </v-chip>
            </div>
            <p class="text-body-small text-medium-emphasis">{{ team.team.description }}</p>
            <div class="d-flex align-center ga-1">
                <v-icon icon="$mdiCrown" size="x-small"></v-icon>
                <router-link 
                    :to="`/users/${team.team.owner?.uid}`" 
                    class="text-body-small text-medium-emphasis"
                >{{ team.team.owner?.name }}</router-link>
                <v-icon icon="$mdiClock" size="x-small" class="ml-2"></v-icon>
                <span class="text-body-small text-medium-emphasis">{{ formatDate(team.team.createdAt!) }}</span>
            </div>
        </div>
        <div class="d-flex flex-column ga-2">
            <v-btn
                color="primary"
                prepend-icon="$mdiPencil"
                @click="locate(`/teams/${team.team.tid}/edit`)"
                v-if="userId == team.team.owner!.uid"
            >编辑</v-btn>
            <v-btn
                color="error"
                prepend-icon="$mdiTrashCan"
                @click="deleteDialogOpen = true"
                v-if="userId == team.team.owner!.uid"
            >删除</v-btn>
            <DeleteDialog v-model:open="deleteDialogOpen" @delete="remove"></DeleteDialog>
        </div>
    </div>
    <v-divider></v-divider>
</template>