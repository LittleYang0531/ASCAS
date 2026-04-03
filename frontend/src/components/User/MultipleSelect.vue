<script setup lang="ts">
import type { Ref } from 'vue';
import type { User } from '../../models/user';
import { ref, watch } from 'vue';
import { API_BASE_URL } from '../../config';
import { newFetch } from '../../utils/fetch';
import VOutlined from '../VOutlined.vue';
import UserCard from './Card.vue';
import InviteDialog from '../Dialog/InviteDialog.vue';

const users = defineModel<Array<User> >("users", { required: true });
const props = defineProps<{
    label: string,
    class?: string,
    enableSelect?: boolean,
    enableInvite?: boolean,
    inviteUrl?: string,
    searchAll?: boolean
}>();
const choose: Ref<number | undefined> = ref(undefined);
const search = ref("");
const items: Ref<Array<any> > = ref([]);
const searchResults: Ref<Array<User> > = ref([]);
const loading = ref(false);

const open = ref(false);

function add() {
    if (choose.value == undefined) return;
    var user = searchResults.value[choose.value]!;
    if (users.value.map((e) => e.uid!).indexOf(user.uid!) == -1) users.value.push(user);
    choose.value = undefined;
}
function remove(item: User) {
    var index = users.value.indexOf(item);
    if (index > -1) {
        users.value.splice(index, 1);
    }
}
watch(search, async (val) => {
    items.value = [];
    if (val == "") {}
    else {
        loading.value = true;
        searchResults.value = (await (await newFetch(`${API_BASE_URL}/users/search?keyword=${val}${props.searchAll ? '&searchAll' : ''}`)).json()).items;
        items.value = [];
        for (var i = 0; i < searchResults.value.length; i++)
            items.value.push({
                title: searchResults.value[i]?.name + "（" + searchResults.value[i]?.email + "）",
                value: i
            });
        loading.value = false;
    }
});
</script>

<template>
    <VOutlined :label="props.label" :class="props.class">
        <div class="d-flex flex-column pa-1" style="width: 100%">
            <v-list class="pa-0" v-if="users.length">
                <UserCard v-for="item in users" :user="item" :active-click="false">
                    <v-btn
                        icon="$mdiTrashCan"
                        color="error"
                        size="small"
                        variant="text"
                        @click="remove(item)"
                    ></v-btn>
                </UserCard>
            </v-list>
            <v-list v-else>
                <v-list-item>
                    <div class="d-flex align-center justify-center ga-1">
                        <v-icon icon="$mdiProgressAlert" color="error"></v-icon>
                        <span>暂无用户</span>
                    </div>
                </v-list-item>
            </v-list>
            <div class="d-flex align-center mt-3 ga-2" v-if="props.enableSelect">
                <v-autocomplete
                    v-model="choose"
                    v-model:search="search"
                    :items="items"
                    :loading="loading"
                    label="查找用户"
                    autocomplete="off"
                    variant="outlined"
                    density="comfortable"
                    active
                    hide-details
                    hide-no-data
                    @update:model-value="add()"
                ></v-autocomplete>
            </div>
            <div class="d-flex align-center mt-3 ga-2" v-if="props.enableInvite">
                <v-btn
                    class="full-width"
                    prepend-icon="$mdiAccountPlus"
                    color="primary"
                    @click="open = true"
                >邀请用户</v-btn>
                <InviteDialog
                    v-model:open="open"
                    :inviteUrl="props.inviteUrl!"
                ></InviteDialog>
            </div>
        </div>
    </VOutlined>
</template>