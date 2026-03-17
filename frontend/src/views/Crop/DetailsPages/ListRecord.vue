<script setup lang="ts">
import { onBeforeMount, ref } from 'vue';
import type { Crop } from '../../../models/crop';
import { API_BASE_URL } from '../../../config';
import { newFetch } from '../../../utils/fetch';

const item = defineProps<{
    item: Crop
}>();
const page = ref(0);
const data = ref<Record<string, string>>({});

onBeforeMount(async () => {
    var res = await(await newFetch(`${API_BASE_URL}/crops/${item.item.cid}/records/list`, {
        "method": "POST",
        "body": JSON.stringify({
            "where": {
                "isLeaf": true,
                "column": "id",
                "op": "SQLOperator::GREATER_OR_EQUAL",
                "value": 0
            },
            "order": [
                {
                    "column": "id",
                    "isASC": true
                }
            ],
            "limit": 10,
            "offset": 0
        })
    })).json();
    page.value = res.pageCount;
    data.value = res.items;
});
</script>

<template>

</template>