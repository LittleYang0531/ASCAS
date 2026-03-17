import { ref } from "vue";
import { MessageType } from "../models/message";

var errorListener = 0;
var errorListener2 = 0;
var errorListener3 = 0;
export var isError = ref(false);
export var type = ref(MessageType.Success);
export var errorText = ref("");
export var display = ref("none");
export function showMsg(errorType: MessageType, msg: string) {
	if (errorListener != 0) { clearTimeout(errorListener); }
	if (errorListener2 != 0) { clearTimeout(errorListener2); }
	if (errorListener3 != 0) { clearTimeout(errorListener3); }
	isError.value = false;
	display.value = "block";
	errorListener = setTimeout(() => {
		isError.value = true;
		type.value = errorType;
		errorText.value = msg;
	}, 500);
	errorListener2 = setTimeout(() => { 
		isError.value = false;
	}, 5000);
	errorListener3 = setTimeout(() => {
		display.value = "none";
	}, 5500);
}