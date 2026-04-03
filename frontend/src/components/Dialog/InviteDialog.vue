<script setup lang="ts">
import { ref } from 'vue';
import UserMultipleSelect from '../User/MultipleSelect.vue';
import type { User } from '../../models/user';
import { newFetch } from '../../utils/fetch';
import { showMsg } from '../../utils/message';
import { MessageType } from '../../models/message';

const open = defineModel<boolean>("open", { required: true });
const url = defineProps<{
    inviteUrl: string
}>();
const users = ref<User[]>([]);
const loading = ref(false);

async function invite() {
    loading.value = true;
    await newFetch(url.inviteUrl, {
        method: "POST",
        body: JSON.stringify(users.value.map((e) => e.uid))
    }, () => loading.value = false);
    showMsg(MessageType.Success, "邀请成功");
    users.value = [];
    loading.value = false;
    open.value = false;
}
</script>

<template>
    <v-dialog v-model="open" max-width="448px">
        <v-card
            prepend-icon="$mdiPencil"
            title="邀请用户"
            class="pt-1"
        >
            <div class="pa-5 pt-0">
                <UserMultipleSelect
                    v-model:users="users"
                    label="被邀请用户"
                    enableSelect
                    searchAll
                ></UserMultipleSelect>
                <div class="mt-3 d-flex justify-center">
                    <v-btn
                        prepend-icon="$mdiSend"
                        color="primary"
                        @click="invite"
                        :disabled="users.length == 0 || loading"
                    >邀请</v-btn>
                </div>
            </div>
        </v-card>
    </v-dialog>
</template>