<script setup lang="ts">
import { utils, write } from 'xlsx';
import { API_BASE_URL } from '../../config';
import type { RecordProperty } from '../../models/crop';
import { MessageType } from '../../models/message';
import { showMsg } from '../../utils/message';

const open = defineModel<boolean>("open", { required: true });

const props = defineProps<{
    properties: Array<RecordProperty>,
    cid: number
}>();
const emits = defineEmits<{
    (e: 'getData', callback: (data: Array<Record<string, string>>) => void): void,
}>();

function exportCSV() {
    emits('getData', (data) => {
        var content = "编号,";
        for (var i = 0; i < props.properties.length; i++) {
            content += "\"" + props.properties[i]!.title?.replace(/"/g, '""') + "\",";
        }
        content = content.slice(0, -1) + "\n";
        for (var i = 0; i < data.length; i++) {
            content += data[i]!.id + ",";
            for (var j = 0; j < props.properties.length; j++) {
                var value = data[i]![props.properties[j]!.name!]!;
                if (props.properties[j]!.type == "RecordPropertyType::MULTI") value = JSON.parse(value as string).join(";");
                else if (props.properties[j]!.type == "RecordPropertyType::IMAGE") value = `${API_BASE_URL}/crops/${props.cid}/images/${value}`;
                content += "\"" + value.toString().replace(/"/g, '""') + "\",";
            }
            content = content.slice(0, -1) + "\n";
        }
        var blob = new Blob([content], { type: "text/csv;charset=US-ASCII" });
        var url = URL.createObjectURL(blob);
        var a = document.createElement("a");
        a.href = url;
        a.download = "export.csv";
        a.click();
        URL.revokeObjectURL(url);
        showMsg(MessageType.Success, "导出成功");
        open.value = false;
    });
}
async function exportXLSX() {
    emits('getData', (data) => {
        var workbook = utils.book_new();
        var sheetheader: Array<string> = [ "编号" ];
        for (var i = 0; i < props.properties.length; i++) sheetheader.push(props.properties[i]!.title!);
        var worksheet = utils.aoa_to_sheet([ sheetheader ]);
        for (var i = 0; i < data.length; i++) {
            utils.sheet_add_aoa(worksheet, [[ data[i]!.id! ]], { origin: { r: i + 1, c: 0 } });
            for (var j = 0; j < props.properties.length; j++) {
                var value = data[i]![props.properties[j]!.name!]!;
                if (props.properties[j]!.type == "RecordPropertyType::MULTI") value = JSON.parse(value as string).join(";");
                utils.sheet_add_aoa(worksheet, [[ value ]], { origin: { r: i + 1, c: j + 1 } }); 
                var address = utils.encode_cell({ r: i + 1, c: j + 1 });
                if (props.properties[j]!.type == "RecordPropertyType::IMAGE") {
                    worksheet[address].l = { Target: `${API_BASE_URL}/crops/${props.cid}/images/${value}` };
                    worksheet[address].v = `${value}.jpg`;
                }
            }
        }
        worksheet['!autofilter'] = {
            ref: utils.encode_range({ s: { r: 0, c: 0 }, e: { r: data.length, c: props.properties.length } })
        };
        worksheet['!freeze'] = { xSplit: 1, ySplit: 1 };

        utils.book_append_sheet(workbook, worksheet, "Records");
        var wbout = write(workbook, { bookType: 'xlsx', type: 'array' });
        var blob = new Blob([wbout], { type: "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" });
        var url = URL.createObjectURL(blob);
        var a = document.createElement("a");
        a.href = url;
        a.download = "export.xlsx";
        a.click();
        URL.revokeObjectURL(url);
        showMsg(MessageType.Success, "导出成功");
        open.value = false;
    });
}
async function exportJSON() {
    emits('getData', (data) => {
        var content: Array<any> = [];
        for (var i = 0; i < data.length; i++) {
            var tmp: any = { "编号": data[i]!.id! };
            for (var j = 0; j < props.properties.length; j++) {
                var title = props.properties[j]!.title!;
                var value = data[i]![props.properties[j]!.name!];
                if (props.properties[j]!.type == "RecordPropertyType::MULTI") tmp[title] = JSON.parse(value as string);
                else if (props.properties[j]!.type == "RecordPropertyType::IMAGE") tmp[title] = `${API_BASE_URL}/crops/${props.cid}/images/${value}`;
                else tmp[title] = value;
            }
            content.push(tmp);
        }
        var blob = new Blob([JSON.stringify(content, null, 4)], { type: "application/json;charset=utf-8" });
        var url = URL.createObjectURL(blob);
        var a = document.createElement("a");
        a.href = url;
        a.download = "export.json";
        a.click();
        URL.revokeObjectURL(url);
        showMsg(MessageType.Success, "导出成功");
        open.value = false;
    });
}
</script>

<template>
    <v-dialog v-model="open" max-width="448px">
        <v-card
            prepend-icon="$mdiExport"
            title="选择导出选项"
            class="pt-1"
        >
            <v-list class="pa-5 pt-0">
                <v-list-item
                    class="cursor-pointer"
                    prepend-icon="$mdiFileTable"
                    @click="exportCSV()"
                >   
                    <p class="ma-0">导出为 CSV</p>
                </v-list-item>
                <v-list-item
                    class="cursor-pointer"
                    prepend-icon="$mdiMicrosoftExcel"
                    @click="exportXLSX()"
                >   
                    <p class="ma-0">导出为 XLSX</p>
                </v-list-item>
                <v-list-item
                    class="cursor-pointer"
                    prepend-icon="$mdiCodeJson"
                    @click="exportJSON()"
                >   
                    <p class="ma-0">导出为 JSON</p>
                </v-list-item>
            </v-list>
        </v-card>
    </v-dialog>
</template>