<script setup lang="ts">
import { API_BASE_URL } from '../../config';
import type { Talk } from '../../models/message';

const talk = defineProps<{
    talk: Talk,
    currTalk: string
}>();
const emits = defineEmits<{
    (e: 'click'): void
}>();
</script>

<template>
    <v-list-item
        :prepend-avatar="API_BASE_URL + talk.talk.avatar!"
        :subtitle="talk.talk.latest?.message"
        :title="talk.talk.title"
        @click="talk.talk.talkId != talk.currTalk && emits('click')"
        class="pa-4 MessageOverview"
        :active="talk.talk.talkId == talk.currTalk"
    >
        <template v-slot:append v-if="talk.talk.unread! > 0">
            <v-badge
                color="error"
                :content="talk.talk.unread"
                inline
            ></v-badge>
        </template>
    </v-list-item>
</template>

<style lang="css">
.MessageOverview > .v-list-item__append {
    width: 28px;
}
</style>